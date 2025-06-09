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

private:
    Ui::Third *ui;
};

#endif // THIRD_H
