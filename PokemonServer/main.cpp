#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DataBase ms;
    ms.initSql();
    ms.createTable();
    MainWindow w;
    w.show();

    return a.exec();
}
