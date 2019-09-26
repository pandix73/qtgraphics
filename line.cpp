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
    // outer most edges
    int startx, endx, starty, endy;
    if(x[0] != x[1]){
        if(x[0] < x[1]){
            startx = brick_x_start + pix_per_brick*((x[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
            endx = brick_x_start + pix_per_brick*((x[1]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        } else {
            startx = brick_x_start + pix_per_brick*((x[1]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
            endx = brick_x_start + pix_per_brick*((x[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        }
        starty = brick_y_start + pix_per_brick*((y[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        endy = brick_y_start + pix_per_brick*((y[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
    } else {
        if(y[0] < y[1]){
            starty = brick_y_start + pix_per_brick*((y[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
            endy = brick_y_start + pix_per_brick*((y[1]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        } else {
            starty = brick_y_start + pix_per_brick*((y[1]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
            endy = brick_y_start + pix_per_brick*((y[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        }
        startx = brick_x_start + pix_per_brick*((x[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        endx = brick_x_start + pix_per_brick*((x[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
    }
    QRectF rect = QRectF(startx - line_width_pix/2, starty - line_width_pix/2, endx - startx + line_width_pix, endy - starty + line_width_pix);
    return rect;


//    if(heater_line)line_width_pix *= 2;
//    if(std::abs(y[1] - y[0]) > std::abs(x[1] - x[0])){
//        if(y[0] < y[1])
//            rect =  QRectF(x[0] - line_width_pix/2, y[0] - line_width_pix/2, line_width_pix, y[1] - y[0] + line_width_pix);
//        else{
//            // add line_width_pix to fill the little error space at the corner
//            rect =  QRectF(x[0] - line_width_pix/2, y[1] - line_width_pix/2, line_width_pix, y[0] - y[1] + line_width_pix);
//        }
//    } else {
//        if(x[0] < x[1])
//            rect =  QRectF(x[0] - line_width_pix/2, y[0] - line_width_pix/2, x[1] - x[0] + line_width_pix, line_width_pix);
//        else{
//            // add line_width_pix to fill the little error space at the corner
//            rect =  QRectF(x[1] - line_width_pix/2, y[0] - line_width_pix/2, x[0] - x[1]+ line_width_pix, line_width_pix);
//        }
//    }
//    if(heater_line)line_width_pix /= 2;
//    return rect;

}

void line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    QRectF rect;
//    if(heater_line)line_width_pix *= 2;
//    if(std::abs(y[1] - y[0]) > std::abs(x[1] - x[0])){
//        if(y[0] < y[1])
//            rect =  QRectF(x[0] - line_width_pix/2, y[0] - line_width_pix/2, line_width_pix, y[1] - y[0] + line_width_pix);
//        else{
//            // add line_width_pix to fill the little error space at the corner
//            rect =  QRectF(x[0] - line_width_pix/2, y[1] - line_width_pix/2, line_width_pix, y[0] - y[1] + line_width_pix);
//        }
//    } else {
//        if(x[0] < x[1])
//            rect =  QRectF(x[0] - line_width_pix/2, y[0] - line_width_pix/2, x[1] - x[0] + line_width_pix, line_width_pix);
//        else{
//            // add line_width_pix to fill the little error space at the corner
//            rect =  QRectF(x[1] - line_width_pix/2, y[0] - line_width_pix/2, x[0] - x[1]+ line_width_pix, line_width_pix);
//        }
//    }
//    if(heater_line)line_width_pix /= 2;

    int startx, endx, starty, endy;
    if(x[0] != x[1]){
        if(x[0] < x[1]){
            startx = brick_x_start + pix_per_brick*((x[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
            endx = brick_x_start + pix_per_brick*((x[1]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        } else {
            startx = brick_x_start + pix_per_brick*((x[1]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
            endx = brick_x_start + pix_per_brick*((x[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        }
        starty = brick_y_start + pix_per_brick*((y[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        endy = brick_y_start + pix_per_brick*((y[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
    } else {
        if(y[0] < y[1]){
            starty = brick_y_start + pix_per_brick*((y[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
            endy = brick_y_start + pix_per_brick*((y[1]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        } else {
            starty = brick_y_start + pix_per_brick*((y[1]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
            endy = brick_y_start + pix_per_brick*((y[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        }
        startx = brick_x_start + pix_per_brick*((x[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
        endx = brick_x_start + pix_per_brick*((x[0]*(line_width_um + line_spacing_um))/(double)de_spacing_um);
    }
    QRectF rect = QRectF(startx - line_width_pix/2, starty - line_width_pix/2, endx - startx + line_width_pix, endy - starty + line_width_pix);


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
