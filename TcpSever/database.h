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
    bool addPoke(QString name,Spirit& s);
    bool addPoke(QString name, vector<Spirit*> p_list);
    //Spirit* queryPoke(QString name);
    vector<Spirit*> queryPoke(QString name);
private:
};

#endif // DATABASE_H
