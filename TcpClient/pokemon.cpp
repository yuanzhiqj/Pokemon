#include"pokemon.h"
#include"QDebug"

Spirit::Spirit(TYPE type, QString name):
    _type(type), _name(name)
{
    _level = 0;
    _exp = 0;
    _id = id++;
    if(_type == STRENGTH)
    {
        _hp = rand() % 15 + 20;         //20~35
        _strength = rand() % 5 + 10;    //10~15
        _agility = rand() % 10 + 1;     //1~10
        _defense = rand() % 5 + 5;      //5~10
    }
    else if(_type == TANKER)
    {
        _hp = rand() % 15 + 30;         //30~45
        _strength = rand() % 5 + 5;     //5~10
        _agility = rand() % 10 + 1;     //1~10
        _defense = rand() % 5 + 5;      //5~10
    }
    else if(_type == DEFENSER)
    {
        _hp = rand() % 15 + 20;         //20~35
        _strength = rand() % 5 + 5;     //5~10
        _agility = rand() % 10 + 1;     //1~10
        _defense = rand() % 5 + 10;     //10~15
    }
    else if(_type == ASSASSINER)
    {
        _hp = rand() % 15 + 20;         //20~35
        _strength = rand() % 5 + 5;     //5~10
        _agility = rand() % 10 + 5;     //5~15
        _defense = rand() % 5 + 5;      //5~10
    }
}
void Spirit::gainExp(int exp)
{
    _exp += exp;
    //存在一次升多级的情况
    while (_exp >= levelexp[_level] && _level != 15)
    {
        levelup();
    }
}
void Spirit::levelup()
{
    _level++;
    //属性增加
    addAttributes();
}

int Spirit::attack()
{
    int d = rand() % 10;
    int damage = _strength * d;
    //cout << "精灵" << _name << "使用普通攻击，打出了" << damage << "的伤害";
    return damage;
}

void Spirit::addAttributes()
{
    int r = rand() % 10 + 10;
    int s = rand() % 2 + 1;
    //精灵的主属性会增加更多。
    if (_type == STRENGTH)
    {
        _strength += s * r;
        _hp += r;
        _defense += r;
        _agility += r;
    }
    else if (_type == TANKER)
    {
        _strength += r;
        _hp += s * r;
        _defense += r;
        _agility += r;
    }
    else if (_type == DEFENSER)
    {
        _strength += r;
        _hp += r;
        _defense += s * r;
        _agility += r;
    }
    else if (_type == ASSASSINER)
    {
        _strength += r;
        _hp += r;
        _defense += r;
        _agility += s * r;
    }
}


void Spirit::output()
{
    qDebug() << "名字: " << _name << endl
             << "编号: " << _id << endl
             << "类型: " << typeName[_type] << endl
             << "等级: " << _level << endl
             << "经验: " << _exp << endl
             << "生命: " << _hp << endl
             << "力量: " << _strength << endl
             << "防御: " << _defense << endl
             << "敏捷: " << _agility << endl;

}

QString Spirit::getInfo()
{
    QString info,temp;
    info += "名字:"+_name;
    info += "\n编号"+temp.setNum(_id);
    info += "\n种类:"+typeName[_type];
    info += "\n等级:"+temp.setNum (_level);
    info += "\n经验:"+temp.setNum (_exp);
    info += "\n力量:"+temp.setNum (_strength);
    info += "\n生命:"+temp.setNum (_hp);
    info += "\n防御:"+temp.setNum (_defense);
    info += "\n敏捷:"+temp.setNum (_agility);
    info +='\n';

    return info;
}

QString Spirit::getImg()
{
    QString img;
    img = ":/res/" + _name + ".png";
    return img;
}
int Water::attack()
{
    int d = rand() % 10;
    int damage = _strength * d;
    //cout << "精灵" << _name << "使用湍流龙击打，打出了" << damage << "的伤害" << endl;
    return damage;
}

int Fire::attack()
{
    int d = rand() % 10;
    int damage = _strength * d;
    //cout << "精灵" << _name << "使用绝命火焰，打出了" << damage << "的伤害" << endl;
    return damage;
}

int Ele::attack()
{
    int d = rand() % 10;
    int damage = _strength * d;
    //cout << "精灵" << _name << "使用飞沙走石，打出了" << damage << "的伤害" << endl;
    return damage;
}

QDataStream &operator<<(QDataStream &stream, const Spirit &pkm)
{
    stream << pkm._name;
    stream << pkm._id;
    stream << pkm._level;
    stream << pkm._hp;
    stream << pkm._strength;
    stream << pkm._defense;
    stream << pkm._agility;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, Spirit &pkm)
{
    stream >> pkm._name;
    stream >> pkm._id;
    stream >> pkm._level;
    stream >> pkm._hp;
    stream >> pkm._strength;
    stream >> pkm._defense;
    stream >> pkm._agility;

    return stream;
}
