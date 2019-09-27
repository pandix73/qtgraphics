#include "line.h"
#include <QDebug>
#include <cmath>
extern bool deletemode;
extern int pix_per_brick;
extern int de_spacing_um;
extern int line_width_um;
extern int line_spacing_um;

extern int brick_x_start;
extern int brick_y_start;

extern int line_width_pix;
extern int line_pix_per_brick;
extern bool heat_line;
extern bool sense_line;
line::line()
{
    setFlags(ItemIsSelectable);
    this->heater_line = heat_line;
    this->sensor_line = sense_line;    
}

QRectF line::boundingRect() const
{
    int x1, x2, y1, y2;
    if(x[0] != x[1]){
        x1 = brick_x_start + pix_per_brick*(((x[0] < x[1] ? x[0] : x[1])*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        x2 = brick_x_start + pix_per_brick*(((x[0] < x[1] ? x[1] : x[0])*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        y1 = brick_y_start + pix_per_brick*((y[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        y2 = brick_y_start + pix_per_brick*((y[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
    } else {
        x1 = brick_x_start + pix_per_brick*((x[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        x2 = brick_x_start + pix_per_brick*((x[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        y1 = brick_y_start + pix_per_brick*(((y[0] < y[1] ? y[0] : y[1])*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        y2 = brick_y_start + pix_per_brick*(((y[0] < y[1] ? y[1] : y[0])*(line_width_um + line_spacing_um))/(double)de_spacing_um);
    }
    QRectF rect = QRectF(x1 - line_width_pix/2, y1 - line_width_pix/2, x2 - x1 + line_width_pix, y2 - y1 + line_width_pix);
    return rect;

}

void line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int x1, x2, y1, y2;
    if(x[0] != x[1]){
        x1 = brick_x_start + pix_per_brick*(((x[0] < x[1] ? x[0] : x[1])*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        x2 = brick_x_start + pix_per_brick*(((x[0] < x[1] ? x[1] : x[0])*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        y1 = brick_y_start + pix_per_brick*((y[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        y2 = brick_y_start + pix_per_brick*((y[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
    } else {
        x1 = brick_x_start + pix_per_brick*((x[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        x2 = brick_x_start + pix_per_brick*((x[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        y1 = brick_y_start + pix_per_brick*(((y[0] < y[1] ? y[0] : y[1])*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        y2 = brick_y_start + pix_per_brick*(((y[0] < y[1] ? y[1] : y[0])*(line_width_um + line_spacing_um))/(double)de_spacing_um);
    }
    QRectF rect = QRectF(x1 - line_width_pix/2, y1 - line_width_pix/2, x2 - x1 + line_width_pix, y2 - y1 + line_width_pix);


    QBrush brush(Qt::gray);
    painter->setBrush(brush);
    QPen pen(Qt::black, 0);
    painter->setPen(pen);
    painter->drawRect(rect);

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
    qDebug()<<this->x[0]<<this->y[0]<<this->x[1]<<this->y[1];
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
