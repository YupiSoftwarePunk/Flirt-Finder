#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "second.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_login_button_clicked();

    void on_registration_button_clicked();

private:
    Ui::MainWindow *ui;
    // SecondWindow *sWindow;
};
#endif // MAINWINDOW_H
