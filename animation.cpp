
#include "animation.h"
#include <QApplication>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QInputDialog>
#include <QIODevice>
#include <QDebug>
#include <QMessageBox>
#include <QTextCodec>
#include <QString>
#include <iostream>
#include <string>
#include <fstream>
#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include <QRect>
#include <QDoubleSpinBox>
#include <QFile>
#include <QScrollBar>

using namespace std;



animation::animation(QWidget *parent) :
    QMainWindow(parent)

{
   setupUi(this);
this->setFixedSize(992,625);

    v_rotation = 0.0f;

    label->hide();
    label_2->hide();
    label_3->hide();

    dist->hide();
    angle->hide();
    vit->hide();


    lastchoice = -1;

    enregistrer->hide();
    supprimer->hide();

    dist->clear();
    angle->clear();
    vit->clear();
    edtFile->setText(QDir::homePath());
    enregistrer->setEnabled(true);


    connect(rot, SIGNAL(clicked()), this, SLOT(NewRotation()));
    connect(trans, SIGNAL(clicked()), this, SLOT(NewTranslation()));
    connect(supprimer,SIGNAL(clicked()),this,SLOT(supression()));
    connect(enregistrer,SIGNAL(clicked()), this, SLOT(save()));
    connect(fichier, SIGNAL(clicked()), this, SLOT(getFile()));
    connect(dist,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,&animation::setDist );
    connect(angle,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,&animation::setAngle );
    connect(vit,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,&animation::setVit );
    connect(rectScroll,SIGNAL(sliderMoved), this,SLOT(update()));


}



void animation::rotation()
{
    cout << "dist : " << ActionList[lastchoice].y() << endl;
    label->hide();
    dist->setValue(0);
    dist->hide();
    dist->blockSignals(true);
    label->setText("Distance (m) : ");

    cout << "vit : " << ActionList[lastchoice].z() << endl;

    label_2->show();
    vit->setValue( double(ActionList[lastchoice].z()));
    vit->show();
    vit->blockSignals(false);
    label_2->setText("Vitesse (°/s) : ");

    cout << "Angle : " << ActionList[lastchoice].w() << endl;

    label_3->show();
    angle->setValue( double(ActionList[lastchoice].w()));
    angle->show();
    angle->blockSignals(false);
    label_3->setText("Angle (°) : ");

    enregistrer->show();
    supprimer->show();
}


void animation::translation()
{
    cout << "dist : " << ActionList[lastchoice].y() << endl;
    label->show();
    dist->setValue( double (ActionList[lastchoice].y()));
    dist->show();
    dist->blockSignals(false);
    label->setText("Distance (m) : ");

    cout << "vit : " << ActionList[lastchoice].z() << endl;
    label_2->show();

    vit->setValue( double (ActionList[lastchoice].z()));
    vit->show();
    vit->blockSignals(false);
    label_2->setText("Vitesse (km/h) : ");

    label_3->show();
    angle->setValue( double (ActionList[lastchoice].w()));
    angle->show();
    angle->blockSignals(false);
    label_3->setText("Angle (°) : ");

    enregistrer->show();
    supprimer->show();
}

void animation::setDist(double value)
{
    ActionList[lastchoice].setY( float (value));
}

void animation::setVit(double value)
{

    ActionList[lastchoice].setZ(float (value));
}


void animation::setAngle(double value)
{

    ActionList[lastchoice].setW(float (value));
}

void animation::NewRotation()
{
    NewDraw(0.0f);
}

void animation::NewTranslation()
{
    NewDraw(1.0f);
}


void animation::NewDraw(float type)
{
    RectList.append(QRect(480, 100 + 100*RectList.length(), 120,50));/*coordonné du rectangle*/
    if(lastchoice > -1)
    {
        ActionList.append(ActionList.last());
        for (int i = ActionList.length() - 1; i > lastchoice; i--) {
            ActionList[i] = ActionList[i-1];
        }
        ActionList[lastchoice] = QVector4D(type,0,0,0);
    }
    else ActionList.append(QVector4D(type,0,0,0));
    lastchoice = -1;
    update();
}

void animation::draw(QPainter& paint)

