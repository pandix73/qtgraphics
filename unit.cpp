#include "unit.h"
#include <QDebug>
extern bool deletemode;
bool detailmode = false;

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
    error = false;
    text = "";
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
                    qDebug() << "TYPE 1";
                    painter->setBrush(QBrush(unit_de1_color));
                    painter->drawRect(rect.x()+i*(pix_per_brick*de1_length_mm*1000/de_spacing_um + pix_per_brick),
                                      rect.y(),
                                      pix_per_brick*de1_length_mm*1000/de_spacing_um,
                                      pix_per_brick*de1_width_mm*1000/de_spacing_um);
                } else {
                    qDebug() << "TYPE 2";
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
        } else if (this->type == "heat"){
            painter->setBrush(QBrush(Qt::red));
            int layer = 6;
            float zigzag_width_um = 216;
            float zigzag_space_um = (de2_length_mm*1000 - 2*layer*zigzag_width_um)/2/layer;
            for(int i = 0; i < layer*2; i++){
                painter->drawRect(rect.x()+pix_per_brick*i*(zigzag_space_um+zigzag_width_um)/de_spacing_um,
                                  rect.y(),
                                  pix_per_brick*zigzag_width_um/de_spacing_um,
                                  pix_per_brick*this->width*1000/de_spacing_um);
                qDebug() << (rect.x()+pix_per_brick*i*(zigzag_space_um+zigzag_width_um)/de_spacing_um,
                             rect.y(),
                             pix_per_brick*zigzag_width_um/de_spacing_um,
                             pix_per_brick*de2_length_mm*1000/de_spacing_um);
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
    if(text == "")
        painter->drawText(rect, type, Qt::AlignVCenter | Qt::AlignHCenter);
    else
        painter->drawText(rect, text, Qt::AlignVCenter | Qt::AlignHCenter);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void unit::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!deletemode){
        Pressed = true;
        qDebug() << "unit deletemode : "<< deletemode;
        update();
        QGraphicsItem::mousePressEvent(event);
    }
}

void unit::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(!deletemode){
        Pressed = false;
        int xindex = qRound(this->x()/pix_per_brick);
        int yindex = qRound(this->y()/pix_per_brick);
        //this->setPos(xindex*pix_per_brick, yindex*pix_per_brick);
        this->xi += xindex;
        this->yi += yindex;
        this->setPos(0, 0);
        update();
    }
    QGraphicsItem::mouseReleaseEvent(event);
    if(deletemode){
        emit delete_this_item(this);
    }
}

void unit::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){
    text_edit *textedit = new text_edit(0);
    textedit->setWindowTitle("Text Edit");
    textedit->type = this->type;
    connect(textedit, SIGNAL(update_this_label(QString)), this, SLOT(update_label(QString)));
    textedit->show();
}

void unit::update_label(QString text){
    qDebug() << text;
    this->text = text;
    this->setToolTip(text);
}

