#ifndef TRANSLATIONOBJ8_H
#define TRANSLATIONOBJ8_H

#include "animobj8.h"

class TranslationObj8 : public AnimObj8
{
    public:
        TranslationObj8(const QString& dref = QString())
        {
            setTranslation();
            newKey(QVector4D());
            setDataref(dref);
        }

        QVector3D keyPoint(int index) const
        {
            if (index == 0) return QVector3D();
            QVector4D k = key(index);
            return QVector3D(k.y(), k.z(), k.w());
        }

};

#endif
