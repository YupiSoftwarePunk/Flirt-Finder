#include "include/fifth.h"
#include "ui_fifth.h"

Fifth::Fifth(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Fifth)
{
    ui->setupUi(this);
}

Fifth::~Fifth()
{
    delete ui;
}




void Fifth::setUserCredentials(const QString &login, const QString &password)
{
    currentLogin = login;
    currentPassword = password;
}
