#include "include/second.h"
#include "include/third.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qnetworkaccessmanager.h"
#include "qnetworkreply.h"
#include "ui_second.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QSqlQuery>
#include <QSqlError>
#include <QSpinBox>


Second::Second(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Second)
{
    ui->setupUi(this);

    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_4->installEventFilter(this);
    ui->textEdit->installEventFilter(this);

    setWindowTitle("FlirtFinder");
}

Second::~Second()
{
    delete ui;
}


// это для загрузки всех данных пользователя
void Second::initializeUserData()
{
    loadUserData();   // Загружаем данные анкеты
    loadPhotoData(login); // Загружаем картинку пользователя
}






// это для регистрации, когда фото еще нет
void Second::initializeUserData2()
{
    loadUserData();

    // QSqlQuery photoQuery;
    // photoQuery.prepare("SELECT photo_path FROM photos WHERE user_id = (SELECT id FROM users WHERE login = :login)");
    // photoQuery.bindValue(":login", login);

    // if (!photoQuery.exec())
    // {
    //     qDebug() << "Ошибка выполнения SQL при загрузке фото:" << photoQuery.lastError().text();
    //     return;
    // }

    // if (photoQuery.next())
    // {
    //     QString photoPath = photoQuery.value(0).toString();
    //     if (!photoPath.isEmpty())
    //     {
    //         loadPhotoData(photoPath);
    //     }
    //     else
    //     {
    //         qDebug() << "Фото отсутствует для пользователя.";
    //     }
    // }
    // else
    // {
    //     qDebug() << "Фото для пользователя не найдено.";
    // }


    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    QNetworkRequest userRequest(QUrl("http://localhost:5002/api/users/me"));
    userRequest.setRawHeader("Authorization", "Bearer " + token.toUtf8());

    QNetworkReply* userReply = manager->get(userRequest);

    connect(userReply, &QNetworkReply::finished, this, [=]() {
        if (userReply->error() == QNetworkReply::NoError)
        {
            QByteArray responseData = userReply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            QJsonObject userObj = jsonDoc.object();

            ui->lineEdit->setText(userObj["username"].toString());
            ui->textEdit->setText(userObj["bio"].toString());
            ui->comboBox->setCurrentText(userObj["gender"].toString());
            ui->lineEdit_4->setText(userObj["city"].toString());
            ui->spinBox->setValue(userObj["age"].toInt());

            QString login = userObj["login"].toString();
            QNetworkRequest photoRequest(QUrl("http://localhost:5002/api/users/" + login + "/photo"));
            QNetworkReply* photoReply = manager->get(photoRequest);

            connect(photoReply, &QNetworkReply::finished, this, [=]() {
                if (photoReply->error() == QNetworkReply::NoError)
                {
                    QByteArray photoData = photoReply->readAll();
                    QPixmap pixmap;
                    pixmap.loadFromData(photoData);
                    QGraphicsScene* scene = new QGraphicsScene(this);
                    scene->addPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatio));
                    ui->graphicsView->setScene(scene);
                }
                else
                {
                    qDebug() << "Фото не найдено:" << photoReply->errorString();
                }

                photoReply->deleteLater();
            });
        }
        else
        {
            qDebug() << "Ошибка загрузки данных пользователя:" << userReply->errorString();
        }

        userReply->deleteLater();
    });
}






void Second::setToken(const QString& jwt)
{
    token = jwt;
}






// кнопка загрузки изображения в анкету
void Second::on_onLoadPhoto_clicked()
{
    QString photoPath = QFileDialog::getOpenFileName(this,
                                                     tr("Выберите фото"),
                                                     QDir::homePath(),
                                                     tr("Изображения (*.png *.jpg *.jpeg *.bmp *.gif)"));

    if (!photoPath.isEmpty())
    {
        QPixmap pixmap(photoPath);

        m_photoPath = photoPath;

        if (pixmap.isNull())
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось загрузить изображение!");
            return;
        }

        QGraphicsScene *scene = new QGraphicsScene(this);
        scene->addPixmap(pixmap.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));
        ui->graphicsView->setScene(scene);

        QMessageBox::information(this, "Фото добавлено", "Фото успешно выбрано!");
    }
}




