#include "loginscence.h"
#include "ui_loginscence.h"

#define ip "127.0.0.1"
#define port 4396

LoginScence::LoginScence(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginScence)
{
    ui->setupUi(this);
    ui->pwd_edit->setEchoMode(QLineEdit::Password);
    init();
    connect_to_server();
}

LoginScence::~LoginScence()
{
    delete ui;
}

//初始化
void LoginScence::init()
{
    //信号和槽的初始化
    connect(ui->signIn_btn,&QPushButton::clicked,this,&LoginScence::signIn);
    connect(ui->signUp_btn,&QPushButton::clicked,this,&LoginScence::signUp);
    //connect(ui->connectBtn,&QPushButton::clicked,this,&MainWindow::connectSever);
    //map_w = new QWidget(this);
    clientSocket = new QTcpSocket(this);
    connect(clientSocket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketAccessError)));
}

void LoginScence::connect_to_server()
{
    //连接至服务端
    clientSocket->connectToHost(ip,4396);
    connect(clientSocket,SIGNAL(readyRead()),this,SLOT(readFromServer()));
    if(!clientSocket->waitForConnected(30000))
    {
        QMessageBox::information(this, "QT网络通信", "连接服务端失败！");
        return;
    }
}

void LoginScence::signIn()
{
    //登录
    QString userName = ui->id_edit->text();
    QString password = ui->pwd_edit->text();
    if(userName=="" || password=="")
        QMessageBox::information(this,"警告","输入不能为空",QMessageBox::Ok);
    QByteArray data;
    QDataStream dsout(&data,QIODevice::ReadWrite);
    dsout << SIGNIN << userName << password;
    clientSocket->write(data);

}

void LoginScence::signUp()
{
    //注册
    QString userName = ui->id_edit->text();
    QString password = ui->pwd_edit->text();
    if(userName=="" || password=="")
        QMessageBox::information(this,"警告","输入不能为空",QMessageBox::Ok);
    QByteArray data;
    QDataStream dsout(&data,QIODevice::ReadWrite);
    dsout << SIGNUP << userName << password;
    clientSocket->write(data);
}

void LoginScence::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<clientSocket->errorString();
}

void LoginScence::readFromServer()
{
    qDebug() << "收到信息";
    QByteArray data = clientSocket->readAll();
    QDataStream dsin(&data,QIODevice::ReadWrite);
    uint loginKind;
    QString username;
    QString pwd;
    dsin>>loginKind;
    if(loginKind == SIGNUP_OK)
    {
        QMessageBox::information(this,"信息提示","注册成功!",QMessageBox::Ok);

        for(int i = 0; i < 3; i++)
        {
            ATTRIBUTE* temp = new ATTRIBUTE;
            dsin >> *temp;
            qDebug() << temp->name;
            attr_list.push_back(temp);
        }
        for(int i = 0; i < 3; i++)
        {
            qDebug() << attr_list[i]->name;
        }
        emit this->accept();
    }
    else if(loginKind == SIGNUP_FAIL)
        QMessageBox::information(this,"信息提示","注册失败,用户名已被注册!",QMessageBox::Ok);
    else if(loginKind == SIGNIN_OK)
    {
        QMessageBox::information(this,"信息提示","登录成功!",QMessageBox::Ok);
        this->name = ui->id_edit->text();
        for(int i = 0; i < 3; i++)
        {
            ATTRIBUTE* temp = new ATTRIBUTE;
            dsin >> *temp;
            attr_list.push_back(temp);
        }
        for(int i = 0; i < 3; i++)
        {
            qDebug() << attr_list[i]->name;
        }
        emit this->accept();
    }
    else if(loginKind == SIGNIN_FAIL)
    {
        QMessageBox::information(this,"信息提示","登录失败,用户名或密码错误!",QMessageBox::Ok);

    }
    else if(loginKind == ONLINE)
    {
        emit sendOnlineUser(data);
    }
    else if(loginKind == QUERY)
    {
        emit sendUserInfo(data);
    }
    else if(loginKind == REGISTER)
    {
        emit sendAllUser(data);
    }

}

vector<ATTRIBUTE*> LoginScence::getAttr()
{
    return this->attr_list;
}

QTcpSocket* LoginScence::getSocket()
{
    return this->clientSocket;
}

QString LoginScence::getId()
{
    return this->name;
}




