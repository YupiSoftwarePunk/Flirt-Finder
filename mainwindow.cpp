#include "mainwindow.h"
#include "./ui_mainwindow.h"

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

void MainWindow::on_pushButton_clicked()
{
    QString login = ui->login->text();
    QString password = ui->password->text();

    if  (login == "user" && password == "1234")
    {
        QMessageBox::information(this, "Success", "Авторизация прошла успешно!");
    }
    else
    {
        QMessageBox::warning(this, "Fail", "Ошибка авторизации! Ошибка в логине или пароле!");
    }
}