{
    if(ActionList.length() <= 4 )
    {
        rectScroll->hide();

        for (int x = 0; x < RectList.length(); x++) { /*boucle de la longueur du tableau qui à chaque boucle affiche le rectangle voulu*/
            paint.setPen(Qt::blue);
            if(x == lastchoice) paint.setPen(Qt::green);

            paint.drawRect(RectList[x]);
            if( ActionList[x].x() == 0.0f) paint.drawText(RectList[x].x() + 10, RectList[x].y() + 25, QString::number(x) + " : Rotation");

            else paint.drawText(RectList[x].x() + 10, RectList[x].y() + 25, QString::number(x) +  " : Translation");
         }
        paint.setPen(Qt::blue);
        if( RectList.length()>= 2)
        {
            for (int x = 1;x<RectList.length();x++) {
               paint.drawLine( 540, 50+100*x,540, 100+100*x);
            }
        }
    }
    if(ActionList.length() > 4)
    {
        rectScroll->show();
        int limit = rectScroll->value();
        for (int x = 0; x < 4;x++) {
            paint.setPen(Qt::blue);
            int y = limit + x;
            if(y == lastchoice) paint.setPen(Qt::green);

            paint.drawRect(RectList[x]);
            if( ActionList[y].x() == 0.0f) paint.drawText(RectList[x].x() + 10, RectList[x].y() + 25, QString::number(y) + " : Rotation");

            else paint.drawText(RectList[x].x() + 10, RectList[x].y() + 25, QString::number(y) +  " : Translation");
        }
        if( RectList.length()>= 2)
        {
            for (int x = 1;x<4;x++) {
               paint.drawLine( 540, 50+100*x,540, 100+100*x);
            }
        }
    }


}

void animation::paintEvent(QPaintEvent *)
{
    if(ActionList.length() > 4) rectScroll->setMaximum(ActionList.length()-4);

    QPainter paint(this);

    paint.setFont(QFont("Arial", 12));
    draw(paint);
}

void animation::supression()
{
    if(lastchoice != -1 )
    {
        ActionList.removeAt(lastchoice);
        RectList.removeLast();
        update();
        lastchoice = -1;
    }
}


void animation::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint point;
    point.setX(event->x());
    point.setY(event->y());

    for (int n=0;n<RectList.length();n++) {
        if(  point.x() >=  RectList[n].x() && point.x() <= RectList[n].topRight().x() && point.y() >= RectList[n].y() &&  point.y() <= RectList[n].bottomLeft().y() )
        {
            if(ActionList.length()>4) {
                lastchoice = n + rectScroll->value();
            }
            else {
                lastchoice = n;
            }
            cout << "bloc n°" << lastchoice << " type : " << ActionList[lastchoice].x() << endl;
            if(ActionList[lastchoice].x() == 1.0f) translation();
            else rotation();
            break;
        }
    }
    update();
}


void animation::save()
{
    for (int i = 0; i< ActionList.length();i++) {
        qDebug() << ActionList[i];
    }
    XPlaneObj8 animList(ActionList);


    QUrl s = QFileDialog::getSaveFileName(
        this,
        "Fichier OBJ"
    );

    QString SFile = s.toString();

    qDebug() << SFile;
    animList.writeData(SFile);
}

void animation::getFile()
{
    QUrl s = QFileDialog::getOpenFileUrl(
        this,
        "Fichier OBJ"
    );

    XPlaneObj8  anim;
    QString temp = s.toString();
    QString SFile;

    for (int x = 7; x < temp.length(); x++) {
        SFile[x-7] = temp[x];

    }

    QFile file(SFile);
        QStringList lines;

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            std::cout  << "read only" << std::endl;
            return ;
        }

        lines << file.readAll();

        lines = lines[0].split(QRegExp("\n"), QString::SkipEmptyParts);

        std::cout << "test" << std::endl;


        anim.setupData(lines);


        ActionList = anim.animList();
        RectList.clear();
        for (int i = 0; i< ActionList.length();i++) {
            RectList.append(QRect(480, 100 + 100*RectList.length(), 120,50));
        }

        update();
        for (int i = 0; i< ActionList.length();i++) {
            qDebug() << ActionList[i];
        }
    if(!s.toString().isNull()) edtFile->setText(s.toString());
}

