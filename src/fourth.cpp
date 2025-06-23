#include "include/fourth.h"
#include "include/fifth.h"
#include "qsqlerror.h"
#include "include/second.h"
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
        item->setData(Qt::UserRole, currentUserId);

        ui->listWidget->addItem(item);
    }

    qDebug() << "Уведомления успешно загружены.";
}



// Нажатие кнопки "Перейти в чат"
void Fourth::onChatButtonClicked()
{
    QListWidgetItem *currentItem = ui->listWidget->currentItem();
    if (!currentItem)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите пользователя для входа в чат.");
        return;
    }

    int targetUserId = currentItem->data(Qt::UserRole).toInt();
    qDebug() << "Открытие чата с userId:" << targetUserId;

    auto fifthWindow = new Fifth(); // Открываем пятый экран
    fifthWindow->setUserCredentials(currentLogin, currentPassword);
    fifthWindow->show();

    this->close();
}



// Нажатие кнопки "Назад"
void Fourth::onBackButtonClicked()
{
    auto thirdWindow = new Third();
    auto secondWindow = new Second();
    secondWindow->setUserCredentials(currentLogin, currentPassword);
    secondWindow->initializeUserData();
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




void Fourth::setUserCredentials(const QString &login, const QString &password)
{
    currentLogin = login;
    currentPassword = password;

    qDebug() << "Логин и пароль установлены: " << login << ", " << password;
}




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





void Fourth::checkMutualLike()
{
    // Получаем выделенный элемент
    QListWidgetItem *currentItem = ui->listWidget->currentItem();
    if (!currentItem)
    {
        ui->ChatButton->setEnabled(false); // Если ничего не выбрано, отключаем кнопку
        return;
    }

    int targetUserId = currentItem->data(Qt::UserRole).toInt();
    qDebug() << "Проверка взаимного лайка для userId:" << targetUserId;

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
        ui->ChatButton->setEnabled(true); // Активируем кнопку
    }
    else
    {
        ui->ChatButton->setEnabled(false); // Отключаем кнопку
    }
}
