#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("postgres");
    db.setUserName("postgres");
    db.setPassword("C0d38_50AdM1Nn6");
    db.setPort(5432);

    if (!db.open())
    {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
    } else
    {
        qDebug() << "Подключение успешно!";
    }

    return a.exec();
}
