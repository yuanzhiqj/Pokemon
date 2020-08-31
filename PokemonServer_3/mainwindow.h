#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"pokemon.h"
#include"database.h"

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
class MainWindow;
}
class Server;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    static MainWindow* getMainWindow();
    ~MainWindow();

signals:
    void sendData(int id,QByteArray data);

public slots:
    void recvData(int id,QByteArray data);

private:
    Ui::MainWindow *ui;
    friend class Server;

    QTimer *m_timer;
    Server *m_server;

    int m_count;
    vector<QString> user_list;

    bool checkSignIn(QString name,QString password);
    bool checkSignUp(QString name,QString password);
    void insertPoke(QString name, vector<ATTRIBUTE*> p_list);
    void updatePoke(QString name, vector<ATTRIBUTE*> p_list, int num, bool iswin);
    vector<ATTRIBUTE*> queryPoke(QString name);
    //查询胜率
    double queryWinRate(const QString name);
    void generate(vector<Spirit*>& pokemon_list);
    void levelUp(QString name,vector<ATTRIBUTE*> attr_list);
    vector<QString> queryAllUsers();
};

#endif // MAINWINDOW_H
