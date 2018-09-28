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
    QList<line*> alllines;
    QList<line*> to_destroy;
public slots:
    void delete_from_list(line *delete_line);
    line *PassSegToTurn(line *segline);
    void MouseToSeg(line *segline, QGraphicsSceneMouseEvent *mouseEvent);

    void AddTurnline(line *turnline);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
     line *turnline;
     line *segline;
     line *segline2;
     float slope;
     bool pressed = false;
     bool same_line = false;
};

#endif // GRAPHICSSCENE_H