// кнопка сохранения в анкете
void Second::on_onSaveData_clicked()
{

    QString name = ui->lineEdit->text().trimmed();
    int age = ui->spinBox->value();
    QString hobbies = ui->textEdit->toPlainText().trimmed();
    QString city = ui->lineEdit_4->text().trimmed();
    QString sex = ui->comboBox->currentText();


    if (m_photoPath.isEmpty())
    {
        QSqlQuery query;
        query.prepare("SELECT photo_path FROM photos WHERE user_id = (SELECT id FROM users WHERE login = :login)");
        query.bindValue(":login", login);
        if (query.exec() && query.next())
        {
            m_photoPath = query.value(0).toString();
        }
    }


    if (name.isEmpty() || hobbies.isEmpty() || city.isEmpty() || m_photoPath.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены!");
        return;
    }


    QRegularExpression validText("^[a-zA-Zа-яА-ЯёЁ\\s-]+$");

    if (!validText.match(name).hasMatch() || name.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Имя содержит недопустимые символы");
        ui->lineEdit->clear();
        ui->lineEdit->setFocus();
        return;
    }

    if (!validText.match(city).hasMatch() || city.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Город содержит недопустимые символы");
        ui->lineEdit_4->clear();
        ui->lineEdit_4->setFocus();
        return;
    }



    // Проверка наличия города в базе данных
    QSqlQuery cityQuery;
    cityQuery.prepare("SELECT COUNT(*) FROM cities WHERE name = :city");
    cityQuery.bindValue(":city", city);

    if (!cityQuery.exec())
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось выполнить запрос к базе данных!");
        return;
    }

    cityQuery.next();
    int cityCount = cityQuery.value(0).toInt();
    if (cityCount == 0)
    {
        QMessageBox::warning(this, "Ошибка", "Город не найден в базе данных! Проверьте правильность ввода.");
        ui->lineEdit_4->setFocus();
        return;
    }


    if (saveUserData(login, password, name, sex, age, hobbies, city, m_photoPath))
    {
        loadPhotoData(login);
        QMessageBox::information(this, "Успех", "Данные успешно сохранены!");

        auto thirdWindow = new Third();
        thirdWindow->setCurrentUserData(login, password);
        thirdWindow->loadProfiles(login);
        thirdWindow->show();
        this->close();
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить данные!");
    }
}




// Загрузка пользовательских данных
void Second::setUserCredentials(const QString &login, const QString &password)
{
    this->login = login;
    this->password = password;
}



// сортировка ввода в поля анкеты
bool Second::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        QLineEdit *edit = qobject_cast<QLineEdit*>(watched);
        if (edit && (edit == ui->lineEdit || edit == ui->lineEdit_4))
        {
            if (keyEvent->key() == Qt::Key_Backspace || keyEvent->key() == Qt::Key_Delete)
            {
                return false;
            }

            if (!keyEvent->text().isEmpty() &&
                !QRegularExpression("[a-zA-Zа-яА-ЯёЁ\\s-]").match(keyEvent->text()).hasMatch())
            {
                return true;
            }
        }

        QTextEdit *textEditt = qobject_cast<QTextEdit *>(watched);
        if (textEditt && textEditt == ui->textEdit)
        {
            if (keyEvent->key() == Qt::Key_Backspace || keyEvent->key() == Qt::Key_Delete)
            {
                return false;
            }
        }
    }

    return QDialog::eventFilter(watched, event);
}





// сохранение данных в бд
bool Second::saveUserData(const QString &login, const QString &password,
                  const QString &name, const QString &gender, int age,
                  const QString &hobbies, const QString &city, const QString &photoPath)
{

    QSqlQuery query;
    query.prepare("UPDATE users SET name = :name, gender = :gender, age = :age, hobbies = :hobbies, city = :city "
                  "WHERE login = :login AND password = :password");
    query.bindValue(":name", name);
    query.bindValue(":gender", gender);
    query.bindValue(":age", age);
    query.bindValue(":hobbies", hobbies);
    query.bindValue(":city", city);
    query.bindValue(":login", login);
    query.bindValue(":password", password);

    if (!query.exec())
    {
        QMessageBox::warning(this, "Ошибка", "Ошибка сохранения данных пользователя!");
        qDebug() << "Ошибка SQL:" << query.lastError().text();
        return false;
    }


    QSqlQuery getUserIdQuery;
    getUserIdQuery.prepare("SELECT id FROM users WHERE login = :login");
    getUserIdQuery.bindValue(":login", login);

    if (!getUserIdQuery.exec() || !getUserIdQuery.next())
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить ID пользователя!");
        qDebug() << "Ошибка SQL:" << getUserIdQuery.lastError().text();
        return false;
    }

    int userId = getUserIdQuery.value(0).toInt();
    qDebug() << "User ID:" << userId;


    if (!photoPath.isEmpty())
    {
        QSqlQuery deletePhotoQuery;
        deletePhotoQuery.prepare("DELETE FROM photos WHERE user_id = :user_id");
        deletePhotoQuery.bindValue(":user_id", userId);

        if (!deletePhotoQuery.exec())
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось удалить старую картинку!");
            qDebug() << "Ошибка SQL:" << deletePhotoQuery.lastError().text();
            return false;
        }

        QSqlQuery photoQuery;
        photoQuery.prepare("INSERT INTO photos (user_id, photo_path) VALUES (:user_id, :photo_path)");
        photoQuery.bindValue(":user_id", userId);
        photoQuery.bindValue(":photo_path", photoPath);

        if (!photoQuery.exec())
        {
            QMessageBox::warning(this, "Ошибка", "Ошибка сохранения изображения!");
            qDebug() << "Ошибка SQL:" << photoQuery.lastError().text();
            return false;
        }
        qDebug() << "Новый путь к картинке успешно сохранён:" << photoPath;
    }
    return true;
}





