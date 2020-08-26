#ifndef POKEMON_H
#define POKEMON_H
#include <string>
#include "QList"
#include "QString"
#include <iostream>
#include <QDataStream>
using namespace std;

//升级所需的经验
const int levelexp[15] {10,20,50,80,100,150,200,300,500,800,1000,1500,2000,3000,5000 };

enum TYPE {
    STRENGTH = 0,
    TANKER = 1,
    DEFENSER = 2,
    ASSASSINER = 3
};

const QList<QString> typeName = {"高攻击", "高生命", "高防御", "高敏捷"};
const QList<QString> nameList_fire= {"火焰鸟","喷火龙","炎帝","固拉多"};
const QList<QString> nameList_water = {"杰尼龟","可达鸭","潜龙","鲤鱼王"};
const QList<QString> nameList_ele = {"皮卡丘","阿尔宙斯","雷电球","闪电鸟"};
static int id = 0;


//属性结构
struct ATTRIBUTE
{
    //int element;
    int type;				//精灵类型
    QString name;			//名字
    int id;                //精灵编号

    int level;					//等级
    int exp;					//经验
    int hp;					//生命
    int strength;				//力量
    int agility;				//敏捷
    int defense;				//防御

    friend QDataStream &operator>>(QDataStream& stream, ATTRIBUTE& attr);
    friend QDataStream &operator<<(QDataStream& stream, ATTRIBUTE& attr);
};

class Spirit {
protected:

    void levelup();				//升级
    void addAttributes();		//属性提升
    //int element;
    int _type;				//精灵类型
    QString _name;			//名字
    int _id;                //精灵编号

    int _level;					//等级
    int _exp;					//经验
    int _hp;					//生命
    int _curhp;                 //当前生命
    int _strength;				//力量
    int _agility;				//敏捷
    int _defense;				//防御
public:
    Spirit();					//构造函数
    Spirit(int type, QString name);
    Spirit(ATTRIBUTE* attr);
    ~Spirit() {};				//析构

    void gainExp(int exp);		//获得经验
    virtual int attack();		//攻击函数
    int beAttack(int);          //被攻击
    void recover();             //恢复血量
    void output();              //输出信息
    QString getInfo();          //获得文本信息
    QString getImg();           //获得图片信息
    //获取各项属性值的接口
    QString getName() const { return _name; }
    int getType() const { return _type; }
    int getId() const { return _id; }
    int getLevel() const { return _level; }
    int getExp() const { return _exp; }
    int getHp() const { return _curhp; }
    int getStrength() const { return _strength; }
    int getAgility() const { return _agility; }
    int getDedense() const { return _defense; }
    ATTRIBUTE* getAttribute();

    //friend QDataStream &operator>>(QDataStream &stream, Spirit &pkm);
    //friend QDataStream &operator<<(QDataStream &stream, Spirit &pkm);

    //友元函数

};



class Fire : public Spirit {
public:
    Fire(int type, QString name) :
        Spirit(type, name) {};
    Fire(ATTRIBUTE* attr) : Spirit(attr){};
    virtual int attack();
};

class Water : public Spirit {
public:
    Water(int type, QString name) :
        Spirit(type, name) {};
    Water(ATTRIBUTE* attr) : Spirit(attr){};
    virtual int attack();
};

class Ele : public Spirit {
public:
    Ele(int type, QString name) :
        Spirit(type, name) {};
    Ele(ATTRIBUTE* attr) : Spirit(attr){};
    virtual int attack();
};

#endif
