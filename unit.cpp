#include "unit.h"

unit::unit()
{
    Pressed = false;
    setFlag(ItemIsMovable);
}

QRectF unit::boundingRect() const
{
    // outer most edges
    return QRectF(xi*unit_pix_per_brick, yi*unit_pix_per_brick, length*unit_pix_per_brick, width*unit_pix_per_brick);
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
    else if (unit_detailmode)
    {
        QPen pen(Qt::black, 1);
        painter->setPen(pen);
        if(this->type == "move"){
            for(int i = 0; i < this->de_xnum; i++){
                if(this->de_type == 1){
                    painter->setBrush(QBrush(unit_de1_color));
                    painter->drawRect(rect.x()+i*(unit_pix_per_brick*unit_de1_length_mm*1000/unit_de_spacing_um + unit_pix_per_brick),
                                      rect.y(),
                                      unit_pix_per_brick*unit_de1_length_mm*1000/unit_de_spacing_um,
                                      unit_pix_per_brick*unit_de1_width_mm*1000/unit_de_spacing_um);
                } else {
                    painter->setBrush(QBrush(unit_de2_color));
                    painter->drawRect(rect.x()+i*(unit_pix_per_brick*unit_de2_length_mm*1000/unit_de_spacing_um + unit_pix_per_brick),
                                      rect.y(),
                                      unit_pix_per_brick*unit_de2_length_mm*1000/unit_de_spacing_um,
                                      unit_pix_per_brick*unit_de2_length_mm*1000/unit_de_spacing_um);
                }
            }
        } else if (this->type == "cycle"){
            painter->setBrush(QBrush(unit_de2_color));
            for(int i = 0; i < this->de_xnum; i++){
                for(int j = 0; j < this->de_ynum; j++){
                    if(i!=0 && i!=this->de_xnum-1 && j!=0 && j!=this->de_ynum-1) continue;
                    painter->drawRect(rect.x()+i*(unit_pix_per_brick*unit_de2_length_mm*1000/unit_de_spacing_um + unit_pix_per_brick),
                                      rect.y()+j*(unit_pix_per_brick*unit_de2_length_mm*1000/unit_de_spacing_um + unit_pix_per_brick),
                                      unit_pix_per_brick*unit_de2_length_mm*1000/unit_de_spacing_um,
                                      unit_pix_per_brick*unit_de2_length_mm*1000/unit_de_spacing_um);
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
    update();
    QGraphicsItem::mousePressEvent(event);

}

void unit::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Pressed = false;
    int xindex = qRound(this->x()/unit_pix_per_brick);
    int yindex = qRound(this->y()/unit_pix_per_brick);
    //this->setPos(xindex*unit_pix_per_brick, yindex*unit_pix_per_brick);
    this->xi += xindex;
    this->yi += yindex;
    this->setPos(0, 0);
    update();
    QGraphicsItem::mouseReleaseEvent(event);
    if(unit_deletemode){
        emit delete_this_item(this);
    }
}

void unit::setDeleteMode(bool deletemode)
{
    unit_deletemode = deletemode;
}