// обновление данных пользователя
void Second::loadUserData()
{
    // QSqlQuery query;
    // query.prepare("SELECT name, gender, age, hobbies, city FROM users WHERE login = :login AND password = :password");
    // query.bindValue(":login", login);
    // query.bindValue(":password", password);

    // if (!query.exec())
    // {
    //     QMessageBox::warning(this, "Ошибка", "Ошибка загрузки данных!");
    //     qDebug() << "Ошибка SQL:" << query.lastError().text();
    //     return;
    // }

    // if (query.next())
    // {
    //     ui->lineEdit->setText(query.value("name").toString());
    //     ui->comboBox->setCurrentText(query.value("gender").toString());
    //     ui->spinBox->setValue(query.value("age").toInt());
    //     ui->textEdit->setPlainText(query.value("hobbies").toString());
    //     ui->lineEdit_4->setText(query.value("city").toString());
    // }



    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    QNetworkRequest request(QUrl("http://localhost:5002/api/users/me"));
    request.setRawHeader("Authorization", "Bearer " + token.toUtf8());

    QNetworkReply* reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError)
        {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            QJsonObject userObj = jsonDoc.object();

            ui->lineEdit->setText(userObj["username"].toString());
            ui->comboBox->setCurrentText(userObj["gender"].toString());
            ui->spinBox->setValue(userObj["age"].toInt());
            ui->textEdit->setPlainText(userObj["bio"].toString());
            ui->lineEdit_4->setText(userObj["city"].toString());
        }
        else
        {
            QMessageBox::warning(this, "Ошибка", "Ошибка загрузки данных!");
            qDebug() << "Ошибка API:" << reply->errorString();
        }

        reply->deleteLater();
        manager->deleteLater();
    });
}




// обновление картинки пользователя
void Second::loadPhotoData(const QString &login)
{
    // QSqlQuery photoQuery;
    // photoQuery.prepare("SELECT photo_path FROM photos WHERE user_id = (SELECT id FROM users WHERE login = :login)");
    // photoQuery.bindValue(":login", login);

    // if (!photoQuery.exec() || !photoQuery.next())
    // {
    //     QMessageBox::warning(this, "Ошибка", "Не удалось загрузить изображение!");
    //     qDebug() << "Ошибка SQL:" << photoQuery.lastError().text();
    //     return;
    // }

    // QString photoPath = photoQuery.value(0).toString();
    // qDebug() << "Загруженный путь к картинке:" << photoPath;

    // if (!photoPath.isEmpty())
    // {
    //     QPixmap pixmap(photoPath);
    //     if (!pixmap.isNull())
    //     {
    //         QGraphicsScene *scene = new QGraphicsScene(this);
    //         scene->addPixmap(pixmap.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));
    //         ui->graphicsView->setScene(scene);
    //     } else
    //     {
    //         QMessageBox::warning(this, "Ошибка", "Не удалось загрузить изображение в интерфейс!");
    //     }
    // }



    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("http://localhost:5002/api/users/" + login + "/photo"));

    QNetworkReply* reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError)
        {
            QByteArray photoData = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(photoData);

            if (!pixmap.isNull())
            {
                QGraphicsScene* scene = new QGraphicsScene(this);
                scene->addPixmap(pixmap.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));
                ui->graphicsView->setScene(scene);
            }
            else
            {
                QMessageBox::warning(this, "Ошибка", "Не удалось загрузить изображение в интерфейс!");
            }
        }
        else
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось загрузить изображение!");
            qDebug() << "Ошибка API:" << reply->errorString();
        }

        reply->deleteLater();
        manager->deleteLater();
    });
}
