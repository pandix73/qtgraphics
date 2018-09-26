#include "line.h"
#include <QDebug>
extern bool deletemode;
extern int line_width_um;
extern int line_width_pix;
line::line()
{
    setFlags(ItemIsSelectable);
}

QRectF line::boundingRect() const
{
    // outer most edges
    int slope = (y[1] - y[0]) / (x[1] - x[0]);
    if(slope > 1 || slope < -1){
        if(y[0] < y[1])
            return  QRectF(x[0], y[0], line_width_pix, y[1] - y[0]);
        else
            return  QRectF(x[0], y[1], line_width_pix, y[0] - y[1]);
    }
    else{
        if(x[0] < x[1])
            return  QRectF(x[0], y[0], x[1] - x[0], line_width_pix);
        else
            return  QRectF(x[1], y[0], x[0] - x[1], line_width_pix);
    }

}

void line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainterPath path;
    QRectF rect;
    float slope = (y[1] - y[0]) / (x[1] - x[0]);
    if(slope > 1 || slope < -1){
        if(y[0] < y[1])
            rect =  QRectF(x[0], y[0], line_width_pix, y[1] - y[0]);
        else
            rect =  QRectF(x[0], y[1], line_width_pix, y[0] - y[1]);
    }
    else{
        if(x[0] < x[1])
            rect =  QRectF(x[0], y[0], x[1] - x[0], line_width_pix);
        else
            rect =  QRectF(x[1], y[0], x[0] - x[1], line_width_pix);
    }

    path.addRect(rect);
    for(int i=1; i<this->segments; i++){
        QRectF rect1;
        int slope = (y[i+1] - y[i]) / (x[i+1] - x[i]);
        if(slope > 1 || slope < -1){
            if(y[i] < y[i+1])
                rect1 =  QRectF(x[i], y[i], line_width_pix, y[i+1] - y[i]);
            else
                rect1 =  QRectF(x[i], y[i+1], line_width_pix, y[i] - y[i+1]);
        }
        else{
            if(x[i] < x[i+1])
                rect1 =  QRectF(x[i], y[i], x[i+1] - x[i], line_width_pix);
            else
                rect1 =  QRectF(x[i+1], y[i], x[i] - x[i+1], line_width_pix);
        }
        path.addRect(rect1);
    }
    QBrush brush(Qt::gray);
    painter->setBrush(brush);
    QPen pen(Qt::black, 1);
    painter->setPen(pen);
    painter->drawPath(path);
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
        qDebug() << "delete a line!!!";
        emit delete_this_line(this);
    }
}




