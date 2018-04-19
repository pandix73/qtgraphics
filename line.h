#ifndef LINE_H
#define LINE_H
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsItem>
#include <QPoint>
#include <QWidget>
#include <QMenu>
#include <QAction>
class line : public QLineF
{
public:
    line();
        float x1;
        float y1;
        float x2;
        float y2;
};

#endif // LINE_H





//class unit : public QObject, public QGraphicsItem
//{
//    Q_OBJECT
//    Q_INTERFACES(QGraphicsItem)

//signals:
//    void delete_this_item(unit *item);

//protected:
//    // overriding mouse events
//    void mousePressEvent(QGraphicsSceneMouseEvent *event);
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
