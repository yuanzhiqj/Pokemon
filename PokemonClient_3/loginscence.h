#ifndef LOGINSCENCE_H
#define LOGINSCENCE_H

#include <QDialog>
#include <QtNetwork>
#include <QMessageBox>
#include <QPainter>
#include "pokemon.h"
const uint SIGNIN = 1;
const uint SIGNUP = 2;
const uint SIGNIN_OK = 3;
const uint SIGNIN_FAIL = 4;
const uint SIGNUP_OK = 5;
const uint SIGNUP_FAIL = 6;
const uint ONLINE = 7;
const uint REGISTER = 8;
const uint QUERY = 9;
const uint GIVE = 10;
const uint LEVELUP = 11;

namespace Ui {
class LoginScence;
}

class LoginScence : public QDialog
{
    Q_OBJECT

public:
    explicit LoginScence(QWidget *parent = 0);
    ~LoginScence();
    QTcpSocket* getSocket();
    vector<ATTRIBUTE*> getAttr();
    QString getId();

private:
    Ui::LoginScence *ui;
    QTcpSocket* clientSocket;
    QString name;
    vector<ATTRIBUTE*> attr_list;

    //重写绘图函数
    void paintEvent(QPaintEvent* );
    void connect_to_server();
    void init();
signals:
    void sendOnlineUser(QByteArray);
    void sendUserInfo(QByteArray);
    void sendAllUser(QByteArray);
private slots:
    void signIn();              //登录
    void signUp();              //注册
    void readFromServer();      //从服务端读入数据
    void displayError(QAbstractSocket::SocketError);
};

#endif // LOGINSCENCE_H
