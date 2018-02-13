#ifndef SVGREADER_H
#define SVGREADER_H
#include "unit.h"
#include <QList>
#include <QGraphicsRectItem>

class SvgReader
{
public:
    SvgReader();
    static QList<unit *> getElements(const QString filename);
    static QRectF getSizes(const QString filename);
};

#endif // SVGREADER_H
