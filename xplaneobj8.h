#ifndef XPLANEOBJ8_H
#define XPLANEOBJ8_H

#include "rotationobj8.h"
#include "translationobj8.h"
#include <QStringList>
#include <QFile>
#include <iostream>
#include <QGeoCoordinate>
#include <math.h>

#define RAYON_TERRE 6371
#define MIN_GAP 0.000001

class XPlaneObj8
{
    private:
        QList<AnimObj8*> m_mvtList;

    public:
        XPlaneObj8();
        XPlaneObj8(QList<QGeoCoordinate> animList, float speed);
        XPlaneObj8(QList<QVector4D> animList);
        void setupData(const QStringList& lines);
        QStringList mvtList();
        QList<QVector4D> animList();
        void writeData(const QString objPath);
};

#endif
