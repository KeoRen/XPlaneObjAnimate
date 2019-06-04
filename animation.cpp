
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

using namespace std;



animation::animation(QWidget *parent) :
    QMainWindow(parent)

{
   setupUi(this);

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
    connect(fichier, SIGNAL(clicked()), this, SLOT(getDir()));
    connect(dist,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,&animation::setDist );
    connect(angle,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,&animation::setAngle );
    connect(vit,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,&animation::setVit );


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
    label_2->setText("Vitesse (km/h) : ");

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
    RectListe.append(QRect(500, 100 + 100*RectListe.length(), 100,50));/*coordonné du rectangle*/
    ActionListe.append(QVector4D(type,0,0,0));

    update();
}

void animation::draw(QPainter& paint)

{
    for (int x=0;x<RectListe.length();x++) { /*boucle de la longueur du tableau qui à chaque boucle affiche le rectangle voulu*/
        paint.drawRect(RectListe[x]);
        if( ActionListe[x].x() == 0.0f) paint.drawText(RectListe[x].x() + 10, RectListe[x].y() + 25,"Rotation");

        else paint.drawText(RectListe[x].x() + 10, RectListe[x].y() + 25,"Translation");
     }
    if( RectListe.length()>= 2)
    {
        for (int x = 1;x<RectListe.length();x++) {
           paint.drawLine( 550, 50+100*x,550, 100+100*x);
        }
    }

}

void animation::paintEvent(QPaintEvent *)
{
    QPainter paint(this);

    paint.setPen(Qt::blue);
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

            lastchoice = n;
            cout << "bloc n°" << lastchoice << " type : " << ActionListe[lastchoice].x() << endl;
            if(ActionListe[n].x() == 1.0f) translation();
            else rotation();
            break;
        }
    }
}


void animation::save()
{
    XPlaneObj8 animList(ActionListe);
    animList.writeData("./test.obj");
}

void animation::getDir()
{
    QUrl s = QFileDialog::getOpenFileUrl(
        this,
        "Dossier racine...",
        edtFile->text()
    );

    XPlaneObj8  anim;
    QString temp = s.toString();
    QString SFile;

    for (int x = 7; x < temp.length(); x++) {
        SFile[x-7] = temp[x];

    }

    QFile file("/home/keo/deer_buck.obj");
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
        cout << ActionListe.length() << endl;
        ActionListe.append(QVector4D(1,0,0,0));
        ActionListe.append(QVector4D(1,0,0,0));
        ActionListe.append(QVector4D(0,0,0,0));
        ActionListe.append(QVector4D(0,0,0,0));

        for (int var = 0; var < ActionListe.length(); ++var) {
            NewDraw(ActionListe[var].x());

        }

    if(!s.toString().isNull()) edtFile->setText(s.toString());
}
