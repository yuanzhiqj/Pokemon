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


    QWidget *containwidget = new QWidget(ui->fightInfo);
    QVBoxLayout *listlayout = new QVBoxLayout;
    for(int i = 0; i < 100; i++)
    {
        QString test = "阿哲";
        QLabel* infoLabel = new QLabel;
        infoLabel->setText(test);
        //QHBoxLayout *rowlayout = new QHBoxLayout;
        listlayout->addWidget(infoLabel);
        containwidget->setLayout(listlayout);
        ui->fightInfo->setWidget(containwidget);
    }
    while(p_list[cur]->getHp() > 0 && enemy->getHp() > 0)
    {


        qDebug() << p_list[cur]->getHp();
        qDebug() << enemy->getHp();
        p_list[cur]->beAttack(enemy->attack());
        if(p_list[cur]->getHp() <= 0)
        {
            winer = enemyName;
            loser = name;
            break;
        }
        enemy->beAttack(p_list[cur]->attack());
        if(enemy->getHp() <= 0)
        {
            winer = name;
            loser = enemyName;
            break;
        }
    }
    qDebug() << "胜者为" << winer;
    p_list[cur]->recover();
    enemy->recover();
    qDebug() << "恢复后" << p_list[cur]->getHp();
    qDebug() << "恢复后" << enemy->getHp();

    //emit fightOver(winer,loser);
}
