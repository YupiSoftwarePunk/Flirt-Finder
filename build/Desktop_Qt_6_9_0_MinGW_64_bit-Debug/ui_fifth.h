/********************************************************************************
** Form generated from reading UI file 'fifth.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIFTH_H
#define UI_FIFTH_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_Fifth
{
public:
    QListWidget *listWidget;
    QPushButton *sendButton;
    QTextEdit *textEdit;

    void setupUi(QDialog *Fifth)
    {
        if (Fifth->objectName().isEmpty())
            Fifth->setObjectName("Fifth");
        Fifth->resize(400, 300);
        Fifth->setStyleSheet(QString::fromUtf8(""));
        listWidget = new QListWidget(Fifth);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(10, 0, 381, 241));
        listWidget->setProperty("isWrapping", QVariant(false));
        listWidget->setWordWrap(true);
        sendButton = new QPushButton(Fifth);
        sendButton->setObjectName("sendButton");
        sendButton->setGeometry(QRect(350, 250, 41, 41));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSend));
        sendButton->setIcon(icon);
        textEdit = new QTextEdit(Fifth);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(10, 250, 331, 41));
        textEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
        textEdit->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);

        retranslateUi(Fifth);

        QMetaObject::connectSlotsByName(Fifth);
    } // setupUi

    void retranslateUi(QDialog *Fifth)
    {
        Fifth->setWindowTitle(QCoreApplication::translate("Fifth", "Dialog", nullptr));
        sendButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Fifth: public Ui_Fifth {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIFTH_H
