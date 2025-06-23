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
    void on_ChatButton_clicked();
    void onBackButtonClicked();
    void on_LikeButton_clicked();

private:
    Ui::Fourth *ui;

    QListWidget *notificationList;
    QPushButton *backButton;

    QString currentUser;

    QString currentLogin;
    QString currentPassword;


    int getCurrentUserId(const QString &login);

    void checkMutualLike();
};

#endif // FOURTH_H
