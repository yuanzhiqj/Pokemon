#ifndef FIGHT_H
#define FIGHT_H

#include <QWidget>
#include <QLayout>
#include <QTimer>
#include "pokemon.h"

namespace Ui {
class Fight;
}

class Fight : public QWidget
{
    Q_OBJECT

public:
    explicit Fight(vector<Spirit*> p_list, Spirit* enemy,QString name,QString enemyName,QWidget *parent = 0);
    ~Fight();

signals:
    void fightOver(QString,QString);
    void levelOver(bool);
private:
    Ui::Fight *ui;
    vector<Spirit*> p_list;
    Spirit* enemy;
    QString name;
    QString enemyName;
    int cur;

    void setLabel(QString info);
    void setImg(QImage* img);
    void setLabel2(QString info);
    void setImg2(QImage* img);

    void challenge();
};

#endif // FIGHT_H
