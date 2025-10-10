/********************************************************************************
** Form generated from reading UI file 'fourth.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FOURTH_H
#define UI_FOURTH_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Fourth
{
public:
    QGroupBox *groupBox;
    QPushButton *BackButton;
    QListWidget *listWidget;
    QPushButton *LikeButton;
    QPushButton *ChatButton;
    QLineEdit *searchInput;
    QPushButton *searchButton;

    void setupUi(QDialog *Fourth)
    {
        if (Fourth->objectName().isEmpty())
            Fourth->setObjectName("Fourth");
        Fourth->resize(400, 300);
        Fourth->setStyleSheet(QString::fromUtf8(""));
        groupBox = new QGroupBox(Fourth);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(0, 0, 401, 291));
        BackButton = new QPushButton(groupBox);
        BackButton->setObjectName("BackButton");
        BackButton->setGeometry(QRect(10, 260, 80, 24));
        BackButton->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        listWidget = new QListWidget(groupBox);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(10, 70, 331, 181));
        LikeButton = new QPushButton(groupBox);
        LikeButton->setObjectName("LikeButton");
        LikeButton->setGeometry(QRect(350, 110, 32, 32));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/images/like.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        LikeButton->setIcon(icon);
        ChatButton = new QPushButton(groupBox);
        ChatButton->setObjectName("ChatButton");
        ChatButton->setGeometry(QRect(350, 160, 32, 32));
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::MailMessageNew));
        ChatButton->setIcon(icon1);
        searchInput = new QLineEdit(groupBox);
        searchInput->setObjectName("searchInput");
        searchInput->setGeometry(QRect(10, 30, 331, 32));
        searchButton = new QPushButton(groupBox);
        searchButton->setObjectName("searchButton");
        searchButton->setGeometry(QRect(350, 30, 32, 32));
        QIcon icon2(QIcon::fromTheme(QIcon::ThemeIcon::EditFind));
        searchButton->setIcon(icon2);

        retranslateUi(Fourth);

        QMetaObject::connectSlotsByName(Fourth);
    } // setupUi

    void retranslateUi(QDialog *Fourth)
    {
        Fourth->setWindowTitle(QCoreApplication::translate("Fourth", "Dialog", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Fourth", "\320\257\321\211\320\270\320\272 \321\203\320\262\320\265\320\264\320\276\320\274\320\273\320\265\320\275\320\270\320\271", nullptr));
        BackButton->setText(QCoreApplication::translate("Fourth", "\320\235\320\260\320\267\320\260\320\264", nullptr));
#if QT_CONFIG(tooltip)
        LikeButton->setToolTip(QCoreApplication::translate("Fourth", "\320\237\320\276\321\201\321\202\320\260\320\262\320\270\321\202\321\214 \320\273\320\260\320\271\320\272", nullptr));
#endif // QT_CONFIG(tooltip)
        LikeButton->setText(QString());
#if QT_CONFIG(tooltip)
        ChatButton->setToolTip(QCoreApplication::translate("Fourth", "\320\237\320\265\321\200\320\265\320\271\321\202\320\270 \320\262 \321\207\320\260\321\202", nullptr));
#endif // QT_CONFIG(tooltip)
        ChatButton->setText(QString());
        searchInput->setPlaceholderText(QCoreApplication::translate("Fourth", "\320\237\320\276\320\270\321\201\320\272", nullptr));
        searchButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Fourth: public Ui_Fourth {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FOURTH_H
