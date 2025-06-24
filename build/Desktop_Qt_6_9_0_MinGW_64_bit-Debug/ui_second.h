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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Second
{
public:
    QGroupBox *groupBox;
    QPushButton *onSaveData;
    QPushButton *onLoadPhoto;
    QGraphicsView *graphicsView;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;
    QComboBox *comboBox;
    QSpinBox *spinBox;
    QTextEdit *textEdit;
    QLineEdit *lineEdit_4;

    void setupUi(QDialog *Second)
    {
        if (Second->objectName().isEmpty())
            Second->setObjectName("Second");
        Second->resize(400, 300);
        groupBox = new QGroupBox(Second);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(10, 0, 381, 291));
        onSaveData = new QPushButton(groupBox);
        onSaveData->setObjectName("onSaveData");
        onSaveData->setGeometry(QRect(170, 260, 80, 24));
        onLoadPhoto = new QPushButton(groupBox);
        onLoadPhoto->setObjectName("onLoadPhoto");
        onLoadPhoto->setGeometry(QRect(270, 150, 91, 24));
        graphicsView = new QGraphicsView(groupBox);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setGeometry(QRect(250, 30, 121, 101));
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(10, 30, 231, 222));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit = new QLineEdit(layoutWidget);
        lineEdit->setObjectName("lineEdit");

        verticalLayout->addWidget(lineEdit);

        comboBox = new QComboBox(layoutWidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");

        verticalLayout->addWidget(comboBox);

        spinBox = new QSpinBox(layoutWidget);
        spinBox->setObjectName("spinBox");
        spinBox->setMinimum(16);

        verticalLayout->addWidget(spinBox);

        textEdit = new QTextEdit(layoutWidget);
        textEdit->setObjectName("textEdit");

        verticalLayout->addWidget(textEdit);

        lineEdit_4 = new QLineEdit(layoutWidget);
        lineEdit_4->setObjectName("lineEdit_4");

        verticalLayout->addWidget(lineEdit_4);


        retranslateUi(Second);

        QMetaObject::connectSlotsByName(Second);
    } // setupUi

    void retranslateUi(QDialog *Second)
    {
        Second->setWindowTitle(QCoreApplication::translate("Second", "Dialog", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Second", "\320\227\320\260\320\277\320\276\320\273\320\275\320\265\320\275\320\270\320\265 \320\270\320\273\320\270 \321\200\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265 \320\260\320\275\320\272\320\265\321\202\321\213", nullptr));
        onSaveData->setText(QCoreApplication::translate("Second", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        onLoadPhoto->setText(QCoreApplication::translate("Second", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\204\320\276\321\202\320\276", nullptr));
        lineEdit->setText(QString());
        lineEdit->setPlaceholderText(QCoreApplication::translate("Second", "\320\230\320\274\321\217", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("Second", "\320\234\321\203\320\266\321\201\320\272\320\276\320\271", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("Second", "\320\226\320\265\320\275\321\201\320\272\320\270\320\271", nullptr));

        comboBox->setPlaceholderText(QCoreApplication::translate("Second", "\320\237\320\276\320\273", nullptr));
        textEdit->setPlaceholderText(QCoreApplication::translate("Second", "\320\243\320\262\320\273\320\265\321\207\320\265\320\275\320\270\321\217", nullptr));
        lineEdit_4->setText(QString());
        lineEdit_4->setPlaceholderText(QCoreApplication::translate("Second", "\320\223\320\276\321\200\320\276\320\264 \320\277\321\200\320\276\320\266\320\270\320\262\320\260\320\275\320\270\321\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Second: public Ui_Second {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SECOND_H
