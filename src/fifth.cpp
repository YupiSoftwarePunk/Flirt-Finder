#include "include/fifth.h"
#include "include/CustomMenuStyle.h"
#include "include/fourth.h"
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
        "INSERT INTO messages (sender_id, receiver_id, message_text, reference_message_id) "
        "VALUES (:senderId, :receiverId, :messageText, :referenceMessageId)"
        );
    query.bindValue(":senderId", senderId);
    query.bindValue(":receiverId", receiverId);
    query.bindValue(":messageText", messageText);

    // Проверяем, есть ли ссылка на другое сообщение
    if (referenceMessageId != -1)
    {
        query.bindValue(":referenceMessageId", referenceMessageId);
        qDebug() << "referenceMessageId при отправке:" << referenceMessageId;

    }
    else
    {
        query.bindValue(":referenceMessageId", QVariant(QVariant::Int));
    }

    if (!query.exec())
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось отправить сообщение.");
        qDebug() << "Ошибка SQL:" << query.lastError().text();
        return;
    }

    // Добавляем сообщение в список
    QListWidgetItem *item = new QListWidgetItem(QString("Вы: %1").arg(messageText), ui->listWidget);
    item->setTextAlignment(Qt::AlignRight);
    if (referenceMessageId != -1)
    {
        item->setData(Qt::UserRole, referenceMessageId);
    }
    ui->listWidget->addItem(item);

    ui->textEdit->clear();
    ui->textEdit->setFocus();

    referenceMessageId = -1;
}




void Fifth::loadChatHistory(int senderId, int receiverId)
{
    ui->listWidget->clear();

    QSqlQuery query;
    query.prepare(
        "SELECT id, sender_id, message_text, send_time, reference_message_id "
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


    QString receiverName; // Получение имени собеседника
    QSqlQuery userQuery;
    userQuery.prepare("SELECT name FROM users WHERE id = :receiverId");
    userQuery.bindValue(":receiverId", receiverId);

    if (userQuery.exec() && userQuery.next()) {
        receiverName = userQuery.value("name").toString();
    }
    else
    {
        receiverName = "Собеседник";
    }

    while (query.next())
    {
        int messageId = query.value("id").toInt();
        int msgSenderId = query.value("sender_id").toInt();
        QString messageText = query.value("message_text").toString();
        int referenceMessageId = query.value("reference_message_id").toInt();

        QDateTime timestamp = query.value("send_time").toDateTime();
        timestamp = timestamp.addSecs(18000);
        timestamp.setTimeSpec(Qt::LocalTime);
        timestamp.setTimeZone(QTimeZone("Asia/Yekaterinburg"));

        QString displayMessage = QString("[%1] %2: %3")
                                     .arg(timestamp.toString("hh:mm"))
                                     .arg(msgSenderId == senderId ? "Вы" : receiverName)
                                     .arg(messageText);

        QListWidgetItem *item = new QListWidgetItem(displayMessage, ui->listWidget);
        item->setData(Qt::UserRole, messageId);

        if (msgSenderId == senderId)
        {
            item->setTextAlignment(Qt::AlignRight); // Ваши сообщения выравниваются вправо
        }
        if (referenceMessageId > 0)
        {
            item->setData(Qt::UserRole, referenceMessageId);

            item->setBackground(QBrush(Qt::lightGray)); // Серый фон для ответа
            item->setToolTip("Ответ на сообщение ID: " + QString::number(referenceMessageId));
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
    QAction *deleteAction = contextMenu.addAction("Удалить");

    deleteAction->setObjectName("dangerAction");

    QAction *selectedAction = contextMenu.exec(ui->listWidget->mapToGlobal(pos));

    if (!selectedAction)
    {
        return;
    }

    if (selectedAction == replyAction)
    {
        // иногда возникает ошибка что неправильно определяется id сообщения
        // и ответ идет не только на последнее выделенное сообщение, но и идет после него

        QString truncatedMessage = item->text().left(50); //первые 50 символов
        if (item->text().length() > 50)
        {
            truncatedMessage.append("...");
        }

        // Сохраняем ID сообщения для referenceMessageId
        referenceMessageId = item->data(Qt::UserRole).toInt(); // Предполагается, что ID хранится в UserRole

        QListWidgetItem *replyItem = new QListWidgetItem(QString("Ответ на: %1").arg(truncatedMessage), ui->listWidget);
        replyItem->setBackground(QBrush(Qt::lightGray));
        replyItem->setTextAlignment(Qt::AlignLeft);

        ui->listWidget->addItem(replyItem);

        ui->textEdit->setText(QString("➤ Ответ на: %1\n──────────────────────────────\n").arg(truncatedMessage));
        ui->textEdit->moveCursor(QTextCursor::End);
        ui->textEdit->setFocus();

        loadChatHistory(senderId, receiverId);


    }
    else if (selectedAction == forwardAction)
    {
        QString messageText = item->text();

        if (messageText.isEmpty())
        {
            QMessageBox::warning(this, "Ошибка", "Сообщение пустое. Нечего копировать.");
            return;
        }

        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(messageText);
        qDebug() << "Сообщение: " << messageText;


        auto fourthWindow = new Fourth();
        fourthWindow->setUserCredentials(currentLogin, currentPassword);
        fourthWindow->loadNotifications();
        fourthWindow->show();


        connect(fourthWindow, &Fourth::switchStateChanged, this, [this, &messageText, &fourthWindow](bool switchState) {
        if (switchState)
        {
            qDebug() << "Переключение в чат активировано. Вставка текста.";

            QMetaObject::invokeMethod(this, [=]()
            {
                ui->textEdit->setFocus();
                ui->textEdit->setText(messageText );
                on_sendButton_clicked();
            });

            fourthWindow->deleteLater();

            qDebug() << "Сообщение отправлено: " << messageText;
        }
        else
        {
            qDebug() << "Ошибка: ui->textEdit недоступен.";
            qDebug() << "Switch_ не активирован.";
        }
        });

        this->deleteLater();

        loadChatHistory(senderId, receiverId);


    }
    else if (selectedAction == copyAction)
    {
        QString messageText = item->text();

        int colonIndex = messageText.indexOf(": ");
        if (colonIndex != -1)
        {
            QString textToCopy = messageText.mid(colonIndex + 2);

            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setText(textToCopy);

            QMessageBox::information(this, "Копирование", "Текст сообщения скопирован в буфер обмена!");
        }
        else
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось определить текст сообщения для копирования.");
        }


    }
    else if (selectedAction == deleteAction)
    {
        // иногда возникает ошибка что неправильно определяется id сообщения и удаления не происходит
        // необходимо найти решение этой проблемы

        int messageId = item->data(Qt::UserRole).toInt();
        qDebug() << "Сообщение с ID:" << messageId;

        if (messageId <= 0)
        {
            QMessageBox::warning(this, "Ошибка", "ID сообщения недействителен.");
            qDebug() << "Недействительный ID сообщения:" << messageId;
            return;
        }

        QSqlQuery query;
        query.prepare("DELETE FROM messages WHERE id = :messageId");
        query.bindValue(":messageId", messageId);

        if (!query.exec()) {
            QMessageBox::warning(this, "Ошибка", "Не удалось удалить сообщение.");
            qDebug() << "Ошибка выполнения SQL запроса:" << query.lastError().text();
            return;
        }

        qDebug() << "Сообщение успешно удалено с ID:" << messageId;

        QMessageBox::information(this, "Удаление", "Сообщение успешно удалено!");

        loadChatHistory(senderId, receiverId);
    }
}

