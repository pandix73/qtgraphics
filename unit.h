#ifndef UNIT_H
#define UNIT_H
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsItem>
#include <QPoint>
#include <QWidget>


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
    int xi;
    int yi;
    int length = 1;
    int width = 1;
    int de_type = 0;
    int de_xnum = 0;
    int de_ynum = 0;
    QString type;
    QColor color = Qt::white;

//    bool unit_deletemode = false;
//    bool unit_detailmode = false;

//    //setting parameter
//    int unit_chip_length_cm = 3;
//    int unit_chip_width_cm = 2;
//    int unit_chip_border_mm = 3;
//    int unit_cp_length_mm = 2;
//    int unit_cp_width_mm = 4;
//    int unit_de1_length_mm = 1;
//    int unit_de1_width_mm = 5;
//    int unit_de2_length_mm = 3;
//    int unit_de_spacing_um = 500;
//    int unit_cp_spacing_um = 300;
//    int unit_dispenser_mm = 10;
//    int unit_pix_per_brick;

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
