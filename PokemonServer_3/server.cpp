#include "mainwindow.h"
#include "server.h"

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
    /* get current dialog object */
    m_window = MainWindow::getMainWindow();
}

Server::~Server()
{

}

void Server::incomingConnection(int sockDesc)
{
    m_socketList.append(sockDesc);
    qDebug() << "客户端编号" << sockDesc;
    serverThread *thread = new serverThread(sockDesc);

    //connect(thread, SIGNAL(disconnectTCP(int)), this, SLOT(clientDisconnected(int)));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(thread, SIGNAL(dataReady(int,QByteArray)),
            m_window, SLOT(recvData(int,QByteArray)));

    connect(m_window, SIGNAL(sendData(int,QByteArray)),
            thread, SLOT(sendDataSlot(int,QByteArray)));

    thread->start();
}
