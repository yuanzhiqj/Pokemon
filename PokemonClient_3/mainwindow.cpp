#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
MainWindow::MainWindow(QWidget *parent,LoginScence* login) :
    QMainWindow(parent),login(login),
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
    qDebug() << "窗口初始化";
    cur = 0;
    mode = 0;
    ui->show_label->hide();
    ui->back_btn->hide();
    ui->cfight_btn->hide();
    this->attr_list = login->getAttr();
    this->name = login->getId();
    this->tcpsocket = login->getSocket();

    int pokenum = attr_list.size();
    QImage* img = new QImage;
    if(pokenum >= 10)
        img->load(":/res/金牌.png");
    else if(pokenum >= 8)
        img->load(":/res/银牌.png");
    else if(pokenum >= 5)
        img->load(":/res/铜牌.png");
    QImage* end = new QImage;
    *end = img->scaled(ui->medal_label->width(),ui->medal_label->height(),Qt::KeepAspectRatio);
    ui->medal_label->setPixmap(QPixmap::fromImage(*end));
    for(int i = 0; i < attr_list.size(); i++)
    {
        Spirit* tempPoke;
        if(count(nameList_fire.begin(),nameList_fire.end(),attr_list[i]->name))
            tempPoke = new Fire(attr_list[i]);
        else if(count(nameList_water.begin(),nameList_water.end(),attr_list[i]->name))
            tempPoke = new Water(attr_list[i]);
        else if(count(nameList_ele.begin(),nameList_ele.end(),attr_list[i]->name))
            tempPoke = new Ele(attr_list[i]);
        poke_list.push_back(tempPoke);
    }
    setLabel(poke_list[cur]->getInfo());
    QImage* img2 = new QImage;
    img2->load(poke_list[cur]->getImg());
    setImg(img2);

    //信号和槽函数
    connect(ui->next_btn,&QPushButton::clicked,this,[=](){
        if(mode == 0)
        {
            if(cur < poke_list.size()-1)
            {
                cur++;
                setLabel(poke_list[cur]->getInfo());
                QImage* img = new QImage;
                img->load(poke_list[cur]->getImg());
                setImg(img);
            }
        }
        else if(mode == 1)
        {
            if(cur < userPoke_list.size()-1)
            {
                cur++;
                setLabel(userPoke_list[cur]->getInfo());
                QImage* img = new QImage;
                img->load(userPoke_list[cur]->getImg());
                setImg(img);
            }
        }

    });
    connect(ui->pre_btn,&QPushButton::clicked,this,[=](){
        if(mode == 0)
        {
            if(cur > 0)
            {
                cur--;
                setLabel(poke_list[cur]->getInfo());
                QImage* img = new QImage;
                img->load(poke_list[cur]->getImg());
                setImg(img);
            }
        }
        else if(mode == 1)
        {
            if(cur > 0)
            {
                cur--;
                setLabel(userPoke_list[cur]->getInfo());
                QImage* img = new QImage;
                img->load(userPoke_list[cur]->getImg());
                setImg(img);
            }
        }

    });

    connect(ui->onlineUser_btn,&QPushButton::clicked,this,&MainWindow::getOnlineUser);
    connect(ui->regUser_btn,&QPushButton::clicked,this,&MainWindow::getAllUser);
    connect(login,&LoginScence::sendOnlineUser,this,&MainWindow::receOnlineUser);
    connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(doubleclicked(QListWidgetItem*)));
    //接收用户精灵消息
    connect(login,&LoginScence::sendUserInfo,this,&MainWindow::receUserInfo);
    connect(login,&LoginScence::sendAllUser,this,&MainWindow::receAllUser);
    connect(ui->back_btn,&QPushButton::clicked,this,&MainWindow::backUser);
    connect(ui->fight_btn,&QPushButton::clicked,this,&MainWindow::fight);
    connect(ui->cfight_btn,&QPushButton::clicked,this,&MainWindow::cfight);


}

void MainWindow::setLabel(QString info)
{
    ui->info_label->setText(info);
}

void MainWindow::setImg(QImage* img)
{
    QImage* end = new QImage;
    *end = img->scaled(ui->img_label->width(),ui->img_label->height(),Qt::KeepAspectRatio);
    ui->img_label->setPixmap(QPixmap::fromImage(*end));
}

