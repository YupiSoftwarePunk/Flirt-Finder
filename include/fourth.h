#ifndef FOURTH_H
#define FOURTH_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>

namespace Ui {
class Fourth;
}

class Fourth : public QDialog
{
    Q_OBJECT

public:
    explicit Fourth(QWidget *parent = nullptr);
    ~Fourth();

    void loadNotifications();


    void setUserCredentials(const QString &login, const QString &password);


private slots:
    void onChatButtonClicked();   // int userId
    void onBackButtonClicked();
    void on_LikeButton_clicked();   // int userId

    void on_BackButton_clicked();

private:
    Ui::Fourth *ui;

    QListWidget *notificationList;
    QPushButton *backButton;

    QString currentUser;

    QString currentLogin;
    QString currentPassword;


    int getCurrentUserId(const QString &login);
};

#endif // FOURTH_H
