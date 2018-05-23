#include "line.h"
#include <QDebug>
extern bool deletemode;
line::line()
{
//    setFlag(ItemIsMovable);
    setFlags(ItemIsSelectable);
}

QRectF line::boundingRect() const
{
    // outer most edges
    int slope = (y[1] - y[0]) / (x[1] - x[0]);
    if(slope > 1 || slope < -1){
        return QRectF(x[0], y[0], 10, y[1] - y[0]);
    }
    else{
        return QRectF(x[0], y[0], x[1] - x[0], 10);
    }


//    for(int i=1; i<this->segments; i++){
//        int slope = (y[i+1] - y[i]) / (x[i+1] - x[i]);
//        if(slope > 1 || slope < -1){
//            return QRectF(x[i], y[i], 10, y[i+1] - y[i]);
//        }
//        else{
//            return QRectF(x[i], y[i], x[i+1] - x[i], 10);
//        }
//    }
}

void line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();
    QBrush brush(Qt::gray);
    painter->setBrush(brush);
    QPen pen(Qt::black, 1);
    painter->setPen(pen);
    painter->drawRect(rect);
    update();

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void line::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "line deletemode : " << deletemode;
    QGraphicsItem::mousePressEvent(event);
}

void line::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    if(deletemode){
        qDebug() << "delete!!!" << this->x[0];
        emit delete_this_line(this);
    }
}




