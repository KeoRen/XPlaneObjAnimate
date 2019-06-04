#ifndef XPLANEOBJ8_H
#define XPLANEOBJ8_H

#include "rotationobj8.h"
#include "translationobj8.h"
#include <QStringList>
#include <QFile>
#include <iostream>
#include <QGeoCoordinate>
#include <math.h>

class XPlaneObj8
{
    private:
        QList<AnimObj8*> m_mvtList;

    public:
        XPlaneObj8();
        XPlaneObj8(QList<QGeoCoordinate> animList);
        XPlaneObj8(QList<QVector4D> animList);
        void setupData(const QStringList& lines);
        QStringList mvtList();
        QList<QVector4D> animList();
        void writeData(const QString objPath);
};

#endif
