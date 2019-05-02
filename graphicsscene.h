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

public slots:
    void delete_from_list(line *delete_line);
    void AddTurnline(line *head);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
     line *current_line;
     line *head;
     bool pressed = false;
     bool current_dir = false;

};

#endif // GRAPHICSSCENE_H
