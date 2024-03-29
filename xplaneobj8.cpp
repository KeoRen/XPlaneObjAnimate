#include "xplaneobj8.h"
#include "QDebug"

XPlaneObj8::XPlaneObj8()
{

}

XPlaneObj8::XPlaneObj8(QList<QGeoCoordinate> animList, float speed)
{
    QVector2D startPos = QVector2D(0, 0);
    startPos.setX(sin(animList[0].latitude() * M_PI / 180.0f) * RAYON_TERRE * 1000);
    startPos.setY(sin(animList[0].longitude() * M_PI / 180.0f) * RAYON_TERRE * 1000);

    QVector2D lastPos = QVector2D(0, 0);
    QVector2D pos = QVector2D(0, 0);
    float chrono = 0;

    speed = speed / 3.6f;

    for (int i = 0 ; i < animList.size() ; ++i)
    {
        TranslationObj8* mvt = new TranslationObj8("LLFFS/AnimChrono");
        mvt->newKey(QVector4D(chrono, lastPos.x(), lastPos.y(), 0));
        mvt->newKey(QVector4D(chrono + MIN_GAP, lastPos.x(), lastPos.y(), 0));
        pos.setX(sin(animList[i].latitude() * M_PI / 180.0f) * RAYON_TERRE * 1000 - startPos.x());
        pos.setY(sin(animList[i].longitude() * M_PI / 180.0f) * RAYON_TERRE * 1000 - startPos.y());
        chrono += sqrt((((pos.x() - lastPos.x()) * (pos.x() - lastPos.x())) + ((pos.y() - lastPos.y()) * (pos.y() - lastPos.y())))) / speed;
        if(isnan(chrono)) chrono = 0;
        mvt->newKey(QVector4D(chrono - MIN_GAP, pos.x(), 0, pos.y()));
        mvt->newKey(QVector4D(chrono, pos.x(), 0, pos.y()));
        m_mvtList.append(mvt);
        lastPos = pos;
    }
}

XPlaneObj8::XPlaneObj8(QList<QVector4D> animList)
{
    float chrono = 0;
    QVector2D lastPos = QVector2D(0, 0);
    float lastAngle = 0;
    int angleType = 0;
    QVector2D pos = QVector2D(0, 0);


    for (int i = 0 ; i < animList.size() ; ++i)
    {
        if (int(animList[i].x()) == 1)
        {
            TranslationObj8* mvt = new TranslationObj8("LLFFS/AnimChrono");
            mvt->newKey(QVector4D(chrono, lastPos.x(), lastPos.y(), 0));
            mvt->newKey(QVector4D(chrono + MIN_GAP, lastPos.x(), lastPos.y(), 0));
            chrono += (animList[i].y() / float(animList[i].z() / 3.6f));
            angleType = animList[i].w() / 90;
            animList[i].setW(animList[i].w() - angleType * 90);
            pos = QVector2D(cos(animList[i].w() * M_PI / 180.0f) * animList[i].y(), sin(animList[i].w() * M_PI / 180.0f) * animList[i].y());
            if (angleType == 1) pos.setY(-pos.y());
            if (angleType == 2)
            {
                pos.setX(-pos.x());
                pos.setY(-pos.y());
            }
            if (angleType == 3) pos.setX(-pos.x());
            mvt->newKey(QVector4D(chrono - MIN_GAP, lastPos.x() + pos.x(), 0, lastPos.y() + pos.y()));
            mvt->newKey(QVector4D(chrono, lastPos.x() + pos.x(), 0, lastPos.y() + pos.y()));
            lastPos = pos;
            m_mvtList.append(mvt);
        }
        else
        {
            RotationObj8* mvt = new RotationObj8(QVector3D(0, 1, 0), "LLFFS/AnimChrono");
            mvt->newKey(QVector4D(chrono, lastAngle, 0, 0));
            mvt->newKey(QVector4D(chrono + MIN_GAP, lastAngle, 0, 0));
            chrono += animList[i].w() / animList[i].z();
            lastAngle += animList[i].w();
            mvt->newKey(QVector4D(chrono - MIN_GAP, lastAngle, 0, 0));
            mvt->newKey(QVector4D(chrono, lastAngle, 0, 0));
            m_mvtList.append(mvt);
        }
    }
}

