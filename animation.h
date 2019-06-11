#ifndef ANIMATION_H
#define ANIMATION_H

#include <QWidget>
#include "ui_animation.h"
#include <QList>
#include <QPainter>
#include <QVector4D>
#include <xplaneobj8.h>


enum ActionType { rotation, translation};

namespace Ui {
class animation;
}

class animation : public QMainWindow, private Ui::animation
{
    Q_OBJECT

public:
    explicit animation(QWidget *parent = nullptr);
    int time;

private:
     QString ecrit;
     void initTable();
     QList<QRect> RectList;
     int lastchoice;
     QList<QVector4D> ActionList;
     float v_rotation;

public slots:

   void rotation();
   void translation();
   void NewDraw(float type);
   void NewRotation();
   void NewTranslation();
   void draw(QPainter& paint);
   void paintEvent(QPaintEvent *);
   void mouseReleaseEvent(QMouseEvent *event);
   void supression();
   void setDist(double value);
   void setAngle(double value);
   void setVit(double value);
   void save();
   void getFile();


};

#endif
