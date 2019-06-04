#include "animation.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    animation w;

    w.setWindowTitle("XplaneObjAnimate (gestion des commandes d'animation)");
    w.show();

    return a.exec();
}
