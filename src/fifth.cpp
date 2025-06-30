#include "include/fifth.h"
#include "qdatetime.h"
#include "qevent.h"
#include "qtimezone.h"
#include "ui_fifth.h"

#include "qsqlerror.h"
#include "qsqlquery.h"
#include <QMessageBox>

Fifth::Fifth(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Fifth)
{
    ui->setupUi(this);


    connect(ui->textEdit, &QTextEdit::textChanged, this, [=]() {
        QString text = ui->textEdit->toPlainText();
        QStringList lines = text.split("\n");
        QStringList updatedLines;

        for (QString &line : lines)
        {
            while (line.length() > 20)
            {
                // Отрезаем первые 20 символов
                QString chunk = line.left(20);
                updatedLines.append(chunk); // Добавляем отрезанную часть как новую строку
                line = line.mid(20); // Убираем первые 20 символов из строки
            }

            // Добавляем оставшийся текст, если есть
            if (!line.isEmpty())
            {
                updatedLines.append(line);
            }
        }

        // Обновляем текст в `textEdit` с разбиением на строки по 20 символов
        ui->textEdit->blockSignals(true); // Блокируем сигнал, чтобы избежать рекурсии
        ui->textEdit->setPlainText(updatedLines.join("\n"));
        ui->textEdit->moveCursor(QTextCursor::End); // Перемещаем курсор в конец текста
        ui->textEdit->blockSignals(false); // Снимаем блокировку сигналов
    });
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

