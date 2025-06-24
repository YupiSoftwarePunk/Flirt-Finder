#include "include/fifth.h"
#include "qdatetime.h"
#include "ui_fifth.h"

#include "qsqlerror.h"
#include "qsqlquery.h"
#include <QMessageBox>

Fifth::Fifth(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Fifth)
{
    ui->setupUi(this);
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
        receiverId = selectedItem->data(Qt::UserRole).toInt(); // Получаем receiverId из пользовательских данных элемента
        qDebug() << "ReceiverId для чата: " << receiverId;
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось извлечь ID целевого пользователя.");
        receiverId = -1; // Установим некорректное значение для предотвращения отправки сообщений
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

    // Вставляем сообщение в базу данных
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

    ui->textEdit->clear(); // Очищаем поле ввода
}




void Fifth::loadChatHistory(int senderId, int receiverId)
{
    ui->listWidget->clear(); // Очищаем список сообщений

    QSqlQuery query;
    query.prepare(
        "SELECT sender_id, message_text, timestamp FROM messages "
        "WHERE (sender_id = :senderId AND receiver_id = :receiverId) "
        "   OR (sender_id = :receiverId AND receiver_id = :senderId) "
        "ORDER BY timestamp ASC"
        );
    query.bindValue(":senderId", senderId);
    query.bindValue(":receiverId", receiverId);

    if (!query.exec())
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить историю чата.");
        qDebug() << "Ошибка SQL:" << query.lastError().text();
        return;
    }

    while (query.next())
    {
        int msgSenderId = query.value("sender_id").toInt();
        QString messageText = query.value("message_text").toString();
        QDateTime timestamp = query.value("timestamp").toDateTime();

        QString displayMessage = QString("[%1] %2: %3")
                                     .arg(timestamp.toString("hh:mm"))
                                     .arg(msgSenderId == senderId ? "Вы" : "Собеседник")
                                     .arg(messageText);

        QListWidgetItem *item = new QListWidgetItem(displayMessage, ui->listWidget);
        if (msgSenderId == senderId)
        {
            item->setTextAlignment(Qt::AlignRight); // Выравнивание для ваших сообщений
        }
        ui->listWidget->addItem(item);
    }
}

