#ifndef FOURTH_H
#define FOURTH_H

#include <QDialog>

namespace Ui {
class Fourth;
}

class Fourth : public QDialog
{
    Q_OBJECT

public:
    explicit Fourth(QWidget *parent = nullptr);
    ~Fourth();

private:
    Ui::Fourth *ui;
};

#endif // FOURTH_H
