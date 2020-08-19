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
    cur = 0;
    mode = 0;
    ui->show_label->hide();
    ui->back_btn->hide();
    this->attr_list = login->getAttr();
    this->name = login->getId();
    this->tcpsocket = login->getSocket();
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
    QImage* img = new QImage;
    img->load(poke_list[cur]->getImg());
    setImg(img);

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
    connect(login,&LoginScence::sendUserInfo,this,&MainWindow::receUserInfo);
    connect(login,&LoginScence::sendAllUser,this,&MainWindow::receAllUser);
    connect(ui->back_btn,&QPushButton::clicked,this,&MainWindow::backUser);
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
    mode = 1;
    cur = 0;
    QString info = "当前显示的是用户" + item->text();
    ui->show_label->setText(info);
    ui->back_btn->show();
    ui->show_label->show();
    QString userName = item->text();
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
    vector<Spirit*>::iterator it;
    for (it = userPoke_list.begin(); it != userPoke_list.end(); )
    {
        it = userPoke_list.erase(it);
    }
    for(int i = 0; i < 3; i++)
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
}
