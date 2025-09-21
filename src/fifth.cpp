#include "include/fifth.h"
#include "include/fourth.h"
#include "qboxlayout.h"
#include "qdatetime.h"
#include "qevent.h"
#include "qlabel.h"
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

    // Установка контекстного меню
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




// отправка сообщения
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




// подгрузка истории чата
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

    QAction *replyAction = contextMenu.addAction("↩️ Ответить");
    QAction *forwardAction = contextMenu.addAction("↪️ Переслать");
    QAction *copyAction = contextMenu.addAction("📋 Копировать");
    QAction *editAction = contextMenu.addAction("✏️ Изменить");
    contextMenu.addSeparator();
    QAction *deleteAction = contextMenu.addAction("🗑️ Удалить");

    QFont font = deleteAction->font();
    font.setBold(true);
    deleteAction->setFont(font);

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

        // Сохраняем ID сообщения
        referenceMessageId = item->data(Qt::UserRole).toInt();

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
        QListWidgetItem *item = ui->listWidget->currentItem();
        if (!item)
        {
            QMessageBox::warning(this, "Ошибка", "Выберите сообщение для пересылки.");
            return;
        }

        QString messageText = item->text();

        if (messageText.isEmpty())
        {
            QMessageBox::warning(this, "Ошибка", "Сообщение пустое. Нечего копировать.");
            return;
        }


        // Форматируем сообщение для пересылки
        QString sender = messageText.mid(0, 11);
        QString message = messageText.mid(11, messageText.length());

        QClipboard *clipboard = QApplication::clipboard(); // можно записать это в бд и перейти в др. чат и отправит, без сигналов
        clipboard->setText("➤ Переслано от: " + sender + "\n──────────────────────────────\n" + message);
        qDebug() << "Сообщение: " << messageText;


        auto fourthWindow = new Fourth();
        auto fifthWindow = new Fifth();

        fourthWindow->setUserCredentials(currentLogin, currentPassword);
        fourthWindow->loadNotifications();

        // Показываем окна
        fourthWindow->show();
        fifthWindow->show();
        this->hide();

        // this->deleteLater();

        loadChatHistory(senderId, receiverId);

        // можно записать это в бд и перейти в др. чат и отправить, без сигналов
        // Нужно переписать этот метод чтобы он отправлял пересланное сообщение в бд как обычное с
        // указанием отправителя и получателя и тогда все должно заработать как нужно


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

        // Присутствует баг, в виде неправильного определения id сообщения, из-за этого и не происходит удаление

        // Так же было бы круто если при наведении фон бы краснел, показывая важность действия


        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Подтверждение удаления");
        msgBox.setText("Удаление сообщения");
        msgBox.setInformativeText("Вы уверены, что хотите удалить это сообщение?");
        msgBox.setIcon(QMessageBox::Question);

        QPushButton *yesButton = new QPushButton("Да, удалить");
        QPushButton *noButton = new QPushButton("Нет, отменить");


        yesButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #d32f2f;"
            "   color: white;"
            "   padding: 8px 16px;"
            "   border-radius: 4px;"
            "   min-width: 80px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #b71c1c;"
            "}"
            );

        noButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #f0f0f0;"
            "   color: #333;"
            "   padding: 8px 16px;"
            "   border-radius: 4px;"
            "   min-width: 80px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #e0e0e0;"
            "}"
            );


        msgBox.addButton(yesButton, QMessageBox::YesRole);
        msgBox.addButton(noButton, QMessageBox::NoRole);
        msgBox.setDefaultButton(noButton);

        msgBox.exec();

        if (msgBox.clickedButton() != yesButton)
        {
            qDebug() << "Удаление отменено пользователем";
            return;
        }


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

        if (!query.exec())
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось удалить сообщение.");
            qDebug() << "Ошибка выполнения SQL запроса:" << query.lastError().text();
            return;
        }

        qDebug() << "Сообщение успешно удалено с ID:" << messageId;

        loadChatHistory(senderId, receiverId);


    }
    else if (selectedAction == editAction)
    {
        int messageId = item->data(Qt::UserRole).toInt();
        qDebug() << "Сообщение с ID:" << messageId;

        if (messageId <= 0)
        {
            QMessageBox::warning(this, "Ошибка", "ID сообщения недействителен.");
            qDebug() << "Недействительный ID сообщения:" << messageId;
            return;
        }

        QString currentText = item->text();

        if (currentText.endsWith(" (изменено)"))
        {
            currentText = currentText.left(currentText.length() - 11);
        }


        if (currentText.startsWith("["))
        {
            currentText = currentText.mid(12);
        }


        QDialog editDialog(this);
        editDialog.setWindowTitle("Редактирование сообщения");
        editDialog.setMinimumWidth(400);

        QVBoxLayout *layout = new QVBoxLayout(&editDialog);

        QLabel *label = new QLabel("Редактируйте сообщение:", &editDialog);
        QTextEdit *textEdit = new QTextEdit(&editDialog);
        textEdit->setPlainText(currentText);
        textEdit->setFocus();

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        QPushButton *saveButton = new QPushButton("Сохранить", &editDialog);
        QPushButton *cancelButton = new QPushButton("Отменить", &editDialog);


        saveButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #4caf50;"
            "   color: white;"
            "   padding: 8px 16px;"
            "   border-radius: 4px;"
            "   min-width: 80px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #388e3c;"
            "}"
            "QPushButton:disabled {"
            "   background-color: #a5d6a7;"
            "}"
            );

        cancelButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #f0f0f0;"
            "   color: #333;"
            "   padding: 8px 16px;"
            "   border-radius: 4px;"
            "   min-width: 80px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #e0e0e0;"
            "}"
            );

        buttonLayout->addWidget(saveButton);
        buttonLayout->addWidget(cancelButton);

        layout->addWidget(label);
        layout->addWidget(textEdit);
        layout->addLayout(buttonLayout);

        connect(saveButton, &QPushButton::clicked, &editDialog, &QDialog::accept);
        connect(cancelButton, &QPushButton::clicked, &editDialog, &QDialog::reject);

        connect(textEdit, &QTextEdit::textChanged, [saveButton, textEdit]() {
            saveButton->setEnabled(!textEdit->toPlainText().trimmed().isEmpty());
        });

        if (editDialog.exec() != QDialog::Accepted)
        {
            qDebug() << "Редактирование отменено пользователем";
            return;
        }

        QString newText = textEdit->toPlainText().trimmed();

        if (newText.isEmpty())
        {
            QMessageBox::warning(this, "Ошибка", "Сообщение не может быть пустым.");
            return;
        }

        if (newText == currentText)
        {
            qDebug() << "Текст сообщения не изменился";
            return;
        }


        QString newTextWithMark = textEdit->toPlainText().trimmed() + " (изменено)";

        QSqlQuery query;
        query.prepare("UPDATE messages SET message_text  = :newText "
                      "WHERE id = :messageId");   // AND sender_id =: senderId
        query.bindValue(":newText", newTextWithMark);
        // query.bindValue(":senderId", senderId);   -- это чтобы можно было редачить только свои сообщения
        query.bindValue(":messageId", messageId);

        if (!query.exec())
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось обновить сообщение.");
            qDebug() << "Ошибка выполнения SQL запроса:" << query.lastError().text();
            return;
        }

        qDebug() << "Сообщение успешно обновлено с ID:" << messageId;

        loadChatHistory(senderId, receiverId);
        QMessageBox::information(this, "Успех", "Сообщение успешно отредактировано.");
    }
}
