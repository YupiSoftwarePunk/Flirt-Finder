#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>

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

    void keyPressEvent(QKeyEvent *event);


private:
    Ui::MainWindow *ui;

    QNetworkAccessManager* networkManager;
};

#endif // MAINWINDOW_H
