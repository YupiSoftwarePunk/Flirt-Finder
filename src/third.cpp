#include "include/third.h"
#include "qevent.h"
#include "include/second.h"
#include "ui_third.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPixmap>
#include <QMessageBox>
#include <QThread>


Third::Third(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Third)
{
    ui->setupUi(this);

    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();
}

Third::~Third()
{
    delete ui;
}

/*
 необходимо сделать на этой странице фильтры для поиска и выдачи анкет

пусть сначала выпадают анкеты из одного города и с общими увлечениями
и люди одного возраста что и залогининый пользователь
потом все остальные
*/


void Third::loadProfiles(const QString &login)
{
    profilesData.clear();
    qDebug() << "Текущий логин для загрузки профилей:" << login;

    // Получаем пол текущего пользователя
    QSqlQuery userQuery;
    userQuery.prepare("SELECT gender FROM users WHERE login = :login");
    userQuery.bindValue(":login", login);

    if (!userQuery.exec() || !userQuery.next())
    {
        qDebug() << "Ошибка выполнения SQL-запроса для текущего пользователя:" << userQuery.lastError().text();
        return;
    }

    QString currentGender = userQuery.value("gender").toString();
    QString oppositeGender = (currentGender == "Мужской") ? "Женский" : "Мужской";

    // Выполняем основной запрос для получения профилей противоположного пола
    QSqlQuery query;
    query.prepare(
        "SELECT DISTINCT users.id, users.name, users.age, users.city, users.hobbies, users.gender, photos.photo_path "
        "FROM users LEFT JOIN photos ON users.id = photos.user_id "
        "WHERE users.login != :login AND users.gender = :gender"
        );
    query.bindValue(":login", login);
    query.bindValue(":gender", oppositeGender); // Применяем фильтр по противоположному полу

    if (!query.exec())
    {
        qDebug() << "Ошибка выполнения SQL-запроса:" << query.lastError().text();
        return;
    }

    QSet<QString> uniqueIds; // Храним уникальные ID пользователей
    while (query.next())
    {
        QString userId = query.value("id").toString();
        if (uniqueIds.contains(userId))
        {
            continue; // Пропускаем дубликаты
        }
        uniqueIds.insert(userId);

        QMap<QString, QString> profile;
        profile["id"] = userId;
        profile["name"] = query.value("name").toString();
        profile["age"] = query.value("age").toString();
        profile["city"] = query.value("city").toString();
        profile["hobbies"] = query.value("hobbies").toString();
        profile["gender"] = query.value("gender").toString();
        profile["photo"] = query.value("photo_path").toString();
        profilesData.append(profile);
    }

    qDebug() << "Загружено анкет:" << profilesData.size();

    if (!profilesData.isEmpty())
    {
        currentIndex = 0;
        sortProfiles(); // Сортировка профилей после загрузки
        updateUI();     // Отображаем первую анкету
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

    qDebug() << "Отображение анкеты с индексом:" << currentIndex << "ID пользователя:" << profile["id"];

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
    int userId = profilesData[currentIndex]["id"].toInt();
    saveReaction(userId, true);

    currentIndex++;

    if (currentIndex < profilesData.size())
    {
        updateUI();
    }
    else
    {
        QMessageBox::information(this, "Информация", "Это была последняя анкета!");
        qDebug() << "Последняя анкета обработана.";
        // currentIndex--;
        // this->close();
        // return;
    }
}




void Third::on_dislikeButton_clicked()
{
    if (profilesData.isEmpty() || currentIndex >= profilesData.size())
    {
        QMessageBox::information(this, "Информация", "Это была последняя анкета!");
        qDebug() << "Ошибка: индекс за пределами массива или массив пуст.";
        return;
    }

    int userId = profilesData[currentIndex]["id"].toInt();
    saveReaction(userId, false);

    currentIndex++;
    if (currentIndex < profilesData.size())
    {
        updateUI();
    }
    else
    {
        QMessageBox::information(this, "Информация", "Это была последняя анкета!");
        qDebug() << "Последняя анкета обработана.";
        // currentIndex--;
        // this->close();
        // return;
    }
}




void Third::on_nextProfile()
{
    if (currentIndex < profilesData.size()-1)
    {
        currentIndex++;
        updateUI();
    }
    else if (currentIndex == profilesData.size()-1)
    {
        QMessageBox::information(this, "Конец", "Это последняя анкета.");
        qDebug() << "Вы достигли конца анкет.";
    }
    else
    {
        qDebug() << "Нет действий, анкеты закончились.";
        // this->close();
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
        QMessageBox::information(this, "Начало", "Это первая анкета.");
        qDebug() << "Вы достигли начала анкет.";
    }
}



void Third::saveReaction(int targetUserId, bool isLike)
{
    if (currentLogin.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Логин текущего пользователя пуст!");
        qDebug() << "Ошибка: currentLogin пуст.";
        return;
    }


    QSqlQuery getCurrentUserIdQuery;
    getCurrentUserIdQuery.prepare("SELECT id FROM users WHERE login = :login");
    getCurrentUserIdQuery.bindValue(":login", currentLogin);

    if (!getCurrentUserIdQuery.exec() || !getCurrentUserIdQuery.next())
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить ID текущего пользователя!");
        qDebug() << "Ошибка выполнения SQL:" << getCurrentUserIdQuery.lastError().text();
        return;
    }

    int currentUserId = getCurrentUserIdQuery.value(0).toInt();


    // Сохраняем реакцию
    QSqlQuery query;

    query.prepare("INSERT INTO likes_dislikes (user_id, liked_by, reaction) "
                  "VALUES (:user_id, :liked_by_id, :reaction) "
                  "ON CONFLICT (user_id, liked_by) DO UPDATE SET reaction = :reaction");

    query.bindValue(":user_id", targetUserId);
    query.bindValue(":liked_by_id", currentUserId);
    query.bindValue(":reaction", isLike ? 1 : -1);   // 1 = лайк, -1 = дизлайк

    if (!query.exec())
    {
        QMessageBox::warning(this, "Ошибка", "Ошибка сохранения лайка/дизлайка!");
        qDebug() << "Ошибка выполнения SQL:" << query.lastError().text();
    }
    else
    {
        qDebug() << "Реакция успешно сохранена: " << (isLike ? "Лайк" : "Дизлайк");
    }
}



