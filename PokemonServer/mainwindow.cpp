#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "server.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_server = new Server(this);
    m_server->listen(QHostAddress::Any, 4396);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::recvData(int id,QByteArray data)
{
    QString msg;
    QDataStream dsIn(&data,QIODevice::ReadWrite);

    bool ret = 0;
    uint kind;
    QString name;
    QString pwd;
    dsIn >> kind;
    QByteArray tempData;
    QDataStream dsOut(&tempData,QIODevice::ReadWrite);
    if(kind == SIGNIN)
    {
        dsIn >> name >> pwd;
        ret = checkSignIn(name,pwd);
        if(ret)
        {
            user_list.push_back(name);
            dsOut << SIGNIN_OK;
            //从数据库查询
            vector<ATTRIBUTE*> p_list = queryPoke(name);
            dsOut << *p_list[0] << *p_list[1] << *p_list[2];
            qDebug() << "传递精灵" << p_list[0]->name << p_list[1]->name << p_list[2]->name;
            emit sendData(id,tempData);
        }
        else
        {
            dsOut << SIGNIN_FAIL;
            emit sendData(id,tempData);
        }
    }
    else if(kind == SIGNUP)
    {
        dsIn >> name >> pwd;
        ret = checkSignUp(name,pwd);
        if(ret)
        {
            user_list.push_back(name);
            vector<Spirit*> poke_list;
            generate(poke_list);
            generate(poke_list);
            generate(poke_list);
            vector<ATTRIBUTE*> attr_list;
            for(int i = 0; i < 3; i++)
            {
                ATTRIBUTE* tempAttr = poke_list[i]->getAttribute();
                attr_list.push_back(tempAttr);
            }
            insertPoke(name,attr_list);
            dsOut << SIGNUP_OK;
            vector<ATTRIBUTE*> p_list = queryPoke(name);
            dsOut << *p_list[0] << *p_list[1] << *p_list[2];
            qDebug() << "注册：" << p_list[0]->name;
            emit sendData(id,tempData);
        }
        else
        {
            dsOut << SIGNUP_FAIL;
            emit sendData(id,tempData);
        }
    }
    else if(kind == ONLINE)
    {
        dsOut << ONLINE;
        dsOut << user_list.size();
        qDebug() <<user_list.size();
        for(int i = 0; i < user_list.size();i++)
        {
            dsOut << user_list[i];
        }
        emit sendData(id,tempData);
    }
    else if(kind == QUERY)
    {   //查询用户精灵
        QString queryName;
        dsIn >> queryName;
        dsOut << QUERY;
        vector<ATTRIBUTE*> p_list = queryPoke(queryName);
        dsOut << *p_list[0] << *p_list[1] << *p_list[2];
        emit sendData(id,tempData);
    }
    else if(kind == REGISTER)
    {   //获得所有用户
        vector<QString> allUser_list;
        allUser_list = queryAllUsers();
        dsOut << REGISTER;
        dsOut << allUser_list.size();
        for(int i = 0; i < allUser_list.size();i++)
        {
            dsOut << allUser_list[i];
            qDebug() << allUser_list[i];
        }
        emit sendData(id,tempData);
    }
    else
        return;
}

MainWindow* MainWindow::getMainWindow()
{
    foreach(QWidget *w, qApp->topLevelWidgets())
    if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
            return mainWin;
    return nullptr;
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

void MainWindow::insertPoke(QString name,vector<ATTRIBUTE*> p_list)
{
    DataBase* db = new DataBase();
    db->addPoke(name,p_list);
}

vector<ATTRIBUTE*> MainWindow::queryPoke(QString name)
{
    DataBase* db = new DataBase();
    vector<ATTRIBUTE*>res = db->queryPoke(name);
    qDebug() << "res::" << res[2]->name;
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

vector<QString> MainWindow::queryAllUsers()
{
    DataBase* db = new DataBase();
    return db->queryAllUsers();
}