QList<QVector4D> XPlaneObj8::animList()
{
    QList<QVector4D> animList;
    float dist;
    float speed;
    float angle;
    float chrono = 0;
    int inv = 2;

    for (int i = 0 ; i < m_mvtList.size() ; ++i)
    {

        if (m_mvtList.at(i)->isTranslation())
        {
            TranslationObj8* t = static_cast<TranslationObj8*>(m_mvtList.at(i));
            for (int k = 3 ; k < 4 /* t->keyCount() */ ; ++k)
            {
                dist = sqrt((abs(t->keyPoint(k).x()) - abs(t->keyPoint(k - 2).x())) * (abs(t->keyPoint(k).x()) - abs(t->keyPoint(k - 2).x())) + (abs(t->keyPoint(k).z()) - abs(t->keyPoint(k - 2).z())) * (abs(t->keyPoint(k).z()) - abs(t->keyPoint(k - 2).z())));
                if (isnan(dist) || isinf(dist)) dist = 0;
                speed = dist / (t->keyValue(k) - chrono) * 3.6f;
                if (isnan(speed) || isinf(speed)) speed = 0;
                angle = atan(abs(abs(t->keyPoint(k).z()) - abs(t->keyPoint(k - 2).z())) / abs(abs(t->keyPoint(k).x()) - abs(t->keyPoint(k - 2).x()))) * 180.0f / M_PI;
                if (isnan(angle) || isinf(angle)) angle = 0;
                if (t->keyPoint(k).x() < t->keyPoint(k - 2).x() && inv % 2 == 0)
                {
                    if (t->keyPoint(k).z() < t->keyPoint(k - 2).z()) angle += 180;
                    else angle += 270;
                }
                else if (t->keyPoint(k).z() < t->keyPoint(k - 2).z()) angle += 90;
                chrono = t->keyValue(k);
                animList << QVector4D(1, dist, speed, angle); //Type, distance, vitesse, angle
            }
        }
        else if (m_mvtList.at(i)->isRotation())
        {
            RotationObj8* r = static_cast<RotationObj8*>(m_mvtList.at(i));
            for (int k = 3 ; k < 4 /* r->keyCount() */ ; ++k)
            {
                ++inv;
                angle = r->keyAngle(k);
                speed = abs((angle - r->keyAngle(k - 1)) / (r->keyValue(k) - chrono));
                if (isnan(speed)) speed = 0;
                chrono = r->keyValue(k);
                animList << QVector4D(0, 0, speed, angle); //Type, distance, vitesse, angle
            }
        }
    }

    return animList;
}

void XPlaneObj8::setupData(const QStringList &lines)
{
    int number = 0; //numéro de ligne

    qDebug() << lines.count() << "lignes reçues";

    while (number < lines.count())
    {
        QString lineData = lines[number].trimmed();

        if (lineData.isEmpty())
        {
            ++number;
            continue;
        }

        QStringList parse = lineData.split(QRegExp("\\s+"), QString::SkipEmptyParts);

        QList<QVariant> columnData;
        for (int i = 0 ; i < parse.count() ; ++i) columnData << parse[i];

        if (parse.at(0) == "ANIM_rotate")
        {
            QVector3D axis;
            QVector4D k1, k2;
            QString dataref;
            if (parse.size() >= 4)
            {
                axis = QVector3D(parse[1].toFloat(), parse[2].toFloat(), parse[3].toFloat());
            }
            if (parse.size() >= 8)
            {
                k1 = QVector4D(parse[6].toFloat(), parse[4].toFloat(), 0, 0);
                k2 = QVector4D(parse[7].toFloat(), parse[5].toFloat(), 0, 0);
            }
            if (parse.size() >= 9) dataref = parse.at(8);

            RotationObj8* mvt = new RotationObj8(axis, dataref);
            mvt->newKey(k1);
            mvt->newKey(k2);
            m_mvtList.append(mvt);
        }
        else if (parse.at(0) == "ANIM_trans")
        {
            QVector4D k1, k2;
            QString dataref;
            if (parse.size() >= 9)
            {
                k1 = QVector4D(parse[7].toFloat(), parse[1].toFloat(), parse[2].toFloat(), parse[3].toFloat());
                k2 = QVector4D(parse[8].toFloat(), parse[4].toFloat(), parse[5].toFloat(), parse[6].toFloat());
            }
            if (parse.size() >= 10) dataref = parse.at(9);

            if (k1.y() != 0 && k1.z() != 0 && k1.w() != 0 && k2.y() != 0 && k2.z() != 0 && k2.w() != 0)
            {
                TranslationObj8* mvt = new TranslationObj8(dataref);
                mvt->newKey(k1);
                mvt->newKey(k2);
                m_mvtList.append(mvt);
            }
        }
        else if (parse.at(0) == "ANIM_rotate_begin")
        {
            QVector3D axis;
            QString dataref;
            if (parse.size() >= 4)
            {
                axis = QVector3D(parse[1].toFloat(), parse[2].toFloat(), parse[3].toFloat());
                dataref = parse.at(4);
            }
            RotationObj8* mvt = new RotationObj8(axis, dataref);
            m_mvtList.append(mvt);
        }
        else if (parse.at(0) == "ANIM_rotate_key")
        {
            if (parse.size() >= 3)
            {
                QVector4D k(parse[1].toFloat(), parse[2].toFloat(), 0, 0);
                m_mvtList.last()->newKey(k);
            }
        }
        else if (parse.at(0) == "ANIM_trans_begin")
        {
            QString dataref;
            if (parse.size() >= 2) dataref = parse.at(1);
            TranslationObj8* mvt = new TranslationObj8(dataref);
            m_mvtList.append(mvt);
        }
        else if (parse.at(0) == "ANIM_trans_key")
        {
            if (parse.size() >= 5)
            {
                QVector4D k(parse[1].toFloat(), parse[2].toFloat(), parse[3].toFloat(), parse[4].toFloat());
                m_mvtList.last()->newKey(k);
            }
        }
        else
        {
            ++number;
            continue;
        }

        ++number ;
    }
}

