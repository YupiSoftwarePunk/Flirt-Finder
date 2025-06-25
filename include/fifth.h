#ifndef FIFTH_H
#define FIFTH_H

#include "qevent.h"
#include "qlistwidget.h"
#include "ui_fifth.h"
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

    void setUserCredentials(const QString &login, const QString &password, QListWidgetItem *selectedItem);

    void loadChatHistory(int senderId, int receiverId);



protected:
    void keyReleaseEvent(QKeyEvent* e)
    {
        if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
        {
            QTextCursor cursor = ui->textEdit->textCursor();
            QString text = ui->textEdit->toPlainText();

            if (text.endsWith('\n'))
            {
                cursor.movePosition(QTextCursor::End);
                cursor.deletePreviousChar();
                ui->textEdit->setTextCursor(cursor);
            }

            on_sendButton_clicked();

            if (ui->listWidget->count() > 0)
            {
                ui->listWidget->scrollToBottom();
            }
        }
    }


private slots:
    void on_sendButton_clicked();


private:
    Ui::Fifth *ui;

    QString currentLogin;
    QString currentPassword;


    int senderId;
    int receiverId;
};

#endif // FIFTH_H
