#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QDialog>

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
    void onSaveData(); // для сохранения данных
    void onLoadPhoto();

private:
    Ui::SecondWindow *ui;
};

#endif // SECONDWINDOW_H
