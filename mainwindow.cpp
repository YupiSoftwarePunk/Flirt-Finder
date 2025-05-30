#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "second.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QMap<QString, QString> userDatabase;


// Enter
void MainWindow::on_pushButton_clicked()
{
    QString login = ui->login->text();
    QString password = ui->password->text();

    if (userDatabase.contains(login) && userDatabase[login] == password)
    {
        QMessageBox::information(this, "Success", "Авторизация прошла успешно!");

        auto secondWindow = new Second();
        secondWindow->show();

        this->close();
    }
    else
    {
        QMessageBox::warning(this, "Fail", "Ошибка авторизации! Неверный логин или пароль!");
    }
}


// Registration
void MainWindow::on_pushButton_2_clicked()
{
    QString login = ui->login->text();
    QString password = ui->password->text();

    if (userDatabase.contains(login))
    {
        QMessageBox::warning(this, "Fail", "Аккаунт с таким логином уже существует!");
    }
    else
    {
        userDatabase.insert(login, password);
        QMessageBox::information(this, "Success", "Регистрация прошла успешно!");

        auto secondWindow = new Second();
        secondWindow->show();

        this->close();
    }
}
