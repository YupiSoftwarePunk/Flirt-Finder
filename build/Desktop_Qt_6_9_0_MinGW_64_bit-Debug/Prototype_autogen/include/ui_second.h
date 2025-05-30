/********************************************************************************
** Form generated from reading UI file 'second.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SECOND_H
#define UI_SECOND_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Second
{
public:
    QGroupBox *groupBox;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QPushButton *onSaveData;
    QLineEdit *lineEdit_4;
    QPushButton *onLoadPhoto;

    void setupUi(QDialog *Second)
    {
        if (Second->objectName().isEmpty())
            Second->setObjectName("Second");
        Second->resize(400, 300);
        groupBox = new QGroupBox(Second);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(10, 0, 381, 291));
        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(10, 30, 113, 24));
        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(10, 70, 113, 24));
        lineEdit_3 = new QLineEdit(groupBox);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(10, 120, 113, 24));
        onSaveData = new QPushButton(groupBox);
        onSaveData->setObjectName("onSaveData");
        onSaveData->setGeometry(QRect(150, 230, 80, 24));
        lineEdit_4 = new QLineEdit(groupBox);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(10, 170, 113, 24));
        onLoadPhoto = new QPushButton(groupBox);
        onLoadPhoto->setObjectName("onLoadPhoto");
        onLoadPhoto->setGeometry(QRect(260, 60, 91, 24));

        retranslateUi(Second);

        QMetaObject::connectSlotsByName(Second);
    } // setupUi

    void retranslateUi(QDialog *Second)
    {
        Second->setWindowTitle(QCoreApplication::translate("Second", "Dialog", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Second", "\320\241\320\276\320\267\320\264\320\260\320\275\320\270\320\265 \320\270 \320\267\320\260\320\277\320\276\320\273\320\275\320\265\320\275\320\270\320\265 \320\260\320\275\320\272\320\265\321\202\321\213", nullptr));
        lineEdit->setText(QCoreApplication::translate("Second", "\320\230\320\274\321\217, \320\277\320\276\320\273", nullptr));
        lineEdit_2->setText(QCoreApplication::translate("Second", "\320\222\320\276\320\267\321\200\320\260\321\201\321\202", nullptr));
        lineEdit_3->setText(QCoreApplication::translate("Second", "\320\243\320\262\320\273\320\265\321\207\320\265\320\275\320\270\321\217", nullptr));
        onSaveData->setText(QCoreApplication::translate("Second", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        lineEdit_4->setText(QCoreApplication::translate("Second", "\320\223\320\276\321\200\320\276\320\264 \320\277\321\200\320\276\320\266\320\270\320\262\320\260\320\275\320\270\321\217", nullptr));
        onLoadPhoto->setText(QCoreApplication::translate("Second", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\204\320\276\321\202\320\276", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Second: public Ui_Second {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SECOND_H
