#include"pokemon.h"
#include"QDebug"

Spirit::Spirit()
{

}


Spirit::Spirit(int type, QString name):
    _type(type), _name(name)
{
    _level = 0;
    _exp = 0;
    _id = id++;
    if(_type == 0)
    {
        _hp = rand() % 15 + 20;         //20~35
        _strength = rand() % 5 + 10;    //10~15
        _agility = rand() % 10 + 1;     //1~10
        _defense = rand() % 5 + 5;      //5~10
    }
    else if(_type == 1)
    {
        _hp = rand() % 15 + 30;         //30~45
        _strength = rand() % 5 + 5;     //5~10
        _agility = rand() % 10 + 1;     //1~10
        _defense = rand() % 5 + 5;      //5~10
    }
    else if(_type == 2)
    {
        _hp = rand() % 15 + 20;         //20~35
        _strength = rand() % 5 + 5;     //5~10
        _agility = rand() % 10 + 1;     //1~10
        _defense = rand() % 5 + 10;     //10~15
    }
    else if(_type == 3)
    {
        _hp = rand() % 15 + 20;         //20~35
        _strength = rand() % 5 + 5;     //5~10
        _agility = rand() % 10 + 5;     //5~15
        _defense = rand() % 5 + 5;      //5~10
    }
}

Spirit::Spirit(ATTRIBUTE *attr)
{
    this->_type = attr->type;
    this->_name = attr->name;
    this->_id = attr->id;
    this->_level = attr->level;
    this->_exp = attr->exp;
    this->_hp = attr->hp;
    this->_strength = attr->strength;
    this->_agility = attr->agility;
    this->_defense = attr->defense;
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
    if (_type == 0)
    {
        _strength += s * r;
        _hp += r;
        _defense += r;
        _agility += r;
    }
    else if (_type == 1)
    {
        _strength += r;
        _hp += s * r;
        _defense += r;
        _agility += r;
    }
    else if (_type == 2)
    {
        _strength += r;
        _hp += r;
        _defense += s * r;
        _agility += r;
    }
    else if (_type == 3)
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

ATTRIBUTE* Spirit::getAttribute()
{
    ATTRIBUTE* a = new ATTRIBUTE();
    //a->element = this->element;
    a->type = this->_type;
    a->name = this->_name;
    a->id = this->_id;
    a->level = this->_level;
    a->exp = this->_exp;
    a->hp = this->_hp;
    a->strength = this->_strength;
    a->agility = this->_agility;
    a->defense = this->_defense;

    return a;
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

QDataStream &operator<<(QDataStream &stream,ATTRIBUTE& attr)
{
    //stream << attr.element;
    stream << attr.type;
    stream << attr.name;
    stream << attr.id;
    stream << attr.level;
    stream << attr.exp;
    stream << attr.hp;
    stream << attr.strength;
    stream << attr.defense;
    stream << attr.agility;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, ATTRIBUTE& attr)
{
    //stream >> attr.element;
    stream >> attr.type;
    stream >> attr.name;
    stream >> attr.id;
    stream >> attr.level;
    stream >> attr.exp;
    stream >> attr.hp;
    stream >> attr.strength;
    stream >> attr.defense;
    stream >> attr.agility;


    return stream;
}


