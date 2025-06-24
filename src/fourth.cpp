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

    connect(ui->BackButton, &QPushButton::clicked, this, &::Fourth::on_BackButton_Clicked);
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

        this->close();
    }
    else
    {
        // Мэтч отсутствует, выводим сообщение
        QMessageBox::warning(this, "Ошибка", "У вас нет взаимного лайка с этим пользователем.");
    }
}



// Нажатие кнопки "Назад"
void Fourth::on_BackButton_Clicked()
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
