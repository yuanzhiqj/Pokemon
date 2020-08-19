#include "mainwindow.h"
#include "loginscence.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginScence login;
    if(login.exec ()==QDialog::Accepted)
    {

        MainWindow w(0,&login);
        w.show();
        return a.exec ();
    }
    else
        return 0;
}
