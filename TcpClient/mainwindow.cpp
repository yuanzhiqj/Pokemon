#include "mainwindow.h"
#include "ui_mainwindow.h"

#define ip "127.0.0.1"
#define port 4396
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pwd_edit->setEchoMode(QLineEdit::Password);
    map_w = new QWidget();
    psw = new PlayScence();
    init();
    connect_to_server();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    connect(ui->signin_btn,&QPushButton::clicked,this,&MainWindow::on_sendBtn_clicked);
    connect(ui->signup_btn,&QPushButton::clicked,this,&MainWindow::on_signBtn_clicket);
    connect(ui->connectBtn,&QPushButton::clicked,this,&MainWindow::connectSever);
    //map_w = new QWidget(this);
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketAccessError)));

}
void MainWindow::connectSever()
{
    //tcpSocket->abort();
    tcpSocket->connectToHost(ip,4396);
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessages()));
    qDebug() << "连接";

    if(!tcpSocket->waitForConnected(30000))

    {

    QMessageBox::information(this, "QT网络通信", "连接服务端失败！");

    return;

    }
}

void MainWindow::on_sendBtn_clicked()
{

    QString userName = ui->id_edit->text();
    QString password = ui->pwd_edit->text();
    if(userName=="" || password=="")
        QMessageBox::information(this,"警告","输入不能为空",QMessageBox::Ok);

    /*
    QString bs="b";
    QString data = bs+"#"+userName+"#"+password;*/

    QByteArray data;
    QDataStream dsout(&data,QIODevice::ReadWrite);
    dsout << SIGNIN << userName << password;
    tcpSocket->write(data);
}

void MainWindow::on_signBtn_clicket()
{
    QString userName = ui->id_edit->text();
    QString password = ui->pwd_edit->text();
    if(userName=="" || password=="")
        QMessageBox::information(this,"警告","输入不能为空",QMessageBox::Ok);
    /*
    QString as = "a";
    QString data = as+"#"+userName+"#"+password;*/
    QByteArray data;
    QDataStream dsout(&data,QIODevice::ReadWrite);
    dsout << SIGNUP << userName << password;
    tcpSocket->write(data);
}

void MainWindow::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<tcpSocket->errorString();
}

void MainWindow::readMessages()
{
    qDebug() << "收到信息";
    //QString data = tcpSocket->readAll();
    /*
    QStringList list=data.split("#");
    if(list[0]=='a' && list[1]=="true")
        QMessageBox::information(this,"信息提示","注册成功!",QMessageBox::Ok);
    else if(list[0]=="a" && list[1]=="false")
        QMessageBox::information(this,"信息提示","注册失败,用户名已被注册!",QMessageBox::Ok);
    else if(list[0]=="b" && list[1]=="true")
        QMessageBox::information(this,"信息提示","登录成功!",QMessageBox::Ok);
    else if(list[0]=="b" && list[1]=="false")
        QMessageBox::information(this,"信息提示","登录失败,用户名或密码错误!",QMessageBox::Ok);
    else
        return;*/
    QByteArray data = tcpSocket->readAll();
    QDataStream dsin(&data,QIODevice::ReadWrite);
    uint loginKind;
    QString username;
    QString pwd;
    dsin>>loginKind;
    if(loginKind == SIGNUP_OK)
    {
        QMessageBox::information(this,"信息提示","注册成功!",QMessageBox::Ok);
        /*QByteArray test;
        QDataStream testIN(&test,QIODevice::ReadWrite);
        Spirit s(STRENGTH,"皮卡");
        testIN << s;*/

        Spirit* s = new Spirit(STRENGTH,"皮卡");
        Spirit* s1 = new Spirit(STRENGTH,"皮卡");
        Spirit* s2 = new Spirit(STRENGTH,"皮卡");
        vector<Spirit*> p_list;

        dsin >> *s >> *s1 >> *s2;
        p_list.push_back(s);
        p_list.push_back(s1);
        p_list.push_back(s2);
        qDebug() << s->_name << s1->_name << s2->_name;
        for(int i = 0; i < 3; i++)
        {
            psw->setLable(p_list[i]->getInfo(),i);
            qDebug() << p_list[i]->getInfo();
            QImage* img = new QImage;
            img->load(p_list[i]->getImg());
            psw->setImage(img,i);
        }
        this->close();
        psw->show();
    }
    else if(loginKind == SIGNUP_FAIL)
        QMessageBox::information(this,"信息提示","注册失败,用户名已被注册!",QMessageBox::Ok);
    else if(loginKind == SIGNIN_OK)
    {
        QMessageBox::information(this,"信息提示","登录成功!",QMessageBox::Ok);
        Spirit* s = new Spirit(STRENGTH,"皮卡");
        Spirit* s1 = new Spirit(STRENGTH,"皮卡");
        Spirit* s2 = new Spirit(STRENGTH,"皮卡");
        vector<Spirit*> p_list;
        dsin >> *s >> *s1 >> *s2;
        p_list.push_back(s);
        p_list.push_back(s1);
        p_list.push_back(s2);
        qDebug() << s->_name << s1->_name << s2->_name;
        for(int i = 0; i < 3; i++)
        {
            psw->setLable(p_list[i]->getInfo(),i);
            qDebug() << p_list[i]->getInfo();
            QImage* img = new QImage;
            img->load(p_list[i]->getImg());
            psw->setImage(img,i);
        }
        this->close();
        psw->show();
    }
    else if(loginKind == SIGNIN_FAIL)
    {
        QMessageBox::information(this,"信息提示","登录失败,用户名或密码错误!",QMessageBox::Ok);

    }

}

void MainWindow::connect_to_server()
{

        //tcpSocket->abort();
        tcpSocket->connectToHost(ip,4396);
        connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessages()));
        qDebug() << "连接";

        if(!tcpSocket->waitForConnected(30000))

        {

        QMessageBox::information(this, "QT网络通信", "连接服务端失败！");

        return;

        }
}
