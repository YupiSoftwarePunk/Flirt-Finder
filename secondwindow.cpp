#include "secondwindow.h"
#include <QLabel>
#include <QVBoxLayout>

SecondWindow::SecondWindow(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("Добро пожаловать во второе окно!", this);
    layout->addWidget(label);

    setLayout(layout);
}

SecondWindow::~SecondWindow()
{
    delete ui;
}
