#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    this->tcpServer=new QTcpServer(this);
    this->tcpSocket=new QTcpSocket(this);


    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::on_startBtn_clicked);
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketError)));

}

void MainWindow::receiveData()
{
    qDebug() << "收到消息";
    /*
    QStringList list=data.split("#");

    bool ret = 0;
    if(list[0]=="a")    //注册
        ret = checkSignUp(list[1],list[2]);
    else if(list[0] == "b")    //登录
        ret = checkSignIn(list[1],list[2]);
    else
        return;
    QString sendData = list[0];
    if(ret)
        sendData += "#true";
    else
        sendData += "#false";
    tcpSocket->write(sendData.toLatin1());*/
    bool ret = 0;
    QByteArray data = tcpSocket->readAll();
    QDataStream dsIn(&data,QIODevice::ReadWrite);
    uint kind;
    QString name;
    QString pwd;
    dsIn >> kind >> name >> pwd;
    QByteArray tempData;
    QDataStream dsOut(&tempData,QIODevice::ReadWrite);
    if(kind == SIGNIN)
    {
        ret = checkSignIn(name,pwd);
        if(ret)
        {

            dsOut << SIGNIN_OK;
            //从数据库查询
            //Spirit* tempPoke = queryPoke(name);
            //dsOut << *tempPoke;
            vector<Spirit*> p_list = queryPoke(name);
            //qDebug() << "传递精灵" << p_list[0]->_name << p_list[1]->_name << p_list[2]->_name;
            dsOut << *p_list[0] << *p_list[1] << *p_list[2];
            qDebug() << "传递精灵" << p_list[0]->_name << p_list[1]->_name << p_list[2]->_name;
            tcpSocket->write(tempData);
        }
        else
        {
            dsOut << SIGNIN_FAIL;
            tcpSocket->write(tempData);
        }
    }
    else if(kind == SIGNUP)
    {
        ret = checkSignUp(name,pwd);
        if(ret)
        {
            Spirit* poke = new Spirit(STRENGTH,"精灵1");
            Spirit* poke2 = new Spirit(STRENGTH,"精灵2");
            Spirit* poke3 = new Spirit(STRENGTH,"精灵3");
            vector<Spirit*> poke_list;
            generate(poke_list);
            generate(poke_list);
            generate(poke_list);
            qDebug() << "精灵1" << poke_list[0]->_name;
            qDebug() << "精灵2" << poke_list[1]->_name;
            qDebug() << "精灵3" << poke_list[2]->_name;
            poke_list[0]->attack();
            poke_list[1]->attack();
            poke_list[2]->attack();
            //poke_list.push_back(poke);
            //poke_list.push_back(poke2);
            //poke_list.push_back(poke3);
            //insertPoke(name,poke);
            insertPoke(name,poke_list);
            dsOut << SIGNUP_OK;
            //dsOut << poke;
            vector<Spirit*> p_list = queryPoke(name);
            //qDebug() << "传递精灵" << p_list[0]->_name << p_list[1]->_name << p_list[2]->_name;
            dsOut << *p_list[0] << *p_list[1] << *p_list[2];
            qDebug() << "传递精灵" << p_list[0]->_name << poke2->_name << poke3->_name;
            tcpSocket->write(tempData);
        }
        else
        {
            dsOut << SIGNUP_FAIL;
            tcpSocket->write(tempData);
        }
    }
    else
        return;

}

void MainWindow::on_startBtn_clicked()
{
    ui->pushButton->setEnabled(false);


    if(!tcpServer->listen(QHostAddress::Any, 4396))
    {
        QMessageBox::information(this, "QT网络通信", "服务器端监听失败！");
        return;
    }
    else
    {
        QMessageBox::information(this, "QT网络通信", "服务器监听成功！");
    }
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

}

void MainWindow::acceptConnection()
{
    qDebug() << "建立连接";
    tcpSocket=tcpServer->nextPendingConnection();

    if(!tcpSocket)
    {
        QMessageBox::information(this, "QT网络通信", "未正确获取客户端连接！");
        return;
    }
    else
    {
        QMessageBox::information(this, "QT网络通信", "成功接受客户端的连接");
        //connect(mp_TCPSocket, SIGNAL(readyRead()), this, SLOT(ServerReadData()));
        //connect(mp_TCPSocket, SIGNAL(disconnected()), this, SLOT(sServerDisConnection()));
    }
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(receiveData()));
}

void MainWindow::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket->errorString();
    tcpSocket->close();
}

bool MainWindow::checkSignIn(QString name, QString password)
{
    DataBase* db = new DataBase();
    bool ret=db->loguser(name,password);
    return ret;

}

bool MainWindow::checkSignUp(QString name, QString password)
{
    DataBase* db = new DataBase();
    bool ret=db->signup(name,password);
    return ret;

}

void MainWindow::insertPoke(QString name,Spirit &s)
{
    DataBase* db = new DataBase();
    db->addPoke(name,s);
}

//重载版本
void MainWindow::insertPoke(QString name,vector<Spirit*> p_list)
{
    DataBase* db = new DataBase();
    db->addPoke(name,p_list);
}

/*
Spirit* MainWindow::queryPoke(QString name)
{
    DataBase* db = new DataBase();
    return db->queryPoke(name);
}*/

vector<Spirit*> MainWindow::queryPoke(QString name)
{
    DataBase* db = new DataBase();
    vector<Spirit*>res = db->queryPoke(name);
    qDebug() << "res::" << res[2]->_name;
    return res;
}

void MainWindow::generate(vector<Spirit*>& pokemon_list)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int element = (qrand()+rand()) % 3;
    int name = (qrand()+rand()) % 4;
    int kind = (qrand()+rand()) % 4;
//    if(kind == 0)
//        kind = STRENGTH;
//    else if(kind == 1)
//        kind = TANKER;
//    else if(kind == 2)
//        kind = DEFENSER;
//    else if(kind == 3)
//        kind = ASSASSINER;
    qDebug() << "元素" << element << "名字" << name << "类型" << kind;
    //水元素
    if(element == 0)
    {
        Water* water = new Water((TYPE)kind, nameList_water[name]);
        pokemon_list.push_back(water);
    }
    //火元素
    else if(element == 1)
    {
        Fire* fire = new Fire(TYPE(kind), nameList_fire[name]);
        pokemon_list.push_back(fire);
    }
    //电元素
    else if(element == 2)
    {
        Ele* ele = new Ele(TYPE(kind), nameList_ele[name]);
        pokemon_list.push_back(ele);
    }
}