void Third::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right)
    {
        on_nextProfile();
        event->accept();
    }
    else if (event->key() == Qt::Key_Left)
    {
        on_prevProfile();
        event->accept();
    }
    else
    {
        QDialog::keyPressEvent(event);
    }
}




void Third::setCurrentUserData(const QString &login, const QString &password)
{
    currentLogin = login;
    currentPassword = password;
}



void Third::on_settingsButton_clicked()
{
    auto secondWindow = new Second();
    secondWindow->setUserCredentials(currentLogin, currentPassword);
    secondWindow->initializeUserData();
    secondWindow->show();
    this->close();
}



void Third::sortProfiles()
{
    // Получаем данные текущего пользователя
    QSqlQuery userQuery;
    userQuery.prepare("SELECT age, city, hobbies FROM users WHERE login = :login");
    userQuery.bindValue(":login", currentLogin);

    if (!userQuery.exec() || !userQuery.next())
    {
        qDebug() << "Ошибка выполнения SQL-запроса для текущего пользователя:" << userQuery.lastError().text();
        return;
    }

    int currentAge = userQuery.value("age").toInt();
    QString currentCity = userQuery.value("city").toString();
    QString currentHobbies = userQuery.value("hobbies").toString();

    // Сортировка профилей
    std::sort(profilesData.begin(), profilesData.end(), [&](const QMap<QString, QString> &a, const QMap<QString, QString> &b) {
        bool isSameAgeA = a["age"].toInt() == currentAge;
        bool isSameAgeB = b["age"].toInt() == currentAge;

        bool isSameCityA = a["city"] == currentCity;
        bool isSameCityB = b["city"] == currentCity;

        bool isSimilarHobbiesA = a["hobbies"].contains(currentHobbies, Qt::CaseInsensitive);
        bool isSimilarHobbiesB = b["hobbies"].contains(currentHobbies, Qt::CaseInsensitive);

        // 1. Приоритет по возрасту
        if (isSameAgeA != isSameAgeB)
        {
            return isSameAgeA > isSameAgeB;
        }
        // 2. Приоритет по городу
        if (isSameCityA != isSameCityB)
        {
            return isSameCityA > isSameCityB;
        }
        // 3. Приоритет по увлечениям
        if (isSimilarHobbiesA != isSimilarHobbiesB)
        {
            return isSimilarHobbiesA > isSimilarHobbiesB;
        }

        // В конце сортировка по увеличению возраста
        return a["age"].toInt() < b["age"].toInt();
    });

    qDebug() << "Профили отсортированы.";
}
