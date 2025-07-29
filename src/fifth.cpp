#include "include/fifth.h"
#include "qdatetime.h"
#include "qevent.h"
#include "qmenu.h"
#include "qtimezone.h"
#include "ui_fifth.h"

#include "qsqlerror.h"
#include "qsqlquery.h"
#include <QMessageBox>
#include <QClipboard>

Fifth::Fifth(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Fifth)
{
    ui->setupUi(this);

    setWindowTitle("FlirtFinder");

    // Установка события на контекстное меню
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget, &QListWidget::customContextMenuRequested, this, &Fifth::onContextMenuRequested);
}

Fifth::~Fifth()
{
    delete ui;
}




void Fifth::setUserCredentials(const QString &login, const QString &password, QListWidgetItem *selectedItem)
{
    currentLogin = login;
    currentPassword = password;


    // Получаем ID текущего пользователя
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE login = :login");
    query.bindValue(":login", login);

    if (!query.exec() || !query.next())
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить данные пользователя.");
        qDebug() << "Ошибка SQL:" << query.lastError().text();
        return;
    }

    senderId = query.value(0).toInt();


    // Извлечение ID целевого пользователя из выбранного элемента
    if (selectedItem)
    {
        receiverId = selectedItem->data(Qt::UserRole).toInt();
        if (receiverId <= 0 || receiverId == senderId)
        {
            QMessageBox::warning(this, "Ошибка", "Некорректный или совпадающий ID целевого пользователя.");
            receiverId = -1;
            return;
        }
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось извлечь ID целевого пользователя.");
        receiverId = -1;
    }
}




void Fifth::on_sendButton_clicked()
{
    QString messageText = ui->textEdit->toPlainText();


    if (!ui->textEdit->hasFocus())
    {
        return;
    }

    if (messageText.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Сообщение не может быть пустым.");
        return;
    }

    qDebug() << "Отправка сообщения от senderId:" << senderId << " к receiverId:" << receiverId;


    QSqlQuery query;
    query.prepare(
        "INSERT INTO messages (sender_id, receiver_id, message_text) "
        "VALUES (:senderId, :receiverId, :messageText)"
        );
    query.bindValue(":senderId", senderId);
    query.bindValue(":receiverId", receiverId);
    query.bindValue(":messageText", messageText);

    if (!query.exec())
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось отправить сообщение.");
        qDebug() << "Ошибка SQL:" << query.lastError().text();
        return;
    }

    // Добавляем сообщение в список
    QListWidgetItem *item = new QListWidgetItem(QString("Вы: %1").arg(messageText), ui->listWidget);
    item->setTextAlignment(Qt::AlignRight);
    ui->listWidget->addItem(item);

    ui->textEdit->clear();
    ui->textEdit->setFocus();
}




void Fifth::loadChatHistory(int senderId, int receiverId)
{
    ui->listWidget->clear();

    QSqlQuery query;
    query.prepare(
        "SELECT sender_id, message_text, send_time "
        "FROM messages "
        "WHERE (sender_id = :senderId AND receiver_id = :receiverId) "
        "   OR (sender_id = :receiverId AND receiver_id = :senderId) "
        "ORDER BY send_time ASC"
        );
    query.bindValue(":senderId", senderId);
    query.bindValue(":receiverId", receiverId);

    if (!query.exec())
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить историю чата.");
        qDebug() << "Ошибка выполнения SQL запроса:" << query.lastError().text();
        return;
    }

    while (query.next())
    {
        int msgSenderId = query.value("sender_id").toInt();
        QString messageText = query.value("message_text").toString();

        QDateTime timestamp = query.value("send_time").toDateTime();
        timestamp = timestamp.addSecs(18000);
        timestamp.setTimeSpec(Qt::LocalTime);
        timestamp.setTimeZone(QTimeZone("Asia/Yekaterinburg"));

        QString displayMessage = QString("[%1] %2: %3")
                                     .arg(timestamp.toString("hh:mm"))
                                     .arg(msgSenderId == senderId ? "Вы" : "Собеседник")
                                     .arg(messageText);

        QListWidgetItem *item = new QListWidgetItem(displayMessage, ui->listWidget);
        if (msgSenderId == senderId)
        {
            item->setTextAlignment(Qt::AlignRight); // Ваши сообщения выравниваются вправо
        }
        ui->listWidget->addItem(item);
    }


    if (ui->listWidget->count() > 0)
    {
        ui->listWidget->scrollToBottom();
    }
}




// Контекстное меню
void Fifth::onContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *item = ui->listWidget->itemAt(pos);
    if (!item)
    {
        return;
    }

    QMenu contextMenu(this);

    QAction *replyAction = contextMenu.addAction("Ответить");
    QAction *forwardAction = contextMenu.addAction("Переслать");
    QAction *copyAction = contextMenu.addAction("Копировать");

    QAction *selectedAction = contextMenu.exec(ui->listWidget->mapToGlobal(pos));
    if (!selectedAction)
    {
        return;
    }

    if (selectedAction == replyAction)
    {
        QString truncatedMessage = item->text().left(50); // Оставляем первые 50 символов
        if (item->text().length() > 50)
        {
            truncatedMessage.append("...");
        }

        QListWidgetItem *replyItem = new QListWidgetItem(QString("Ответ на: %1").arg(truncatedMessage), ui->listWidget);
        replyItem->setBackground(QBrush(Qt::lightGray));
        replyItem->setTextAlignment(Qt::AlignLeft);

        ui->listWidget->addItem(replyItem);

        ui->textEdit->setFocus();
    }
    else if (selectedAction == forwardAction)
    {
        QMessageBox::information(this, "Пересылка", QString("Сообщение '%1' переслано!").arg(item->text()));
    }
    else if (selectedAction == copyAction)
    {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(item->text());
        QMessageBox::information(this, "Копирование", "Сообщение скопировано в буфер обмена!");
    }
}
