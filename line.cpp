#include "line.h"
#include <QDebug>
extern bool deletemode;
extern int line_width_pix;
extern int line_pix_per_brick;
extern bool heat_line;
line::line()
{
    setFlags(ItemIsSelectable);
    this->heater_line = heat_line;
}

QRectF line::boundingRect() const
{
    // outer most edges
    float slope = (y[1] - y[0]) / (x[1] - x[0]);
    if(slope > 1 || slope < -1){
        if(y[0] < y[1])
            return  QRectF(x[0], y[0], line_width_pix*(1+heater_line), y[1] - y[0]);
        else
            return  QRectF(x[0], y[1], line_width_pix*(1+heater_line), y[0] - y[1]);
    }
    else{
        if(x[0] < x[1])
            return  QRectF(x[0], y[0], x[1] - x[0], line_width_pix*(1+heater_line));
        else
            return  QRectF(x[1], y[0], x[0] - x[1], line_width_pix*(1+heater_line));
    }

}

void line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainterPath path;
    QRectF rect;
    if(heater_line)line_width_pix *= 2;
    float slope = (y[1] - y[0]) / (x[1] - x[0]);
    if(slope > 1 || slope < -1){
        if(y[0] < y[1])
            rect =  QRectF(x[0] - (float)line_width_pix/2, y[0] - (float)line_width_pix/2, line_width_pix, y[1] - y[0] + line_width_pix);
        else{
            // add line_width_pix to fill the little error space at the corner
            rect =  QRectF(x[0] - (float)line_width_pix/2, y[1] - (float)line_width_pix/2, line_width_pix, y[0] - y[1] + line_width_pix);
        }
    }
    else{
        if(x[0] < x[1])
            rect =  QRectF(x[0] - (float)line_width_pix/2, y[0] - (float)line_width_pix/2, x[1] - x[0] + line_width_pix, line_width_pix);
        else{
            // add line_width_pix to fill the little error space at the corner
            rect =  QRectF(x[1] - (float)line_width_pix/2, y[0] - (float)line_width_pix/2, x[0] - x[1]+ line_width_pix, line_width_pix );
        }
    }
    if(heater_line)line_width_pix /= 2;

    path.addRect(rect);
    QBrush brush(Qt::gray);
    painter->setBrush(brush);
    QPen pen(Qt::black, 0);
    painter->setPen(pen);
    painter->drawPath(path);
    update();

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void line::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    if(deletemode){
        emit delete_this_line(this);
    }
}

void line::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

void line::merge(){
    if(this->next != nullptr){
        if(int(this->next->x[1]) == int(this->x[0]) || int(this->next->y[1]) == int(this->y[0])){
            this->x[1] = this->next->x[1];
            this->y[1] = this->next->y[1];
            line* temp = this->next;
            this->next = this->next->next;
            delete(temp);
            this->merge();
        } else {
            this->next->merge();
        }
    }
}
