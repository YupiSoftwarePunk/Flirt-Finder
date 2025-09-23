#include "include/fourth.h"
#include "include/fifth.h"
#include "qsqlerror.h"
#include "include/third.h"
#include "ui_fourth.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QPixmap>


Fourth::Fourth(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Fourth)
{
    ui->setupUi(this);


    connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &Fourth::checkMutualLike);

    connect(ui->searchButton, &QPushButton::clicked, this, &::Fourth::performSearch);


    // Создаём валидатор с разрешёнными символами
    QRegularExpression regex("^[a-zA-Zа-яА-ЯёЁ0-9.,\\n]*$");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, this);

    // Устанавливаем валидатор для поля поиска
    ui->searchInput->setValidator(validator);


    // поиск по нажатию на enter
    connect(ui->searchInput, &QLineEdit::returnPressed, ui->searchButton, &QPushButton::click);

    setWindowTitle("FlirtFinder");
}

Fourth::~Fourth()
{
    delete ui;
}


// Загрузка уведомлений
void Fourth::loadNotifications()
{
    // Получаем ID текущего пользователя
    int currentUserId = getCurrentUserId(currentLogin);

    if (currentUserId == -1)
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить уведомления. ID пользователя не найден.");
        return;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT u.id, u.name, u.age, u.city, p.photo_path "
        "FROM users u "
        "INNER JOIN likes_dislikes l ON u.id = l.user_id "
        "LEFT JOIN photos p ON u.id = p.user_id "
        "WHERE l.liked_by = :currentUserId AND l.reaction = 1"
        );
    query.bindValue(":currentUserId", currentUserId);

    if (!query.exec())
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить уведомления.");
        qDebug() << "Ошибка выполнения SQL:" << query.lastError().text();
        return;
    }

    while (query.next())
    {
        int userId = query.value("id").toInt();
        QString name = query.value("name").toString();
        int age = query.value("age").toInt();
        QString city = query.value("city").toString();
        QString photoPath = query.value("photo_path").toString();

        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        item->setText(QString("%1, %2 лет, %3").arg(name).arg(age).arg(city));

        if (!photoPath.isEmpty())
        {
            QPixmap pixmap(photoPath);
            item->setIcon(QIcon(pixmap.scaled(50, 50, Qt::KeepAspectRatio)));
        }
        item->setData(Qt::UserRole, userId);

        ui->listWidget->addItem(item);
    }

    qDebug() << "Уведомления успешно загружены.";
}



// Нажатие кнопки "Перейти в чат"
void Fourth::on_ChatButton_clicked()
{
    QListWidgetItem *currentItem = ui->listWidget->currentItem();
    if (!currentItem)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите пользователя для входа в чат.");
        return;
    }

    int targetUserId = currentItem->data(Qt::UserRole).toInt(); // Извлекаем ID целевого пользователя
    qDebug() << "Проверка взаимного лайка для targetUserId:" << targetUserId;


    if (targetUserId <= 0 || targetUserId == getCurrentUserId(currentLogin))
    {
        QMessageBox::warning(this, "Ошибка", "Некорректный или совпадающий ID целевого пользователя.");
        return;
    }

    // Проверяем взаимный лайк в базе данных
    QSqlQuery query;
    query.prepare(
        "SELECT COUNT(*) "
        "FROM likes_dislikes AS l1 "
        "INNER JOIN likes_dislikes AS l2 "
        "ON l1.user_id = l2.liked_by AND l1.liked_by = l2.user_id "
        "WHERE l1.user_id = :currentUserId AND l2.user_id = :targetUserId "
        "AND l1.reaction = 1 AND l2.reaction = 1"
        );
    query.bindValue(":currentUserId", getCurrentUserId(currentLogin)); // ID текущего пользователя
    query.bindValue(":targetUserId", targetUserId); // ID целевого пользователя

    if (!query.exec() || !query.next())
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось проверить взаимный лайк.");
        qDebug() << "Ошибка выполнения SQL запроса:" << query.lastError().text();
        return;
    }

    int mutualLikeCount = query.value(0).toInt();
    if (mutualLikeCount == 1)
    {
        // Успешный мэтч, открываем пятый экран
        qDebug() << "Взаимный лайк подтверждён!";
        auto fifthWindow = new Fifth();

        fifthWindow->setUserCredentials(currentLogin, currentPassword, currentItem); // Передача данных
        fifthWindow->loadChatHistory(getCurrentUserId(currentLogin), targetUserId);
        fifthWindow->show();
    }
    else
    {
        // Мэтч отсутствует
        QMessageBox::warning(this, "Ошибка", "У вас нет взаимного лайка с этим пользователем.");
    }
}



