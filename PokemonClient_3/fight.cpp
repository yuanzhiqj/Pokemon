#include "fight.h"
#include "ui_fight.h"
#include<QDebug>
Fight::Fight(vector<Spirit*> p_list, Spirit* enemy,QString name,QString enemyName,QWidget *parent) :
    p_list(p_list),
    enemy(enemy),
    name(name),
    enemyName(enemyName),
    QWidget(parent),
    ui(new Ui::Fight)
{
    ui->setupUi(this);
    ui->blood1->hide();
    ui->blood2->hide();
    cur = 0;
    setLabel(p_list[0]->getInfo());
    QImage* img = new QImage;
    img->load(p_list[0]->getImg());
    setImg(img);
    setLabel2(enemy->getInfo());
    //QImage* img = new QImage;
    img->load(enemy->getImg());
    setImg2(img);



    connect(ui->next_btn,&QPushButton::clicked,this,[=](){
        if(cur < p_list.size()-1)
        {
            cur++;
            setLabel(p_list[cur]->getInfo());
            QImage* img = new QImage;
            img->load(p_list[cur]->getImg());
            setImg(img);
        }
    });

    connect(ui->pre_btn,&QPushButton::clicked,this,[=](){
        if(cur > 0)
        {
            cur--;
            setLabel(p_list[cur]->getInfo());
            QImage* img = new QImage;
            img->load(p_list[cur]->getImg());
            setImg(img);
        }
    });
    connect(ui->fight_btn,&QPushButton::clicked,this,&Fight::challenge);
}

Fight::~Fight()
{
    delete ui;
}

void Fight::setLabel(QString info)
{
    ui->info_label1->setText(info);
}

void Fight::setImg(QImage* img)
{
    QImage* end = new QImage;
    *end = img->scaled(ui->img_label1->width(),ui->img_label1->height(),Qt::KeepAspectRatio);
    ui->img_label1->setPixmap(QPixmap::fromImage(*end));
}

void Fight::setLabel2(QString info)
{
    ui->info_label2->setText(info);
}

void Fight::setImg2(QImage* img)
{
    QImage* end = new QImage;
    *end = img->scaled(ui->img_label2->width(),ui->img_label2->height(),Qt::KeepAspectRatio);
    ui->img_label2->setPixmap(QPixmap::fromImage(*end));
}

void Fight::challenge()
{
    QString winer;
    QString loser;

    qDebug() << name;
    qDebug() << enemyName;
    QWidget *containwidget = new QWidget(ui->fightInfo);
    QVBoxLayout *listlayout = new QVBoxLayout;

    //血条初始化
    ui->blood1->show();
    ui->blood2->show();
    ui->blood1->setOrientation(Qt::Horizontal);
    ui->blood1->setFormat("%v");
    ui->blood1->setMaximum(p_list[cur]->getHp());
    ui->blood1->setMinimum(0);
    ui->blood1->setValue(p_list[cur]->getHp());
    ui->blood2->setOrientation(Qt::Horizontal);
    ui->blood2->setFormat("%v");
    ui->blood2->setMaximum(enemy->getHp());
    ui->blood2->setMinimum(0);
    ui->blood2->setValue(enemy->getHp());
    while(p_list[cur]->getHp() > 0 && enemy->getHp() > 0)
    {
        QString info = enemy->attack(p_list[cur]);
        ui->blood1->setValue(p_list[cur]->getHp());
        qDebug() << p_list[cur]->getHp();
        QLabel* infoLabel = new QLabel;
        QLabel* infoLabel2 = new QLabel;
        qDebug() << info;
        infoLabel->setText(info);
        listlayout->addWidget(infoLabel);
        containwidget->setLayout(listlayout);
        if(p_list[cur]->getHp() <= 0)
        {
            ui->fightInfo->setWidget(containwidget);
            qDebug() << enemy->getName() << "胜利";
            winer = enemyName;
            loser = name;
            break;
        }
        info = p_list[cur]->attack(enemy);
        ui->blood2->setValue(enemy->getHp());
        qDebug() << info;
        infoLabel2->setText(info);
        listlayout->addWidget(infoLabel2);
        containwidget->setLayout(listlayout);
        if(enemy->getHp() <= 0)
        {
            ui->fightInfo->setWidget(containwidget);
            qDebug() << p_list[cur]->getName() << "胜利";
            winer = name;
            loser = enemyName;
            break;
        }
        ui->fightInfo->setWidget(containwidget);
        QEventLoop loop;
        QTimer::singleShot(800,&loop,SLOT(quit()));
        loop.exec();
    }
    qDebug() << "胜者为" << winer << "失败者:" << loser << name << enemyName;
    p_list[cur]->recover();
    enemy->recover();
    qDebug() << "恢复后" << p_list[cur]->getHp();
    qDebug() << "恢复后" << enemy->getHp();

    if(enemyName == "")
    {
        if(winer == name)
            emit levelOver(true);
        else
            emit levelOver(false);
    }
    else
        emit fightOver(winer,loser);
}
