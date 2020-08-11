#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QMessageBox>
#include"pokemon.h"
#include"playscence.h"
const uint SIGNIN = 1;
const uint SIGNUP = 2;
const uint SIGNIN_OK = 3;
const uint SIGNIN_FAIL = 4;
const uint SIGNUP_OK = 5;
const uint SIGNUP_FAIL = 6;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void connect_to_server();
    QWidget* map_w;
    PlayScence* psw;
private:
    Ui::MainWindow *ui;
    QTcpSocket* tcpSocket;
private slots:
    void on_sendBtn_clicked();
    void displayError(QAbstractSocket::SocketError);
    void on_signBtn_clicket();
    void readMessages();
    void connectSever();
protected:
    void init();

};

#endif // MAINWINDOW_H
