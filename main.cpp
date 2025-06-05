#include "mainwindow.h"

#include <QApplication>
// #include <QSqlDatabase>

// QSqlDatabase db = QSqlDatabase::addDatabase("QPostgre");


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // db.setHostName("localhost:5432");
    // db.setDatabaseName("flightdb");
    // db.setUserName("yupi");
    // db.setPassword("admin1234");
    // bool ok = db.open();

    return a.exec();
}