void MainWindow::getOnlineUser()
{
    QByteArray data;
    QDataStream dsout(&data,QIODevice::ReadWrite);
    dsout << ONLINE;
    tcpsocket->write(data);
}

void MainWindow::receOnlineUser(QByteArray data)
{
    ui->listWidget->clear();
    QDataStream dsIn(&data,QIODevice::ReadWrite);
    int userNum;
    int j;
    dsIn >> j;
    dsIn >> userNum;
    qDebug() << userNum;
    for(int i = 0; i < userNum; i++)
    {
        QString tempUser;
        dsIn >> tempUser;
        ui->listWidget->addItem(tempUser);
    }
}

void MainWindow::doubleclicked(QListWidgetItem* item)
{
    if(item->text() == name)
        return;
    mode = 1;
    cur = 0;
    QString info = "当前显示的是用户" + item->text();
    ui->show_label->setText(info);
    ui->back_btn->show();
    ui->show_label->show();
    ui->cfight_btn->show();
    QString userName = item->text();
    username = userName;
    qDebug() << item->text();
    QByteArray data;
    QDataStream dsout(&data,QIODevice::ReadWrite);

    dsout << QUERY;
    dsout << userName;
    tcpsocket->write(data);
}

void MainWindow::receUserInfo(QByteArray data)
{
    QDataStream dsIn(&data,QIODevice::ReadWrite);
    int j;
    dsIn >> j;
    qDebug() << j;

    QString queryName;
    dsIn >> queryName;

    double winRate;
    dsIn >> winRate;
    qDebug() << "胜率为:" << winRate;
    QString info = "当前显示的是用户" + queryName + "\n胜率为" + QString::number(winRate);
    ui->show_label->setText(info);

    int pokenum;
    dsIn >> pokenum;

    QImage* img = new QImage;
    if(pokenum >= 10)
        img->load(":/res/金牌.png");
    else if(pokenum >= 8)
        img->load(":/res/银牌.png");
    else if(pokenum >= 5)
        img->load(":/res/铜牌.png");
    QImage* end = new QImage;
    *end = img->scaled(ui->medal_label->width(),ui->medal_label->height(),Qt::KeepAspectRatio);
    ui->medal_label->setPixmap(QPixmap::fromImage(*end));

    if(queryName == username)
    {
        //删除原列表
        vector<Spirit*>::iterator it;
        for (it = userPoke_list.begin(); it != userPoke_list.end(); )
        {
            it = userPoke_list.erase(it);
        }
        for(int i = 0; i < pokenum; i++)
        {
            ATTRIBUTE* tempAttr = new ATTRIBUTE;
            dsIn >> *tempAttr;
            qDebug() << tempAttr->name;
            Spirit* tempPoke;
            if(count(nameList_fire.begin(),nameList_fire.end(),tempAttr->name))
                tempPoke = new Fire(tempAttr);
            else if(count(nameList_water.begin(),nameList_water.end(),tempAttr->name))
                tempPoke = new Water(tempAttr);
            else if(count(nameList_ele.begin(),nameList_ele.end(),tempAttr->name))
                tempPoke = new Ele(tempAttr);
            userPoke_list.push_back(tempPoke);
        }
        setLabel(userPoke_list[cur]->getInfo());
        QImage* img = new QImage;
        img->load(userPoke_list[cur]->getImg());
        setImg(img);
    }
    else if(queryName == name)
    {
        //删除原列表
        vector<Spirit*>::iterator it;
        for (it = poke_list.begin(); it != poke_list.end(); )
        {
            it = poke_list.erase(it);
        }
        for(int i = 0; i < pokenum; i++)
        {
            ATTRIBUTE* tempAttr = new ATTRIBUTE;
            dsIn >> *tempAttr;
            qDebug() << tempAttr->name;
            Spirit* tempPoke;
            if(count(nameList_fire.begin(),nameList_fire.end(),tempAttr->name))
                tempPoke = new Fire(tempAttr);
            else if(count(nameList_water.begin(),nameList_water.end(),tempAttr->name))
                tempPoke = new Water(tempAttr);
            else if(count(nameList_ele.begin(),nameList_ele.end(),tempAttr->name))
                tempPoke = new Ele(tempAttr);
            poke_list.push_back(tempPoke);
        }
        ui->info_label->clear();
        setLabel(poke_list[0]->getInfo());
        QImage* img = new QImage;
        img->load(poke_list[0]->getImg());
        setImg(img);
    }
}