// Нажатие кнопки "Назад"
void Fourth::on_BackButton_clicked()
{
    auto thirdWindow = new Third();
    thirdWindow->setCurrentUserData(currentLogin, currentPassword);
    thirdWindow->loadProfiles(currentLogin);
    thirdWindow->show();
    this->close();
}



// Нажатие кнопки "Лайкнуть в ответ"
void Fourth::on_LikeButton_clicked()
{
    QListWidgetItem *currentItem = ui->listWidget->currentItem();

    if (!currentItem)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите пользователя, чтобы поставить лайк.");
        return;
    }

    int userId = currentItem->data(Qt::UserRole).toInt();
    if (userId <= 0)
    {
        QMessageBox::warning(this, "Ошибка", "ID пользователя некорректен.");
        qDebug() << "userId = "<<userId;
        return;
    }

    QSqlQuery query;
    query.prepare(
        "INSERT INTO likes_dislikes (user_id, liked_by, reaction) "
        "VALUES (:user_id, (SELECT id FROM users WHERE login = :login), 1) "
        "ON CONFLICT (user_id, liked_by) DO UPDATE SET reaction = 1"
        );
    query.bindValue(":user_id", userId);
    query.bindValue(":login", currentLogin);

    if (query.exec())
    {
        QMessageBox::information(this, "Лайк", "Вы лайкнули пользователя в ответ!");
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось поставить лайк.");
        qDebug() << "Ошибка выполнения SQL:" << query.lastError().text();
    }
}




// Загрузка пользовательских данных
void Fourth::setUserCredentials(const QString &login, const QString &password)
{
    currentLogin = login;
    currentPassword = password;

    qDebug() << "Логин и пароль установлены: " << login << ", " << password;
}




// получить id пользователя
int Fourth::getCurrentUserId(const QString &login)
{
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE login = :login");
    query.bindValue(":login", login);

    if (!query.exec() || !query.next())
    {
        qDebug() << "Ошибка: ID пользователя для логина " << login << " не найден.";
        return -1;
    }

    return query.value(0).toInt();
}





// проверка взаимного лайка
void Fourth::checkMutualLike()
{
    QListWidgetItem *currentItem = ui->listWidget->currentItem();
    if (!currentItem)
    {
        ui->ChatButton->setEnabled(false);
        qDebug() << "Элемент не выбран. Отключаем кнопку.";
        return;
    }

    int targetUserId = currentItem->data(Qt::UserRole).toInt();
    qDebug() << "Проверка взаимного лайка для userId:" << targetUserId;

    QSqlQuery query;
    query.prepare(
        "SELECT COUNT(*) "
        "FROM likes_dislikes AS l1 "
        "INNER JOIN likes_dislikes AS l2 "
        "ON l1.user_id = l2.liked_by AND l1.liked_by = l2.user_id "
        "WHERE l1.user_id = :currentUserId AND l2.user_id = :targetUserId "
        "AND l1.reaction = 1 AND l2.reaction = 1"
        );

    query.bindValue(":currentUserId", getCurrentUserId(currentLogin));
    query.bindValue(":targetUserId", targetUserId);

    if (!query.exec() || !query.next())
    {
        qDebug() << "Ошибка выполнения SQL запроса:" << query.lastError().text();
        ui->ChatButton->setEnabled(false);
        return;
    }

    int mutualLikeCount = query.value(0).toInt();
    if (mutualLikeCount == 1)
    {
        ui->ChatButton->setEnabled(true);
        qDebug() << "Взаимный лайк подтверждён. Кнопка активирована.";
    }
    else
    {
        ui->ChatButton->setEnabled(false);
        qDebug() << "Взаимный лайк отсутствует. Кнопка отключена.";
    }
}




