#include "include/mainwindow.h"
#include "qdir.h"
#include "qevent.h"
#include "include/third.h"
#include "ui_mainwindow.h"
#include "include/second.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->resize(550, 350);


    networkManager = new QNetworkAccessManager(this);

    // Загрузка стилей
    QString sCssMain;
    QFile styleFile(":/styles/style.css");

    if (styleFile.open(QIODevice::ReadOnly))
    {
        sCssMain = styleFile.readAll();
        styleFile.close();
    }

    qApp->setStyleSheet(sCssMain);


    // Создаём валидатор с разрешёнными символами
    QRegularExpression regex("^[a-zA-Zа-яА-Я0-9@.,]*$"); // Только буквы, цифры, @, ., запятая
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, this);

    // Устанавливаем валидатор для полей ввода логина и пароля
    ui->login->setValidator(validator);
    ui->password->setValidator(validator);
    ui->login_2->setValidator(validator);
    ui->password_2->setValidator(validator);

    setWindowTitle("FlirtFinder");
}

MainWindow::~MainWindow()
{
    delete ui;
}



// Вход
void MainWindow::on_login_button_clicked()
{
    QString login = ui->login->text().trimmed();
    QString password = ui->password->text().trimmed();


    if (login.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "Fail", "Логин и пароль не могут быть пустыми!");
        return;
    }


    if (login.length() < 3 || login.length() > 20)
    {
        QMessageBox::warning(this, "Ошибка", "Логин должен содержать от 3 до 20 символов!");
        ui->login->clear();
        ui->password->clear();
        ui->login->setFocus();
        return;
    }

    if (password.length() < 6 || password.length() > 20)
    {
        QMessageBox::warning(this, "Ошибка", "Пароль должен содержать от 6 до 30 символов!");
        ui->login->clear();
        ui->password->clear();
        ui->login->setFocus();
        return;
    }

    QUrl url("http://localhost:5002/api/auth/login");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["Login"] = login;
    json["Password"] = password;

    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        // reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError)
        {
            QMessageBox::warning(this, "Ошибка", "Ошибка подключения к серверу!");
            qDebug() << "Ошибка сети:" << reply->errorString();
            return;
        }

        QByteArray responseData = reply->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
        QJsonObject responseObj = responseDoc.object();

        if (!responseObj.contains("token"))
        {
            QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль!");
            ui->login->clear();
            ui->password->clear();
            ui->login->setFocus();
            return;
        }

        QString token = responseObj["token"].toString();
        // QString token = QString::fromUtf8(responseData).trimmed();
        token_ = token;
        reply->deleteLater();

    QMessageBox::information(this, "Успех", "Авторизация прошла успешно!");

    qDebug() << "Token used mainWindow: " << token_;

    auto thirdWindow = new Third(token_);
    auto secondWindow = new Second(token_);


    secondWindow->setToken(token_);
    secondWindow->setUserCredentials(login, password);
    secondWindow->initializeUserData();

    thirdWindow->setCurrentUserData(login, password);
    thirdWindow->loadProfiles(login);
    thirdWindow->show();

    this->close();
    });
}





// Регистрация
void MainWindow::on_registration_button_clicked()
{
    QString login = ui->login_2->text();
    QString password = ui->password_2->text();


    qDebug() << "Login:" << login;
    qDebug() << "Password:" << password;


    if (login.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "Fail", "Логин и пароль не могут быть пустыми!");
        return;
    }


    if (login.length() < 3 || login.length() > 20)
    {
        QMessageBox::warning(this, "Ошибка", "Логин должен содержать от 3 до 20 символов!");
        ui->login_2->clear();
        ui->password_2->clear();
        ui->login_2->setFocus();
        return;
    }

    if (password.length() < 6 || password.length() > 20)
    {
        QMessageBox::warning(this, "Ошибка", "Пароль должен содержать от 6 до 30 символов!");
        ui->login_2->clear();
        ui->password_2->clear();
        ui->login_2->setFocus();
        return;
    }


    QJsonObject json;
    json["Login"] = login;
    json["Password"] = password;

    QNetworkRequest request(QUrl("http://localhost:5002/api/auth/register"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->post(request, QJsonDocument(json).toJson());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError)
        {
            QByteArray responseData = reply->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
            QJsonObject responseObj = responseDoc.object();
            QString token = responseObj["token"].toString();

            if (token.isEmpty())
            {
                QMessageBox::warning(this, "Ошибка", "Токен не получен!");
                return;
            }
            token_ = token;

            QMessageBox::information(this, "Успех", responseObj["message"].toString());

            auto secondWindow = new Second(token_);
            secondWindow->setToken(token_);
            secondWindow->setUserCredentials(login, password);
            secondWindow->initializeUserData2();
            secondWindow->show();
            this->close();
        }
        else
        {
            QByteArray errorData = reply->readAll();
            QJsonDocument errorDoc = QJsonDocument::fromJson(errorData);
            QString errorMessage = errorDoc.isObject() ? errorDoc.object().value("message").toString() : "Ошибка регистрации";

            QMessageBox::warning(this, "Ошибка", errorMessage);
            qDebug() << "Ошибка регистрации:" << reply->errorString();
        }

        reply->deleteLater();
        manager->deleteLater();
    });



    QMessageBox::information(this, "Успех", "Регистрация прошла успешно!");

    // Передача данных во вторую страницу
    auto secondWindow = new Second(token_);
    secondWindow->setToken(token_);
    secondWindow->setUserCredentials(login, password);
    secondWindow->initializeUserData2();
    secondWindow->show();
    this->close();
}





void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        if (ui->login->hasFocus() || ui->password->hasFocus())
        {
            ui->login_button->click();
        }
        else if (ui->login_2->hasFocus() || ui->password_2->hasFocus())
        {
            ui->registration_button->click();
        }
        event->accept();
    }
    else if (event->key() == Qt::Key_Down)
    {
        if (ui->login->hasFocus())
        {
            ui->password->setFocus();
        }
        else if (ui->login_2->hasFocus())
        {
            ui->password_2->setFocus();
        }
        event->accept();
    }
    else if (event->key() == Qt::Key_Up)
    {
        if (ui->password->hasFocus())
        {
            ui->login->setFocus();
        }
        else if (ui->password_2->hasFocus())
        {
           ui->login_2->setFocus();
        }
        event->accept();
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}
