#include "third.h"
#include "ui_third.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QKeyEvent>

Third::Third(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Third)
{
    ui->setupUi(this);
}

Third::~Third()
{
    delete ui;
}




void Third::loadProfiles(const QString &login)
{
    this->currentLogin = login;
    profilesData.clear();

    QSqlQuery query;
    qDebug() << "Запрос на загрузку анкет:";
    query.prepare("SELECT id, name, age, city, photo_path, hobbies FROM users WHERE login != :login");
    query.bindValue(":login", login);

    if (!query.exec())
    {
        qDebug() << "Ошибка загрузки анкет:" << query.lastError().text();
        return;
    }

    while (query.next())
    {
        QMap<QString, QString> profile;
        profile["id"] = query.value("id").toString();
        profile["name"] = query.value("name").toString();
        profile["age"] = query.value("age").toString();
        profile["city"] = query.value("city").toString();
        profile["photo"] = query.value("photo_path").toString();
        profile["hobbies"] = query.value("hobbies").toString();
        profilesData.append(profile);
    }

    if (!profilesData.isEmpty())
    {
        currentIndex = 0;
        updateUI();
    }
}




void Third::updateUI()
{
    if (profilesData.isEmpty())
    {
        qDebug() << "Ошибка: profilesData пуст!";
        return;
    }

    QMap<QString, QString> profile = profilesData[currentIndex];

    qDebug() << "Обновление профиля:" << profile["name"];

    ui->profileName->setText(profile["name"]);
    ui->profileAge->setText(profile["age"] + " лет");
    ui->profileCity->setText("Город: " + profile["city"]);
    ui->profileHobbies->setText(profile["hobbies"]);

    QString photoPath = profile["photo"];
    if (!photoPath.isEmpty())
    {
        QPixmap pixmap(photoPath);
        if (!pixmap.isNull())
        {
            ui->profilePhoto->setPixmap(pixmap.scaled(ui->profilePhoto->size(), Qt::KeepAspectRatio));
        }
        else
        {
            qDebug() << "Ошибка загрузки картинки: " << photoPath;
        }
    }
    else
    {
        qDebug() << "Пустой путь к картинке!";
    }
}



void Third::on_likeButton_clicked()
{
    int userId = profilesData[currentIndex]["id"].toInt();
    updateLikeStatus(userId, true);
}



void Third::on_dislikeButton_clicked()
{
    int userId = profilesData[currentIndex]["id"].toInt();
    updateLikeStatus(userId, false);
}




void Third::updateLikeStatus(int targetUserId, bool isLike)
{
    QSqlQuery query;
    query.prepare("INSERT INTO likes (user_id, target_user_id, like_status) VALUES "
                  "((SELECT id FROM users WHERE login = :login), :target_user_id, :like_status)");
    query.bindValue(":login", currentLogin);
    query.bindValue(":target_user_id", targetUserId);
    query.bindValue(":like_status", isLike);

    if (!query.exec())
    {
        qDebug() << "Ошибка лайка/дизлайка:" << query.lastError().text();
    } else
    {
        qDebug() << "Лайк поставлен: " << isLike;
        /*on_nextProfile_clicked();*/ // Переключаем анкету после лайка/дизлайка
    }
}
