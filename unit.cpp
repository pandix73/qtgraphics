#include "unit.h"
#include <QDebug>
extern bool deletemode;
extern bool detailmode;

//setting parameter
extern int chip_length_cm;
extern int chip_width_cm;
extern int chip_border_mm;
extern int cp_length_mm;
extern int cp_width_mm;
extern int de1_length_mm;
extern int de1_width_mm;
extern int de2_length_mm;
extern int de2_width_mm;
extern int de_spacing_um;
extern int cp_spacing_um;
extern int dispenser_mm;
extern int pix_per_brick;
unit::unit()
{
    Pressed = false;
    setFlag(ItemIsMovable);
}

QRectF unit::boundingRect() const
{
    // outer most edges
    return QRectF(xi*pix_per_brick, yi*pix_per_brick, length*pix_per_brick, width*pix_per_brick);
}

void unit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();
    QBrush brush(this->color);
    painter->setBrush(brush);
    if(Pressed)
    {
        QPen pen(Qt::lightGray, 1);
        painter->setPen(pen);
        painter->drawRect(rect);
    }
    else if (detailmode)
    {
        QPen pen(Qt::black, 1);
        painter->setPen(pen);
        if(this->type == "move"){
            for(int i = 0; i < this->de_xnum; i++){
                if(this->de_type == 1){
                    painter->setBrush(QBrush(unit_de1_color));
                    painter->drawRect(rect.x()+i*(pix_per_brick*de1_length_mm*1000/de_spacing_um + pix_per_brick),
                                      rect.y(),
                                      pix_per_brick*de1_length_mm*1000/de_spacing_um,
                                      pix_per_brick*de1_width_mm*1000/de_spacing_um);
                } else {
                    painter->setBrush(QBrush(unit_de2_color));
                    painter->drawRect(rect.x()+i*(pix_per_brick*de2_length_mm*1000/de_spacing_um + pix_per_brick),
                                      rect.y(),
                                      pix_per_brick*de2_length_mm*1000/de_spacing_um,
                                      pix_per_brick*de2_length_mm*1000/de_spacing_um);
                }
            }
        } else if (this->type == "cycle"){
            painter->setBrush(QBrush(unit_de2_color));
            for(int i = 0; i < this->de_xnum; i++){
                for(int j = 0; j < this->de_ynum; j++){
                    if(i!=0 && i!=this->de_xnum-1 && j!=0 && j!=this->de_ynum-1) continue;
                    painter->drawRect(rect.x()+i*(pix_per_brick*de2_length_mm*1000/de_spacing_um + pix_per_brick),
                                      rect.y()+j*(pix_per_brick*de2_length_mm*1000/de_spacing_um + pix_per_brick),
                                      pix_per_brick*de2_length_mm*1000/de_spacing_um,
                                      pix_per_brick*de2_length_mm*1000/de_spacing_um);
                }
            }
        } else {
            painter->drawRect(rect);
        }

    }
    else
    {
        QPen pen(Qt::black, 1);
        painter->setPen(pen);
        painter->drawRect(rect);
    }
    painter->drawText(rect, type, Qt::AlignVCenter | Qt::AlignHCenter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void unit::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Pressed = true;
    qDebug() << "unit deletemode : "<< deletemode;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void unit::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    Pressed = false;
    int xindex = qRound(this->x()/pix_per_brick);
    int yindex = qRound(this->y()/pix_per_brick);
    //this->setPos(xindex*pix_per_brick, yindex*pix_per_brick);
    this->xi += xindex;
    this->yi += yindex;
    this->setPos(0, 0);
    update();
    QGraphicsItem::mouseReleaseEvent(event);
    if(deletemode){
        emit delete_this_item(this);
    }
}
