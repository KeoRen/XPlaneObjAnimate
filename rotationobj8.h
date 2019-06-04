#ifndef ROTATIONOBJ8_H
#define ROTATIONOBJ8_H

#include "animobj8.h"

class RotationObj8 : public AnimObj8
{
    public:
        RotationObj8(QVector3D axis, const QString&dref = QString())
        {
            setRotation();
            newKey(axis.toVector4D());
            setDataref(dref);
        }
        QVector3D axis() const { return key(0).toVector3D(); }

        float keyAngle(int index) const
        {
            if (index == 0) return 0.0;
            return key(index).y();
        }
};

#endif
