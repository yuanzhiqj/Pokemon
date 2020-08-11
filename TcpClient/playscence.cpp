#include "playscence.h"
#include "ui_playscence.h"

PlayScence::PlayScence(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayScence)
{
    ui->setupUi(this);
}

PlayScence::~PlayScence()
{
    delete ui;
}

void PlayScence::setLable(QString info,int i)
{
    switch (i) {
    case 0:
        ui->info_label1->setText(info);
        break;
    case 1:
        ui->info_label2->setText(info);
        break;
    case 2:
        ui->info_label3->setText(info);
        break;
    default:
        break;
    }

}

void PlayScence::setImage(QImage* img, int i)
{
    QImage* end = new QImage;
    *end = img->scaled(ui->img_label1->width(),ui->img_label1->height(),Qt::KeepAspectRatio);
    switch (i) {
    case 0:
        ui->img_label1->setPixmap(QPixmap::fromImage(*end));
        break;
    case 1:
        ui->img_label2->setPixmap(QPixmap::fromImage(*end));
        break;
    case 2:
        ui->img_label3->setPixmap(QPixmap::fromImage(*end));
        break;
    default:
        break;
    }

}
