#ifndef ANIMOBJ8_H
#define ANIMOBJ8_H

#include <QString>
#include <QList>
#include <QVector3D>
#include <QVector4D>

class AnimObj8
{
    private:
        bool m_isTranslation;
        bool m_isRotation;
        QString m_dataref;
        QList<QVector4D> keys;

    protected:
        void setTranslation(bool v = true) { m_isTranslation = v; }
        void setRotation(bool v = true) { m_isRotation = v; }
        void setDataref(const QString& s) { m_dataref = s; }

        QVector4D key(int index) const
        {
            if (index < keys.count()) return keys[index];
            return QVector4D();
        }

    public:
        AnimObj8() : m_isTranslation(false), m_isRotation(false) {}

        bool isTranslation() const { return m_isTranslation; }
        bool isRotation() const { return m_isRotation; }

        QString dataref() const { return m_dataref; }

        int keyCount() const { return keys.size(); }

        float keyValue(int index) const
        {
            if (index) return key(index).x();
            return 0;
        }

        void newKey(QVector4D v) { keys << v; }
};

#endif
