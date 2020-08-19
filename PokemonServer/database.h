#ifndef DATABASE_H
#define DATABASE_H

#include<QTextCodec>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>
#include <QtDebug>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QMessageBox>
#include "pokemon.h"
class DataBase
{
public:
    DataBase();
    void initSql();
    void createTable();
    bool loguser(QString name,QString password);
    bool signup(QString name, QString password);
    bool addPoke(QString name,ATTRIBUTE& s);
    bool addPoke(QString name, vector<ATTRIBUTE*> p_list);
    vector<QString> queryAllUsers();
    //Spirit* queryPoke(QString name);
    vector<ATTRIBUTE*> queryPoke(QString name);
private:
};

#endif // DATABASE_H
