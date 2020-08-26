#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H


#include <QThread>
#include <QObject>
#include <QtNetwork>
#include <QDebug>

#include "mysocket.h"

class Socket;

class serverThread : public QThread
{
    Q_OBJECT
public:
    serverThread(int sockDesc, QObject *parent = Q_NULLPTR);
    ~serverThread();

private:
    void run(void);

public slots:
    void sendDataSlot(int sockDesc,QByteArray data);

signals:
    void dataReady(int id,QByteArray data);
    void sendData(int sockDesc,QByteArray data);
    void disconnectTCP(int sockDesc);

private slots:
    void recvDataSlot(int id,QByteArray data);
    void disconnectToHost(void);

private:
    MySocket *m_socket;

    int m_sockDesc;
};

#endif // SERVERTHREAD_H
