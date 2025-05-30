#ifndef SECOND_H
#define SECOND_H

#include <QDialog>

namespace Ui {
class Second;
}

class Second : public QDialog
{
    Q_OBJECT

public:
    explicit Second(QWidget *parent = nullptr);
    ~Second();

private slots:
    void on_onLoadPhoto_clicked();

    void on_onSaveData_clicked();

private:
    Ui::Second *ui;
    QString m_photoPath;
};

#endif // SECOND_H
