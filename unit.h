#ifndef UNIT_H
#define UNIT_H
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsItem>
#include <QPoint>
#include <QWidget>
#include <QMenu>
#include <QAction>
//#include <QGraphicsSceneMouseEvent>
#include "merge_edit.h"
#include "dispenser_edit.h"
#include "move_edit.h"
#include "cycle_edit.h"
#include "heat_edit.h"
class unit : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    unit();

    QRectF boundingRect() const;

    // overriding paint()
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

    // item state
    bool Pressed;
    bool error;
    float xi;
    float yi;
    float actual_length = 1;
    float actual_width = 1;
    float length = 1;
    float width = 1;
    int de_type = 0;
    int de_xnum = 0;
    int de_ynum = 0;
    int tilt = 0;
    //move parameters
    int child_gap = 0;

    //dispenser parameters
    int direction = 0;
    int main_length = 0;
    int main_width = 0;
    int child_length = 0;
    int child_width = 0;

    //heater parameters
    int zigzag_linewidth;
    int zigzag_layer;
    bool clockwise;
    QString type;
    QString text;
    QColor color = Qt::white;

    //units color
    QColor unit_de1_color = QColor(255, 0, 0, 127);
    QColor unit_de2_color = QColor(0, 0, 255, 127);

public slots:
    void update_merge(QString, int, int, int, int);
    void update_dispenser(QString , int , int, int, int);
    void update_move(QString, int, int, int);
    void update_cycle(QString, int, int);
    void update_heat(QString, int , int , int , int , int );

signals:
    void delete_this_item(unit *item);


protected:
    // overriding mouse events
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // UNIT_H
