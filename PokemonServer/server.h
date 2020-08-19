#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "serverthread.h"
#include "mainwindow.h"

class MainWindow;

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = Q_NULLPTR);
    ~Server();

private:
    void incomingConnection(int sockDesc);

    /*
private slots:
    void clientDisconnected(int sockDesc);*/

private:
    MainWindow *m_window;

    QList<int> m_socketList;
};

#endif // SERVER_H