void MainWindow::getAllUser()
{
    QByteArray data;
    QDataStream dsout(&data,QIODevice::ReadWrite);
    dsout << REGISTER;
    tcpsocket->write(data);
}

void MainWindow::receAllUser(QByteArray data)
{
    qDebug() << "添加...";
    QDataStream dsIn(&data,QIODevice::ReadWrite);
    ui->listWidget->clear();
    int j;
    int userNum;
    dsIn >> j >> userNum;
    for(int i = 0; i < userNum; i++)
    {
        QString tempUser;
        dsIn >> tempUser;
        ui->listWidget->addItem(tempUser);
        qDebug() << tempUser;
    }
}

void MainWindow::backUser()
{
    mode = 0;
    cur = 0;
    setLabel(poke_list[cur]->getInfo());
    QImage* img = new QImage;
    img->load(poke_list[cur]->getImg());
    setImg(img);
    ui->show_label->hide();
    ui->back_btn->hide();
    ui->cfight_btn->hide();
}

void MainWindow::fight()
{
    qDebug() << "升级赛";
    //Ele* fightPoke = new Ele(ASSASSINER,"皮卡丘");
    Spirit* fightPoke = generateRandom(poke_list[cur]->getLevel());
    fightScence = new Fight(poke_list,fightPoke,name,"");
    connect(fightScence,&Fight::levelOver,this,&MainWindow::isLevelUp);
    fightScence->show();
}

void MainWindow::isLevelUp(bool isWin)
{

    if(isWin)
    {
        poke_list[cur]->gainExp(levelexp[poke_list[cur]->getLevel()]);
        qDebug() << "升级";
        QByteArray data;
        QDataStream dsOut(&data,QIODevice::ReadWrite);

        dsOut << LEVELUP;
        dsOut << name;
        dsOut << poke_list.size();
        for(int i = 0; i < poke_list.size(); i++)
        {
            ATTRIBUTE* tempattr = new ATTRIBUTE;
            tempattr = poke_list[i]->getAttribute();
            qDebug() << tempattr->level;
            dsOut << *tempattr;
        }
        tcpsocket->write(data);

        //更新界面
        QByteArray data2;
        QDataStream dsOut2(&data2,QIODevice::ReadWrite);

        dsOut2 << QUERY;
        dsOut2 << name;
        tcpsocket->write(data2);
    }
}
void MainWindow::cfight()
{
    fightScence = new Fight(poke_list,userPoke_list[cur],name,username);
    //接收到战斗场景信息
    connect(fightScence,&Fight::fightOver,this,&MainWindow::whowin);
    fightScence->show();

    //更新界面
    QByteArray data2;
    QDataStream dsOut2(&data2,QIODevice::ReadWrite);

    dsOut2 << QUERY;
    dsOut2 << name;
    tcpsocket->write(data2);
}
void MainWindow::whowin(QString winer, QString loser)
{
    QByteArray data;
    QDataStream dsOut(&data,QIODevice::ReadWrite);
    dsOut << GIVE;
    dsOut << winer;
    dsOut << loser;
    tcpsocket->write(data);
}

//生成一个随机精灵
Spirit* MainWindow::generateRandom(const int level)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int element = (qrand()+rand()) % 3;
    int name = (qrand()+rand()) % 4;
    int kind = (qrand()+rand()) % 4;

    qDebug() << "元素" << element << "名字" << name << "类型" << kind;
    //水元素
    if(element == 0)
    {
        Water* water = new Water((TYPE)kind, nameList_water[name]);
        water->gainExp(levelexp[level]);
        return water;
    }
    //火元素
    else if(element == 1)
    {
        Fire* fire = new Fire(TYPE(kind), nameList_fire[name]);
        fire->gainExp(levelexp[level]);
        return fire;
    }
    //电元素
    else if(element == 2)
    {
        Ele* ele = new Ele(TYPE(kind), nameList_ele[name]);
        ele->gainExp(levelexp[level]);
        return ele;
    }
}

/*
void MainWindow::paintEvent(QPaintEvent* )
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/back2.jpg");

    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}*/