// подробная информация о пользователе
void Fourth::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (!item)
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось извлечь данные для перехода.");
        return;
    }

    // Извлекаем ID выбранного пользователя
    int targetUserId = item->data(Qt::UserRole).toInt();



    // пересылка сообщений
    if (isForwardMode)
    {
        if (forwardSenderId > 0 && !forwardMessageText.isEmpty())
        {
            QSqlQuery query;
            query.prepare("INSERT INTO messages (sender_id, receiver_id, message_text) "
                          "VALUES (:senderId, :receiverId, :messageText)");
            query.bindValue(":senderId", forwardSenderId);
            query.bindValue(":receiverId", targetUserId);
            query.bindValue(":messageText", forwardMessageText);

            if (query.exec())
            {
                qDebug() << "Сообщение переслано от" << forwardSenderId << "к" << targetUserId;
            }

            auto fifthWindow = new Fifth();
            fifthWindow->loadChatHistory(forwardSenderId, targetUserId);
            fifthWindow->show();
            this->close();
        }

        return;
    }





    // открытие анкеты пользователя
    if (targetUserId <= 0)
    {
        QMessageBox::warning(this, "Ошибка", "Некорректный ID пользователя.");
        return;
    }

    qDebug() << "Открытие анкеты для targetUserId: " << targetUserId;

    // Выполняем запрос в БД, чтобы загрузить данные пользователя
    QSqlQuery query;
    query.prepare(
        "SELECT u.name, u.age, u.city, u.hobbies, p.photo_path "
        "FROM users u "
        "LEFT JOIN photos p ON u.id = p.user_id "
        "WHERE u.id = :userId"
        );
    query.bindValue(":userId", targetUserId);

    if (!query.exec() || !query.next())
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить данные анкеты.");
        qDebug() << "Ошибка SQL: " << query.lastError().text();
        return;
    }

    // Извлекаем данные пользователя
    QString name = query.value("name").toString();
    int age = query.value("age").toInt();
    QString city = query.value("city").toString();
    QString photoPath = query.value("photo_path").toString();
    QString hobby = query.value("hobbies").toString();

    // Открываем 3 страницу для отображения данных пользователя
    auto thirdWindow = new Third();
    thirdWindow->hideAllButtons();
    thirdWindow->setProfileData(name, age, city, photoPath, hobby);
    thirdWindow->show();
}





// поиск. Обработка пользователей по релевантности
void Fourth::performSearch()
{
    QString searchText = ui->searchInput->text().trimmed();

    // выше ^

    // Если строка пустая, показываем все элементы
    if (searchText.isEmpty())
    {
        for (int i = 0; i < ui->listWidget->count(); ++i)
        {
            QListWidgetItem *item = ui->listWidget->item(i);
            item->setHidden(false);
        }
        return;
    }

    // Фильтруем элементы в listWidget
    for (int i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        QString itemText = item->text();

        if (itemText.contains(searchText, Qt::CaseInsensitive))
        {
            item->setHidden(false);
        }
        else
        {
            item->setHidden(true);
        }
    }
}




// нажатие на кнопку поиска и перейти в чат
void Fourth::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        if (ui->searchInput->hasFocus())
        {
            ui->searchButton->click();
            event->accept();
            return;
        }
        else if (ui->listWidget->hasFocus())
        {
            ui->ChatButton->click();
            event->accept();
            return;
        }
    }
    else
    {
        QDialog::keyPressEvent(event);
    }
}




// Задание полей для пересылки сообщения
void Fourth::setForwardData(int senderId, const QString &messageText)
{
    forwardSenderId = senderId;
    forwardMessageText = messageText;

    qDebug() << "Fourth: получены данные для пересылки, senderId:" << senderId;
}




// Задание режима пересылки для double click
void Fourth::setForwardMode(bool isForward)
{
    isForwardMode = isForward;
}
