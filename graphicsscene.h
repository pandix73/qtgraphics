#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H
#include "unit.h"
#include "line.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QLineF>
class QGraphicsLineItem;
class QGraphicsSceneMouseEvent;

class graphicsscene : public QGraphicsScene
{
    Q_OBJECT
public:
    graphicsscene(QObject *parent);
    QList<QLineF*> alllines;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
private:
     QGraphicsLineItem *myline;
     QGraphicsLineItem *turnline;
     float slope;
     bool pressed = false;
};

#endif // GRAPHICSSCENE_H
