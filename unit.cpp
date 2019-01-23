#include "unit.h"
#include <QDebug>
extern bool deletemode;
bool detailmode = false;

//setting parameter
extern int chip_border_mm;
extern int cp_length_mm;
extern int cp_width_mm;
extern int de1_length_um;
extern int de1_width_um;
extern int de2_length_um;
extern int de2_width_um;
extern int de_spacing_um;
extern int cp_spacing_um;
extern int dispenser_mm;
extern int pix_per_brick;
unit::unit()
{
    Pressed = false;
    error = false;
    text = "";
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
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
        QPen pen(Qt::lightGray, 0);
        painter->setPen(pen);
        painter->drawRect(rect);
    }
    else if (detailmode)
    {
        QPen pen(Qt::black, 0);
        painter->setPen(pen);
        if(this->type == "move"){
            for(int i = 0; i < this->de_xnum; i++){
                if(this->de_type == 1){
                    painter->setBrush(QBrush(unit_de1_color));
                    painter->drawRect(rect.x()+i*(pix_per_brick*de1_length_um/de_spacing_um + pix_per_brick),
                                      rect.y(),
                                      pix_per_brick*de1_length_um/de_spacing_um,
                                      pix_per_brick*de1_width_um/de_spacing_um);
                } else {
                    painter->setBrush(QBrush(unit_de2_color));
                    painter->drawRect(rect.x()+i*(pix_per_brick*de2_length_um/de_spacing_um + pix_per_brick),
                                      rect.y(),
                                      pix_per_brick*de2_length_um/de_spacing_um,
                                      pix_per_brick*de2_width_um/de_spacing_um);
                }
            }
        } else if (this->type == "cycle"){
            painter->setBrush(QBrush(unit_de2_color));
            for(int i = 0; i < this->de_xnum; i++){
                for(int j = 0; j < this->de_ynum; j++){
                    if(i!=0 && i!=this->de_xnum-1 && j!=0 && j!=this->de_ynum-1) continue;
                    painter->drawRect(rect.x()+i*(pix_per_brick*de2_length_um/de_spacing_um + pix_per_brick),
                                      rect.y()+j*(pix_per_brick*de2_width_um/de_spacing_um + pix_per_brick),
                                      pix_per_brick*de2_length_um/de_spacing_um,
                                      pix_per_brick*de2_width_um/de_spacing_um);
                }
            }
        } else if (this->type == "heat"){
            painter->setBrush(QBrush(Qt::red));
            int layer = 6;
            float zigzag_width_um = 216;
            float zigzag_space_um = (de2_length_um - 2*layer*zigzag_width_um)/2/layer;
            for(int i = 0; i < layer*2; i++){
                painter->drawRect(rect.x()+pix_per_brick*i*(zigzag_space_um+zigzag_width_um)/de_spacing_um,
                                  rect.y(),
                                  pix_per_brick*zigzag_width_um/de_spacing_um,
                                  pix_per_brick*this->width*1000/de_spacing_um);
            }


        } else if(this->type == "sensor"){
            painter->drawRect(rect);

        } else {
            painter->drawRect(rect);
        }

    }
    else
    {
        QPen pen(Qt::black, 0);
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
    if(this->type == "merge"){
        merge_edit *edit = new merge_edit(this->text, this->actual_length, this->actual_width, 0);
        edit->setWindowTitle("Merge Edit");
        connect(edit, SIGNAL(update_this_label(QString, int, int)), this, SLOT(update_merge(QString, int , int)));
        edit->show();
    }else if(this->type == "dispenser"){
        dispenser_edit *edit = new dispenser_edit(this->text, this->actual_length, this->actual_width, 0);
        edit->setWindowTitle("Dispener Edit");
        connect(edit, SIGNAL(update_this_label(QString, int, int)), this, SLOT(update_dispenser(QString, int , int)));
        edit->show();
    }else if(this->type == "move"){
        move_edit *edit = new move_edit(this->text, this->de_xnum, this->tilt, this->de_type, 0);
        edit->setWindowTitle("Move Edit");
        connect(edit, SIGNAL(update_this_label(QString, int, int, int)), this, SLOT(update_move(QString, int , int, int)));
        edit->show();
    }else if(this->type == "cycle"){
        cycle_edit *edit = new cycle_edit(this->text, this->de_xnum, this->de_ynum, 0);
        connect(edit, SIGNAL(update_this_label(QString, int, int)), this, SLOT(update_cycle(QString, int, int)));
        edit->show();
    }else if(this->type == "heat"){
        heat_edit *edit = new heat_edit(this->text, this->de_xnum, this->zigzag_linewidth, this->zigzag_layer, this->tilt, this->clockwise, 0);
        connect(edit, SIGNAL(update_this_label(QString, int, int, int, int, int)), this, SLOT(update_heat(QString, int, int, int, int, int)));
        edit->show();
    }

}

void unit::update_merge(QString text, int a, int b){
    this->length = a;
    this->width = b;
    this->text = text;
}

void unit::update_dispenser(QString text, int a, int b){
    this->length = a;
    this->width = b;
    this->text = text;
}

void unit::update_move(QString text, int num, int tilt, int de_type){
    this->tilt = tilt;
    this->de_type = de_type;
    if(tilt == 90){
        this->de_xnum = 1;
        this->de_ynum = num;
        if(de_type == 1){
            this->length = de1_width_um/de_spacing_um;
            this->width = num*de1_length_um/de_spacing_um + num-1;
        }else{
            this->length = de2_width_um/de_spacing_um;
            this->width = num*de2_length_um/de_spacing_um + num-1;
        }
    }else{
        this->de_xnum = num;
        this->de_ynum = 1;
        if(de_type == 1){
            this->length = num*de1_length_um/de_spacing_um + num-1;
            this->width = de1_width_um/de_spacing_um;
        }else{
            this->length = num*de2_length_um/de_spacing_um + num-1;
            this->width = de2_width_um/de_spacing_um;
        }
    }
    this->text = text;
}

void unit::update_cycle(QString text, int a, int b){
    this->de_xnum = a;
    this->de_ynum = b;
    this->length = a*de2_length_um/de_spacing_um + a-1;
    this->width = b*de2_length_um/de_spacing_um + b-1;
    this->text = text;
}

void unit::update_heat(QString text, int width, int linewidth, int layer, int tilt, int clockwise)
{
    this->de_xnum = width;
    this->zigzag_linewidth = linewidth;
    this->zigzag_layer = layer;
    this->tilt = tilt;
    this->clockwise = clockwise;
    this->length = 1+((4*layer-1)*linewidth-1)/de_spacing_um;
    this->width = 1000*width/de_spacing_um;
    if(tilt == 90 || tilt == 270) std::swap(this->length, this->width);
    this->text = text;
}
