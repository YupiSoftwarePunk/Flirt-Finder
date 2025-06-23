#ifndef FIFTH_H
#define FIFTH_H

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

    void setUserCredentials(const QString &login, const QString &password);

private:
    Ui::Fifth *ui;

    QString currentLogin;
    QString currentPassword;
};

#endif // FIFTH_H
