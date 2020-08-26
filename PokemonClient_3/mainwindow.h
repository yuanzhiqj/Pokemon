#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "pokemon.h"
#include "loginscence.h"
#include "fight.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0,LoginScence* login = NULL);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    LoginScence* login;
    Fight* fightScence;
    vector<ATTRIBUTE*> attr_list;   //属性列表
    vector<Spirit*> poke_list;      //精灵列表

    vector<Spirit*> userPoke_list;  //用户的精灵列表
    QTcpSocket* tcpsocket;
    QString name;                   //用户名
    QString username;               //查看的用户名
    int cur;                        //当前宠物
    int mode;                       //显示模式

    void init();
    void setLabel(QString info);
    void setImg(QImage* img);

private slots:
    void getOnlineUser();
    void receOnlineUser(QByteArray);
    void doubleclicked(QListWidgetItem*);
    void receUserInfo(QByteArray);
    void getAllUser();
    void receAllUser(QByteArray);
    void backUser();
    void fight();
    void cfight();
    void whowin(QString winer, QString loser);
};

#endif // MAINWINDOW_H
