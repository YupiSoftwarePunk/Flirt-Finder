#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include "include/mainwindow.h"
#include "qdir.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Загрузка стилей
    QFile styleFile(":/styles/style.css");
    if (!styleFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Не удалось открыть файл стилей!";
    }
    else
    {
        a.setStyleSheet(styleFile.readAll());
        styleFile.close();
    }

    a.setApplicationName("FlirtFinder");

    MainWindow w;
    w.show();

    return a.exec();
}
