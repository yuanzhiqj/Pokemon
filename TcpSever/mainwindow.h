#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtNetwork>
#include<QTimer>
#include<QMessageBox>
#include"pokemon.h"
#include"database.h"

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

    bool checkSignIn(QString name,QString password);
    bool checkSignUp(QString name,QString password);
    void insertPoke(QString name, Spirit& s);
    void insertPoke(QString name, vector<Spirit*> p_list);
    //Spirit* queryPoke(QString name);
    vector<Spirit*> queryPoke(QString name);
    void generate(vector<Spirit*>& pokemon_list);
protected:
    void init();
private slots:
    void on_startBtn_clicked();
    void acceptConnection();
    void receiveData();
    void displayError(QAbstractSocket::SocketError);

private:
    Ui::MainWindow *ui;
    QTcpSocket* tcpSocket;
    QTcpServer* tcpServer;
    vector<QTcpServer*> tcp_list;
    QTimer* timer;
};

#endif // MAINWINDOW_H
