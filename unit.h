#ifndef UNIT_H
#define UNIT_H
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
//#include <QGraphicsSceneContextMenuEvent>
#include <QPainter>
#include <QGraphicsItem>
#include <QPoint>
#include <QWidget>
#include <QMenu>
#include <QAction>


class unit : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    unit();

    QRectF boundingRect() const;

    // overriding paint()
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

    void Disable();
    void Enable();

    // item state
    bool Pressed;
    float xi;
    float yi;
    float length = 1;
    float width = 1;
    int de_type = 0;
    int de_xnum = 0;
    int de_ynum = 0;
    QString type;
    QColor color = Qt::white;

    //units color
    QColor unit_de1_color = QColor(255, 0, 0, 127);
    QColor unit_de2_color = QColor(0, 0, 255, 127);



signals:
    void delete_this_item(unit *item);


protected:
    // overriding mouse events
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // UNIT_H
