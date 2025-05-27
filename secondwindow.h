#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QDialog>
#include <QPixmap>

namespace Ui {
class SecondWindow;
}

class SecondWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = nullptr);
    ~SecondWindow();

private slots:
    void onSaveData();
    void onLoadPhoto();

private:
    Ui::SecondWindow *ui;
    QPixmap userPhoto;
    QString photoPath;
};

#endif // SECONDWINDOW_H
