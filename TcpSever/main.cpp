#include "mainwindow.h"
#include <QApplication>
#include "database.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DataBase ms;
    ms.initSql();
    ms.createTable();
    MainWindow w;
    w.setWindowTitle("Server");
    w.show();

    return a.exec();
}
