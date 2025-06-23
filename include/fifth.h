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

private:
    Ui::Fifth *ui;
};

#endif // FIFTH_H
