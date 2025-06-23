#include "include/fourth.h"
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
        "SELECT u.name, u.age, u.city, p.photo_path "
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

        ui->listWidget->addItem(item);
    }

    qDebug() << "Уведомления успешно загружены.";
}



// Нажатие кнопки "Перейти в чат"
void Fourth::onChatButtonClicked()
{
    QMessageBox::information(this, "Чат", "Вы вошли в чат с пользователем!");
}



// Нажатие кнопки "Назад"
void Fourth::onBackButtonClicked()
{
    // auto thirdWindow = new Third();
    // auto secondWindow = new Second();
    // secondWindow->setUserCredentials(login, password);
    // secondWindow->initializeUserData();
    // thirdWindow->setCurrentUserData(login, password);
    // thirdWindow->loadProfiles(login);
    // thirdWindow->show();
    // this->close();
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
        qDebug() << "Не удалось получить ID пользователя.";
        return -1; // Возврат ошибки
    }

    return query.value(0).toInt(); // Возвращаем ID пользователя
}


