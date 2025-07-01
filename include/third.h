#ifndef THIRD_H
#define THIRD_H

#include <QDialog>

namespace Ui {
class Third;
}

class Third : public QDialog
{
    Q_OBJECT

public:
    explicit Third(QWidget *parent = nullptr);
    ~Third();

    void loadProfiles(const QString &currentLogin);

    void setCurrentUserData(const QString &login, const QString &password);

    void setProfileData(const QString &name, int age, const QString &city, const QString &photoPath, const QString & hobby);

    void hideAllButtons();

private slots:
    void on_likeButton_clicked();
    void on_dislikeButton_clicked();

    void on_nextProfile();
    void on_prevProfile();

    void keyPressEvent(QKeyEvent *event);

    void on_settingsButton_clicked();

    void on_messageBox_clicked();

private:
    Ui::Third *ui;

    QString currentLogin;
    QString currentPassword;
    int currentIndex = 0; // Индекс текущей анкеты

    QVector<QMap<QString, QString>> profilesData;

    QSet<int> viewedProfiles;

    void updateUI();

    void saveReaction(int targetUserId, bool isLike);

    void sortProfiles();
};

#endif // THIRD_H
