#ifndef FIFTH_H
#define FIFTH_H

#include "qlistwidget.h"
#include <QDialog>

namespace Ui {
class Fifth;
}

class Fifth : public QDialog
{
    Q_OBJECT


public:
    explicit Fifth(QWidget *parent = nullptr);
    ~Fifth();

    void setUserCredentials(const QString &login, const QString &password, QListWidgetItem *selectedItem);

    void loadChatHistory(int senderId, int receiverId);


private slots:
    void on_sendButton_clicked();


private:
    Ui::Fifth *ui;

    QString currentLogin;
    QString currentPassword;


    int senderId;
    int receiverId;
};

#endif // FIFTH_H
