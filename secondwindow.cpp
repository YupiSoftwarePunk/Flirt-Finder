#include "secondwindow.h"
#include <QLabel>
#include <QVBoxLayout>

SecondWindow::SecondWindow(QWidget *parent)
    : QWidget(parent)
{
    // Простой интерфейс второго окна
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("Добро пожаловать во второе окно!", this);
    layout->addWidget(label);

    setLayout(layout);
}

SecondWindow::~SecondWindow()
{

}
