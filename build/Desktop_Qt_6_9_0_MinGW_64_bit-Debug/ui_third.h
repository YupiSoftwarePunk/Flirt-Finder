/********************************************************************************
** Form generated from reading UI file 'third.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_THIRD_H
#define UI_THIRD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Third
{
public:
    QLabel *profilePhoto;
    QLabel *profileAge;
    QLabel *profileName;
    QLabel *profileCity;
    QPushButton *likeButton;
    QPushButton *dislikeButton;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QWidget *page_2;
    QTextEdit *profileHobbies;

    void setupUi(QDialog *Third)
    {
        if (Third->objectName().isEmpty())
            Third->setObjectName("Third");
        Third->resize(400, 300);
        Third->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        profilePhoto = new QLabel(Third);
        profilePhoto->setObjectName("profilePhoto");
        profilePhoto->setGeometry(QRect(150, 10, 231, 211));
        profileAge = new QLabel(Third);
        profileAge->setObjectName("profileAge");
        profileAge->setGeometry(QRect(10, 60, 121, 16));
        profileName = new QLabel(Third);
        profileName->setObjectName("profileName");
        profileName->setGeometry(QRect(10, 20, 131, 16));
        profileCity = new QLabel(Third);
        profileCity->setObjectName("profileCity");
        profileCity->setGeometry(QRect(10, 200, 111, 20));
        likeButton = new QPushButton(Third);
        likeButton->setObjectName("likeButton");
        likeButton->setGeometry(QRect(80, 250, 80, 24));
        dislikeButton = new QPushButton(Third);
        dislikeButton->setObjectName("dislikeButton");
        dislikeButton->setGeometry(QRect(200, 250, 80, 24));
        stackedWidget = new QStackedWidget(Third);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(-1, -1, 401, 301));
        page = new QWidget();
        page->setObjectName("page");
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        profileHobbies = new QTextEdit(page_2);
        profileHobbies->setObjectName("profileHobbies");
        profileHobbies->setGeometry(QRect(10, 100, 121, 91));
        profileHobbies->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        stackedWidget->addWidget(page_2);
        stackedWidget->raise();
        profilePhoto->raise();
        profileAge->raise();
        profileName->raise();
        profileCity->raise();
        likeButton->raise();
        dislikeButton->raise();

        retranslateUi(Third);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Third);
    } // setupUi

    void retranslateUi(QDialog *Third)
    {
        Third->setWindowTitle(QCoreApplication::translate("Third", "Dialog", nullptr));
        profilePhoto->setText(QCoreApplication::translate("Third", "TextLabel", nullptr));
        profileAge->setText(QCoreApplication::translate("Third", "TextLabel", nullptr));
        profileName->setText(QCoreApplication::translate("Third", "TextLabel", nullptr));
        profileCity->setText(QCoreApplication::translate("Third", "TextLabel", nullptr));
        likeButton->setText(QCoreApplication::translate("Third", "Like", nullptr));
        dislikeButton->setText(QCoreApplication::translate("Third", "Dislike", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Third: public Ui_Third {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_THIRD_H
