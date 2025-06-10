#include "third.h"
#include "qevent.h"
#include "ui_third.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPixmap>
#include <QMessageBox>

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
    profilesData.clear(); // Очищаем массив перед загрузкой

    QSqlQuery query;
    query.prepare("SELECT users.id, users.name, users.age, users.city, users.hobbies, photos.photo_path "
                  "FROM users LEFT JOIN photos ON users.id = photos.user_id WHERE users.login != :login");
    query.bindValue(":login", login);

    if (!query.exec())
    {
        qDebug() << "Ошибка выполнения SQL:" << query.lastError().text();
        return;
    }

    while (query.next())
    {
        QMap<QString, QString> profile;
        profile["id"] = query.value("id").toString();
        profile["name"] = query.value("name").toString();
        profile["age"] = query.value("age").toString();
        profile["city"] = query.value("city").toString();
        profile["hobbies"] = query.value("hobbies").toString();
        profile["photo"] = query.value("photo_path").toString(); // Путь к фото
        profilesData.append(profile);
    }

    qDebug() << "Загружено анкет:" << profilesData.size();

    if (!profilesData.isEmpty())
    {
        currentIndex = 0;
        updateUI(); // Отображаем первую анкету
    }
    else
    {
        QMessageBox::information(this, "Информация", "Нет доступных анкет для отображения.");
    }
}





void Third::updateUI()
{
    if (profilesData.isEmpty() || currentIndex >= profilesData.size())
    {
        qDebug() << "Ошибка: нет данных для отображения.";
        return;
    }

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
            qDebug() << "Ошибка загрузки картинки: " << photoPath;
        }
    }
    else
    {
        ui->profilePhoto->clear();
        qDebug() << "Фото отсутствует.";
    }
}




void Third::on_likeButton_clicked()
{
    if (currentIndex >= profilesData.size() || profilesData.isEmpty())
    {
        qDebug() <<"Ошибка: индекс за пределами массива или пустой массив.";
        return;
    }

    int userId = profilesData[currentIndex]["id"].toInt();
    saveReaction(userId, true);
    on_nextProfile(); // Переключаем на следующую анкету
}




void Third::on_dislikeButton_clicked()
{
    if (currentIndex >= profilesData.size() || profilesData.isEmpty())
    {
        qDebug() <<"Ошибка: индекс за пределами массива или пустой массив.";
        return;
    }

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
    query.clear();
    query.prepare("INSERT INTO likes (user_id, target_user_id, like_status) VALUES "
                  "((SELECT id FROM users WHERE login = :login), :target_user_id, :like_status)"
                  "ON CONFLICT (user_id, target_user_id) DO UPDATE SET like_status = :like_status");
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