QStringList XPlaneObj8::mvtList()
{
    QStringList list;

    for (int anim = 0 ; anim < m_mvtList.size() ; ++anim)
    {
        QString mvt;

        if (m_mvtList.at(anim)->isTranslation())
        {
            mvt = "T: ";
            TranslationObj8* t = static_cast<TranslationObj8*>(m_mvtList.at(anim));
            mvt += QString(" %1").arg(m_mvtList.at(anim)->dataref());
            list << mvt;

            for (int i = 1 ; i < m_mvtList.at(anim)->keyCount() ; ++i)
            {
                mvt = "> ";
                QVector3D p = t->keyPoint(i);
                mvt += QString("value %1 / point(%2,%3,%4)").arg(t->keyValue(i)).arg(p.x()).arg(p.y()).arg(p.z());
                list << mvt;
            }
        }

        if (m_mvtList.at(anim)->isRotation())
        {
            mvt = "R: ";
            RotationObj8* r = static_cast<RotationObj8*>(m_mvtList.at(anim));
            QVector3D axis = r->axis();
            mvt += QString("axis(%1,%2,%3)").arg(axis.x()).arg(axis.y()).arg(axis.z());
            mvt += QString(" %1").arg(m_mvtList.at(anim)->dataref());
            list << mvt;

            for (int i = 1 ; i < m_mvtList.at(anim)->keyCount() ; ++i)
            {
                mvt = "> ";
                mvt += QString("value %1 / angle %2").arg(r->keyValue(i)).arg(r->keyAngle(i));
                list << mvt;
            }
        }
    }

    return list;
}

void XPlaneObj8::writeData(const QString objPath)
{
    if (objPath == "") return;

    QFile file(objPath);

    if(!file.open(QIODevice::ReadWrite)) return;

    QTextStream stream(&file);
    QStringList list;
    QString tris;
    bool inch(false);
    QVector3D lastResort(0, 0, 0);

    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        if (!line.contains("ANIM") && !line.contains("TRIS")) list << line;
        if (line.contains("TRIS")) tris = line;
    }

    list << "\nANIM_begin";

    for (int anim = 0 ; anim < m_mvtList.size() ; ++anim)
    {
        QString mvt;

        if (m_mvtList.at(anim)->isTranslation())
        {
            mvt = "\n\tANIM_trans_begin";
            TranslationObj8* t = static_cast<TranslationObj8*>(m_mvtList.at(anim));
            mvt += QString(" %1").arg(m_mvtList.at(anim)->dataref());
            list << mvt;

            for (int i = 1 ; i < m_mvtList.at(anim)->keyCount() ; ++i)
            {
                mvt = "\t\tANIM_trans_key";
                QVector3D p = t->keyPoint(i);
                if (p.x() < 0.001) p.setX(0);
                if (p.y() < 0.001) p.setY(0);
                if (p.z() < 0.001) p.setZ(0);
                if (i > 2 && inch) mvt += QString(" %1 %2 %3 %4").arg(t->keyValue(i)).arg(-p.x()).arg(p.y()).arg(p.z());
                else mvt += QString(" %1 %2 %3 %4").arg(t->keyValue(i)).arg(p.x()).arg(p.y()).arg(p.z());
                list << mvt;
                lastResort = t->keyPoint(i);
            }
            mvt = "\tANIM_trans_end";
            list << mvt;
        }

        if (m_mvtList.at(anim)->isRotation())
        {
            mvt = QString("\n\tANIM_trans %1 %2 %3 %1 %2 %3 no_ref").arg(lastResort.x()).arg(lastResort.y()).arg(lastResort.z());
            mvt += "\n\tANIM_rotate_begin";
            RotationObj8* r = static_cast<RotationObj8*>(m_mvtList.at(anim));
            QVector3D axis = r->axis();
            mvt += QString(" %1 %2 %3").arg(axis.x()).arg(axis.y()).arg(axis.z());
            mvt += QString(" %1").arg(m_mvtList.at(anim)->dataref());
            list << mvt;

            for (int i = 1 ; i < m_mvtList.at(anim)->keyCount() ; ++i)
            {
                mvt = "\t\tANIM_rotate_key";
                mvt += QString(" %1 %2").arg(r->keyValue(i)).arg(r->keyAngle(i));
                list << mvt;
            }
            mvt = "\tANIM_rotate_end";
            mvt += QString("\n\tANIM_trans %1 %2 %3 %1 %2 %3 no_ref").arg(-lastResort.x()).arg(-lastResort.y()).arg(-lastResort.z());
            list << mvt;
            inch = !inch;
        }
    }

    list << tris;
    list << "\nANIM_end";

    file.resize(0);

    for (int i = 0 ; i < list.size() ; ++i)
    {
        stream << list[i] << endl;
    }

    file.close();
}
