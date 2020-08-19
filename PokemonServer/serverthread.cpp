#include "serverthread.h"

serverThread::serverThread(int sockDesc, QObject *parent) :
    QThread(parent),
    m_sockDesc(sockDesc)
{

}

serverThread::~serverThread()
{
    m_socket->close();
}

void serverThread::run(void)
{
    m_socket = new MySocket(m_sockDesc);

    if (!m_socket->setSocketDescriptor(m_sockDesc)) {
        qDebug() << "返回";
        return ;
    }

    connect(m_socket, &MySocket::disconnected, this, &serverThread::disconnectToHost);
    connect(m_socket, SIGNAL(dataReady(int,QByteArray)),
            this, SLOT(recvDataSlot(int,QByteArray)));
    connect(this, SIGNAL(sendData(int, QByteArray)),
            m_socket, SLOT(sendData(int,QByteArray)));

    this->exec();
}

void serverThread::sendDataSlot(int sockDesc, QByteArray data)
{
    QDataStream dsIn(&data,QIODevice::ReadWrite);
    QString msg;
    dsIn >> msg;
    qDebug() << msg;
    if (data.isEmpty()) {
        qDebug() << "数据为空";
        return ;
    }
    qDebug() << "线程发送";
    emit sendData(sockDesc, data);
}

void serverThread::recvDataSlot(int id,QByteArray data)
{
    qDebug() << "线程收到消息";
    emit dataReady(id, data);
}

void serverThread::disconnectToHost(void)
{
    emit disconnectTCP(m_sockDesc);
    m_socket->disconnectFromHost();
    this->quit();
}
