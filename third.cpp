#include "third.h"
#include "qevent.h"
#include "ui_third.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPixmap>

Third::Third(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Third)
{
    ui->setupUi(this);


    connect(ui->likeButton, &QPushButton::clicked, this, &Third::on_likeButton_clicked);
    connect(ui->dislikeButton, &QPushButton::clicked, this, &Third::on_dislikeButton_clicked);
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
    else
    {
        qDebug() << "Нет анкет для отображения!";
    }
}





void Third::updateUI()
{
    if (profilesData.isEmpty()) return;

    QMap<QString, QString> profile = profilesData[currentIndex];

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
            ui->profilePhoto->clear();
            qDebug() << "Ошибка загрузки картинки:" << photoPath;
        }
    }
    else
    {
        ui->profilePhoto->clear();
        qDebug() << "Фото отсутствует!";
    }
}




void Third::on_likeButton_clicked()
{
    int userId = profilesData[currentIndex]["id"].toInt();
    saveReaction(userId, true);
    on_nextProfile(); // Переключаем на следующую анкету
}




void Third::on_dislikeButton_clicked()
{
    int userId = profilesData[currentIndex]["id"].toInt();
    saveReaction(userId, false);
    on_nextProfile(); // Переключаем на следующую анкету
}




void Third::on_nextProfile()
{
    if (currentIndex < profilesData.size() - 1)
    {
        currentIndex++;
        updateUI();
    }
    else
    {
        qDebug() << "Вы достигли конца анкет.";
    }
}




void Third::on_prevProfile()
{
    if (currentIndex > 0)
    {
        currentIndex--;
        updateUI();
    }
    else
    {
        qDebug() << "Вы достигли начала анкет.";
    }
}



void Third::saveReaction(int targetUserId, bool isLike)
{
    QSqlQuery query;
    query.prepare("INSERT INTO likes (user_id, target_user_id, like_status) VALUES "
                  "((SELECT id FROM users WHERE login = :login), :target_user_id, :like_status)");
    query.bindValue(":login", currentLogin);
    query.bindValue(":target_user_id", targetUserId);
    query.bindValue(":like_status", isLike);

    if (!query.exec())
    {
        qDebug() << "Ошибка сохранения лайка/дизлайка:" << query.lastError().text();
    }
    else
    {
        qDebug() << "Реакция сохранена: " << (isLike ? "Лайк" : "Дизлайк");
    }
}



void Third::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right)
    {
        on_nextProfile();
    }
    else if (event->key() == Qt::Key_Left)
    {
        on_prevProfile();
    }
}
