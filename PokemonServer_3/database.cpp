    #include "database.h"

DataBase::DataBase()
{

}

void DataBase::initSql()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","pokeDb");
    db.setDatabaseName(".//pokemon.db");
    if(!db.open())
    {
        qDebug() << "无法建立数据库连接";
    }
}

void DataBase::createTable()
{
    QSqlDatabase db = QSqlDatabase::database("pokeDb");
    QSqlQuery query(db);
    bool success = query.exec("create table user(id varchar primary key,password varchar,pokemon blob,pokenum integer,win integer, total integer)");
    if(success)
    {
        qDebug() << QObject::tr("数据库表创建成功!\n");
    }
    else
    {
        qDebug() << QObject::tr("数据库表创建失败!\n");
    }
}

bool DataBase::loguser(QString name, QString password)
{
    QSqlDatabase db = QSqlDatabase::database("pokeDb");
    qDebug() << name;
    qDebug() << password;

        QSqlQuery query(db);
        //sql语句在数据库中进行查询验证
        QString S =QString("select * from user where id='%1' and password='%2' ").arg(name).arg(password);
        query.exec(S);
        if(query.next())
        {
          // QMessageBox::warning(NULL,"Error","登录成功");
            return true;
        }
        else
            return false;
            //QMessageBox::warning(NULL,"Error","用户名或密码错误！！！");

}

bool DataBase::signup(QString name, QString password)
{
    //emit send(userId,pass);
    QByteArray data;
    QDataStream dsIn(&data,QIODevice::ReadWrite);
    QSqlDatabase db = QSqlDatabase::database("pokeDb");
    QSqlQuery query(db);
    query.prepare("insert into user values(?,?,?,?,?,?)");

    query.bindValue(0,name);
    query.bindValue(1,password);
    query.bindValue(2,data);
    query.bindValue(3,3);
    query.bindValue(4,0);
    query.bindValue(5,0);

    bool success=query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("插入失败"));
        return false;
    }
    return true;
}

bool DataBase::addPoke(QString name, ATTRIBUTE &s)
{
    QByteArray data;
    QDataStream dsIn(&data,QIODevice::ReadWrite);
    dsIn << s;
    QSqlDatabase db = QSqlDatabase::database("pokeDb");
    QSqlQuery query(db);
    //QString S =QString("update user set pokemon='%1' where id = '%2' ").arg(data).arg(name);
    //qDebug() << "正在插入的精灵为:" << s._name;
    query.prepare("UPDATE user set pokemon= :poke where id = :ID");
    query.bindValue(":poke",data);
    query.bindValue(":ID",name);
    bool success=query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("插入精灵失败"));
        return false;
    }
    return true;
}

//重载版本
bool DataBase::addPoke(QString name, vector<ATTRIBUTE*> p_list)
{
    QByteArray data;
    QDataStream dsIn(&data,QIODevice::ReadWrite);
    for(int i = 0; i < p_list.size(); i++)
    {
        dsIn << *p_list[i];
    }
    QSqlDatabase db = QSqlDatabase::database("pokeDb");
    QSqlQuery query(db);
    //QString S =QString("update user set pokemon='%1' where id = '%2' ").arg(data).arg(name);
    qDebug() << "正在插入的精灵为:" << p_list[2]->name;
    query.prepare("UPDATE user set pokemon= :poke, pokenum = :pokenum where id = :ID");
    query.bindValue(":poke",data);
    query.bindValue(":pokenum",3);
    query.bindValue(":ID",name);

    bool success=query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("插入精灵失败"));
        return false;
    }
    return true;
}

vector<ATTRIBUTE*> DataBase::queryPoke(QString name)
{
    QSqlDatabase db = QSqlDatabase::database("pokeDb");
    QSqlQuery query(db);
    QString S =QString("select * from user where id= '%1'").arg(name);
    query.exec(S);
    //query.exec("select* from automobil");
    QSqlRecord rec = query.record();
    vector<ATTRIBUTE*> p_list;

    while(query.next())
    {
        int pokenum = query.value(3).toInt();
        QByteArray data = query.value(2).toByteArray();
        QDataStream dsOut(&data,QIODevice::ReadWrite);
        for(int i = 0; i < pokenum; i++)
        {
            ATTRIBUTE* qq = new ATTRIBUTE;
            dsOut >> *qq;
            p_list.push_back(qq);
        }

    }
    return p_list;
}

double DataBase::queryWinRate(const QString name)
{
    QSqlDatabase db = QSqlDatabase::database("pokeDb");
    QSqlQuery query(db);
    QString S =QString("select * from user where id= '%1'").arg(name);
    query.exec(S);
    int totalNum = 0;
    int winNum = 0;
    double winRate = 0.0;
    while(query.next())
    {
        winNum = query.value(4).toInt();
        totalNum = query.value(5).toInt();
    }
    if(totalNum == 0)
    {
        qDebug() << "无比赛记录";
        return 0;
    }
    else
        winRate = (double)winNum / totalNum;
    qDebug() << "胜场为:" << winNum << "胜率为" << winRate;
    return winRate;
}

vector<QString> DataBase::queryAllUsers()
{
    QSqlDatabase db = QSqlDatabase::database("pokeDb");
    QSqlQuery query(db);
    QSqlRecord rec = query.record();
    query.exec("SELECT * FROM user");
    vector<QString> user_list;
    while(query.next())
    {
        //qDebug() << query.value(id).toString();
        user_list.push_back(query.value(id).toString());
    }
    return user_list;
}

void DataBase::updatePoke(QString name, vector<ATTRIBUTE*> p_list, int num, bool iswin)
{
    QByteArray data;
    QDataStream dsIn(&data,QIODevice::ReadWrite);
    for(int i = 0; i < p_list.size(); i++)
    {
        dsIn << *p_list[i];
    }
    QSqlDatabase db = QSqlDatabase::database("pokeDb");
    QSqlQuery query(db);

    QString S =QString("select * from user where id= '%1'").arg(name);
    query.exec(S);

    while(query.next())
    {
        int winnum = query.value(4).toInt();
        if(iswin == true)
            winnum = winnum+1;
        int totalnum = query.value(5).toInt()+1;
        qDebug() << "更新后的胜场" << winnum << totalnum;

        //QSqlQuery query(db);
        query.prepare("UPDATE user set pokemon= :poke, pokenum = :pokenum,win = :win, total = :total where id = :ID");
        query.bindValue(":poke",data);
        query.bindValue(":pokenum",num);
        query.bindValue(":win",winnum);
        query.bindValue(":total",totalnum);
        query.bindValue(":ID",name);

        bool success=query.exec();
        if(!success)
        {
            QSqlError lastError = query.lastError();
            qDebug() << lastError.driverText() << QString(QObject::tr("更新精灵失败"));
        }
    }

}

void DataBase::levelUp(QString name,vector<ATTRIBUTE*> attr_list)
{
    QByteArray data;
    QDataStream dsIn(&data,QIODevice::ReadWrite);
    for(int i = 0; i < attr_list.size(); i++)
    {
        dsIn << *attr_list[i];
    }
    QSqlDatabase db = QSqlDatabase::database("pokeDb");
    QSqlQuery query(db);

    query.prepare("UPDATE user set pokemon= :poke where id = :ID");
    query.bindValue(":poke",data);
    query.bindValue(":ID",name);

    bool success=query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("更新升级精灵失败"));
    }
}
