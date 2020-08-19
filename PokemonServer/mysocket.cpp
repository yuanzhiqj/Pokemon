#include "mysocket.h"

MySocket::MySocket(int sockDesc, QObject *parent) :
    QTcpSocket(parent),
    m_sockDesc(sockDesc)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(recvData()));
}

MySocket::~MySocket()
{

}

void MySocket::sendData(int id,QByteArray data)
{
    if (id == m_sockDesc && !data.isEmpty()) {
        qDebug() << "socket发送";
        this->write(data);
    }
}

void MySocket::recvData(void)
{
    qDebug() << "socket收到消息";
    int id = m_sockDesc;
    QByteArray data = readAll();
    emit dataReady(id, data);
}
