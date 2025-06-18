#include "fourth.h"
#include "ui_fourth.h"

Fourth::Fourth(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Fourth)
{
    ui->setupUi(this);
}

Fourth::~Fourth()
{
    delete ui;
}
