#ifndef SECOND_H
#define SECOND_H

#include <QDialog>
#include <QRegularExpressionValidator>

namespace Ui {
class Second;
}

class Second : public QDialog
{
    Q_OBJECT

public:
    explicit Second(const QString& token, QWidget *parent = nullptr);
    ~Second();

    void setUserCredentials(const QString &login, const QString &password);

    bool saveUserData(const QString &login, const QString &password,
                      const QString &name, const QString &gender, int age,
                      const QString &hobbies, const QString &city, const QString &photoPath);

    void loadUserData();
    void loadPhotoData(const QString &login);

    void initializeUserData();
    void initializeUserData2();
    void setToken(const QString& jwt);

private slots:
    void on_onLoadPhoto_clicked();
    void on_onSaveData_clicked();


private:
    Ui::Second *ui;

    QString m_photoPath;

    bool eventFilter(QObject *watched, QEvent *event) override;

    QString login;
    QString password;

    QString token;

};

#endif // SECOND_H
