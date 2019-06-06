
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
#include <QFrame>
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
    cout << "dist : " << ActionListe[lastchoice].y() << endl;
    label->hide();
    dist->setValue(0);
    dist->hide();
    dist->blockSignals(true);
    label->setText("Distance (m) : ");

    cout << "vit : " << ActionListe[lastchoice].z() << endl;

    label_2->show();
    vit->setValue( double(ActionListe[lastchoice].z()));
    vit->show();
    vit->blockSignals(false);
    label_2->setText("Vitesse (°/s) : ");

    cout << "Angle : " << ActionListe[lastchoice].w() << endl;

    label_3->show();
    angle->setValue( double(ActionListe[lastchoice].w()));
    angle->show();
    angle->blockSignals(false);
    label_3->setText("Angle (°) : ");

    enregistrer->show();
    supprimer->show();
}


void animation::translation()
{
    cout << "dist : " << ActionListe[lastchoice].y() << endl;
    label->show();
    dist->setValue( double (ActionListe[lastchoice].y()));
    dist->show();
    dist->blockSignals(false);
    label->setText("Distance (m) : ");

    cout << "vit : " << ActionListe[lastchoice].z() << endl;
    label_2->show();
    vit->setValue( double (ActionListe[lastchoice].z()));
    vit->show();
    vit->blockSignals(false);
    label_2->setText("Vitesse (km/h) : ");

    label_3->show();
    angle->setValue( double (ActionListe[lastchoice].w()));
    angle->show();
    angle->blockSignals(false);
    label_3->setText("Angle (°) : ");

    enregistrer->show();
    supprimer->show();
}

void animation::setDist(double value)
{
    ActionListe[lastchoice].setY( float (value));
}

void animation::setVit(double value)
{

    ActionListe[lastchoice].setZ(float (value));
}


void animation::setAngle(double value)
{

    ActionListe[lastchoice].setW(float (value));
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
    RectListe.append(QRect(480, 100 + 100*RectListe.length(), 120,50));/*coordonné du rectangle*/
    if(lastchoice > -1)
    {
        ActionListe.append(ActionListe.last());
        for (int i = ActionListe.length() - 1; i > lastchoice; i--) {
            ActionListe[i] = ActionListe[i-1];
        }
        ActionListe[lastchoice] = QVector4D(type,0,0,0);
    }
    else ActionListe.append(QVector4D(type,0,0,0));
    lastchoice = -1;
    update();
}

void animation::draw(QPainter& paint)

{
    if(ActionListe.length() <= 4 )
    {
        rectScroll->hide();

        for (int x = 0; x < RectListe.length(); x++) { /*boucle de la longueur du tableau qui à chaque boucle affiche le rectangle voulu*/
            paint.setPen(Qt::blue);
            if(x == lastchoice) paint.setPen(Qt::green);

            paint.drawRect(RectListe[x]);
            if( ActionListe[x].x() == 0.0f) paint.drawText(RectListe[x].x() + 10, RectListe[x].y() + 25, QString::number(x) + " : Rotation");

            else paint.drawText(RectListe[x].x() + 10, RectListe[x].y() + 25, QString::number(x) +  " : Translation");
         }
        paint.setPen(Qt::blue);
        if( RectListe.length()>= 2)
        {
            for (int x = 1;x<RectListe.length();x++) {
               paint.drawLine( 540, 50+100*x,540, 100+100*x);
            }
        }
    }
    if(ActionListe.length() > 4)
    {
        rectScroll->show();
        int limit = rectScroll->value();
        for (int x = 0; x < 4;x++) {
            paint.setPen(Qt::blue);
            int y = limit + x;
            if(y == lastchoice) paint.setPen(Qt::green);

            paint.drawRect(RectListe[x]);
            if( ActionListe[y].x() == 0.0f) paint.drawText(RectListe[x].x() + 10, RectListe[x].y() + 25, QString::number(y) + " : Rotation");

            else paint.drawText(RectListe[x].x() + 10, RectListe[x].y() + 25, QString::number(y) +  " : Translation");
        }
        if( RectListe.length()>= 2)
        {
            for (int x = 1;x<4;x++) {
               paint.drawLine( 540, 50+100*x,540, 100+100*x);
            }
        }
    }


}

void animation::paintEvent(QPaintEvent *)
{
    if(ActionListe.length() > 4) rectScroll->setMaximum(ActionListe.length()-4);

    QPainter paint(this);

    paint.setFont(QFont("Arial", 12));
    draw(paint);
}

void animation::supression()
{
    if(lastchoice != -1 )
    {
        ActionListe.removeAt(lastchoice);
        RectListe.removeLast();
        update();
        lastchoice = -1;
    }
}


void animation::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint point;
    point.setX(event->x());
    point.setY(event->y());

    for (int n=0;n<RectListe.length();n++) {
        if(  point.x() >=  RectListe[n].x() && point.x() <= RectListe[n].topRight().x() && point.y() >= RectListe[n].y() &&  point.y() <= RectListe[n].bottomLeft().y() )
        {

            lastchoice = n + rectScroll->value();
            cout << "bloc n°" << lastchoice << " type : " << ActionListe[lastchoice].x() << endl;
            if(ActionListe[lastchoice].x() == 1.0f) translation();
            else rotation();
            break;
        }
    }
    update();
}


void animation::save()
{
    for (int i = 0; i< ActionListe.length();i++) {
        qDebug() << ActionListe[i];
    }
    XPlaneObj8 animList(ActionListe);


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


        ActionListe = anim.animList();
        RectListe.clear();
        for (int i = 0; i< ActionListe.length();i++) {
            RectListe.append(QRect(500, 100 + 100*RectListe.length(), 100,50));
        }

        update();
        for (int i = 0; i< ActionListe.length();i++) {
            qDebug() << ActionListe[i];
        }
    if(!s.toString().isNull()) edtFile->setText(s.toString());
}

