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
            if (ui->textEdit->hasFocus()) // Если фокус на textEdit
            {
                if (e->modifiers() & Qt::ShiftModifier)
                {
                    // Shift+Enter - вставляем перенос строки
                    QTextCursor cursor = ui->textEdit->textCursor();
                    e->accept();
                    return;
                }
                else
                {
                    QTextCursor cursor = ui->textEdit->textCursor();
                    QString text = ui->textEdit->toPlainText();
                    QStringList lines = text.split("\n");
                    QStringList updatedLines;

                    for (QString &line : lines)
                    {
                        while (line.length() > 30)
                        {
                            // Разбиваем строку на блоки по 20 символов
                            QString chunk = line.left(30);
                            updatedLines.append(chunk);
                            line = line.mid(30);
                        }

                        if (!line.isEmpty())
                        {
                            updatedLines.append(line);
                        }
                    }

                    // Обновляем текст перед отправкой
                    ui->textEdit->blockSignals(true);
                    ui->textEdit->setPlainText(updatedLines.join("\n"));
                    ui->textEdit->blockSignals(false);

                    // Отправляем сообщение
                    on_sendButton_clicked();
                    e->accept();
                    return;

                }
            }
            else
            {
                // Если фокус НЕ на textEdit, Enter всегда отправляет сообщение
                on_sendButton_clicked();
                e->accept();
                return;
            }
        }
        else
        {
            QDialog::keyReleaseEvent(e); // Обрабатываем остальные клавиши
        }



        if (ui->listWidget->count() > 0)
        {
            ui->listWidget->scrollToBottom();
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
