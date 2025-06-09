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


private slots:
    void on_likeButton_clicked();
    void on_dislikeButton_clicked();

private:
    Ui::Third *ui;

    QString currentLogin;
    int currentIndex = 0; // Индекс текущей анкеты
    QVector<QMap<QString, QString>> profilesData;
    void updateUI();
    void updateLikeStatus(int targetUserId, bool isLike);
};

#endif // THIRD_H
