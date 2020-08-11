#ifndef PLAYSCENCE_H
#define PLAYSCENCE_H

#include <QWidget>

namespace Ui {
class PlayScence;
}

class PlayScence : public QWidget
{
    Q_OBJECT

public:
    explicit PlayScence(QWidget *parent = 0);
    ~PlayScence();
    void setLable(QString info,int i);
    void setImage(QImage* img, int i);

private:
    Ui::PlayScence *ui;
};

#endif // PLAYSCENCE_H
