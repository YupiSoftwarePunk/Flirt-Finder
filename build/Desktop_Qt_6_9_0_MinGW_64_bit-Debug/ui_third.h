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
#include <QtGui/QIcon>
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
    QLabel *profileAge;
    QLabel *profileName;
    QLabel *profileCity;
    QPushButton *likeButton;
    QPushButton *dislikeButton;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QWidget *page_2;
    QTextEdit *profileHobbies;
    QPushButton *settingsButton;
    QPushButton *settingsButton_2;
    QLabel *profilePhoto;

    void setupUi(QDialog *Third)
    {
        if (Third->objectName().isEmpty())
            Third->setObjectName("Third");
        Third->resize(400, 300);
        Third->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        profileAge = new QLabel(Third);
        profileAge->setObjectName("profileAge");
        profileAge->setGeometry(QRect(10, 60, 121, 16));
        profileName = new QLabel(Third);
        profileName->setObjectName("profileName");
        profileName->setGeometry(QRect(10, 20, 131, 16));
        profileCity = new QLabel(Third);
        profileCity->setObjectName("profileCity");
        profileCity->setGeometry(QRect(10, 200, 271, 20));
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
        settingsButton = new QPushButton(page_2);
        settingsButton->setObjectName("settingsButton");
        settingsButton->setGeometry(QRect(370, 20, 32, 32));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/images/settings.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        settingsButton->setIcon(icon);
        settingsButton->setIconSize(QSize(32, 32));
        settingsButton_2 = new QPushButton(page_2);
        settingsButton_2->setObjectName("settingsButton_2");
        settingsButton_2->setGeometry(QRect(370, 70, 32, 32));
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::MailForward));
        settingsButton_2->setIcon(icon1);
        settingsButton_2->setIconSize(QSize(32, 32));
        profilePhoto = new QLabel(page_2);
        profilePhoto->setObjectName("profilePhoto");
        profilePhoto->setGeometry(QRect(140, 10, 221, 181));
        stackedWidget->addWidget(page_2);
        stackedWidget->raise();
        profileAge->raise();
        profileName->raise();
        profileCity->raise();
        likeButton->raise();
        dislikeButton->raise();

        retranslateUi(Third);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Third);
    } // setupUi

    void retranslateUi(QDialog *Third)
    {
        Third->setWindowTitle(QCoreApplication::translate("Third", "Dialog", nullptr));
        profileAge->setText(QCoreApplication::translate("Third", "TextLabel", nullptr));
        profileName->setText(QCoreApplication::translate("Third", "TextLabel", nullptr));
        profileCity->setText(QCoreApplication::translate("Third", "TextLabel", nullptr));
        likeButton->setText(QCoreApplication::translate("Third", "Like", nullptr));
        dislikeButton->setText(QCoreApplication::translate("Third", "Dislike", nullptr));
#if QT_CONFIG(tooltip)
        settingsButton->setToolTip(QCoreApplication::translate("Third", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", nullptr));
#endif // QT_CONFIG(tooltip)
        settingsButton->setText(QString());
#if QT_CONFIG(tooltip)
        settingsButton_2->setToolTip(QCoreApplication::translate("Third", "\320\257\321\211\320\270\320\272 \321\201\320\276\320\276\320\261\321\211\320\265\320\275\320\270\320\271", nullptr));
#endif // QT_CONFIG(tooltip)
        settingsButton_2->setText(QString());
        profilePhoto->setText(QCoreApplication::translate("Third", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Third: public Ui_Third {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_THIRD_H
