#include "include/mainwindow.h"
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


    // Создаём валидатор с разрешёнными символами
    QRegularExpression regex("^[a-zA-Z0-9@.,]*$"); // Только буквы, цифры, @, ., запятая
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, this);

    // Устанавливаем валидатор для полей ввода логина и пароля
    ui->login->setValidator(validator);
    ui->password->setValidator(validator);
    ui->login_2->setValidator(validator);
    ui->password_2->setValidator(validator);
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


    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Fail", "Логин и пароль не могут быть пустыми!");
        return;
    }


    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE login = :login AND password = :password");
    query.bindValue(":login", login);
    query.bindValue(":password", password);

    if (!query.exec())
    {
        QMessageBox::warning(this, "Ошибка", "Ошибка проверки авторизации!");
        qDebug() << "Ошибка SQL:" << query.lastError().text();
        return;
    }

    query.next();
    int count = query.value(0).toInt();
    if (count == 0)
    {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль!");
        ui->login->clear();
        ui->password->clear();
        ui->login->setFocus();
        return;
    }

    QMessageBox::information(this, "Успех", "Авторизация прошла успешно!");

    // Переход на второе окно
    // auto secondWindow = new Second();
    // secondWindow->setUserCredentials(login, password);
    // secondWindow->initializeUserData(); // Загрузить данные из базы
    // secondWindow->show();
    // this->close();


    // переход на третье окно
    auto thirdWindow = new Third();
    auto secondWindow = new Second();
    secondWindow->setUserCredentials(login, password);
    secondWindow->initializeUserData();
    thirdWindow->setCurrentUserData(login, password);
    thirdWindow->loadProfiles(login);
    thirdWindow->show();
    this->close();
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

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE login = :login");
    query.bindValue(":login", login);

    if (!query.exec())
    {
        QMessageBox::warning(this, "Ошибка", "Ошибка проверки логина в базе данных!");
        qDebug() << "Ошибка SQL:" << query.lastError().text();
        return;
    }

    query.next();
    int count = query.value(0).toInt();
    if (count > 0)
    {
        QMessageBox::warning(this, "Ошибка", "Аккаунт с таким логином уже существует!");
        ui->login_2->clear();
        ui->password_2->clear();
        ui->login_2->setFocus();
        return;
    }


    // Регистрация нового пользователя
    query.clear();
    query.prepare("INSERT INTO users (login, password, name, gender, age, hobbies, city) "
                  "VALUES (:login, :password, :name, :gender, :age, :hobbies, :city)");
    query.bindValue(":login", login);
    query.bindValue(":password", password);
    query.bindValue(":name", "");
    query.bindValue(":gender", "");
    query.bindValue(":age", 0);
    query.bindValue(":hobbies", "");
    query.bindValue(":city", "");

    if (!query.exec())
    {
        QMessageBox::warning(this, "Ошибка", "Ошибка регистрации!");
        qDebug() << "Ошибка SQL:" << query.lastError().text();
        return;
    }


    QMessageBox::information(this, "Успех", "Регистрация прошла успешно!");

    // Передача данных во вторую страницу
    auto secondWindow = new Second();
    secondWindow->setUserCredentials(login, password);
    secondWindow->initializeUserData2();     // Загружаем данные перед отображением
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
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}
