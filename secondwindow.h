#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QWidget>

class SecondWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = nullptr);

    ~SecondWindow();
};

#endif // SECONDWINDOW_H
