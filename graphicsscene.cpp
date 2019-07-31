#include "graphicsscene.h"
#include <QDebug>
#include <math.h>
#define myqDebug() qDebug() << fixed << qSetRealNumberPrecision(2)
extern int pix_per_brick;
extern int de_spacing_um;
extern int line_width_um;
extern int line_width_pix;
extern bool deletemode;
extern int cm_to_px;
extern int chip_width_px;
extern int line_pix_per_brick;

graphicsscene::graphicsscene(QObject *parent) :QGraphicsScene(parent)
{

}


// Start drawing line
void graphicsscene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    int tempx = int(mouseEvent->scenePos().x() / line_pix_per_brick + 0.5);
    int tempy = int(mouseEvent->scenePos().y() / line_pix_per_brick + 0.5);
    if(deletemode){
        QGraphicsScene::mousePressEvent(mouseEvent);
    }
    if(!deletemode){
        if(!pressed){                                             // start a new turnning line         
            if(mouseEvent->button() == Qt::LeftButton){
                current_line = new line();
                current_line->previous = nullptr;

                head = current_line;

                current_line->x[0] = tempx*line_pix_per_brick;
                current_line->y[0] = tempy*line_pix_per_brick;
                current_line->x[1] = current_line->x[0];
                current_line->y[1] = current_line->y[0];

                this->addItem(current_line);
                connect(current_line, SIGNAL(delete_this_line(line *)), this, SLOT(delete_from_list(line *)));

                pressed = true;
                qDebug() << "Start a new line";
            }

        } else if(pressed){
            if(mouseEvent->button() == Qt::LeftButton){
                current_line->next = new line();
                current_line->next->previous = current_line;
                current_line->next->x[0] = current_line->x[1];
                current_line->next->y[0] = current_line->y[1];
                current_line->next->x[1] = current_line->x[1];
                current_line->next->y[1] = current_line->y[1];
                current_line = current_line->next;

                this->addItem(current_line);
                connect(current_line, SIGNAL(delete_this_line(line *)), this, SLOT(delete_from_list(line *)));

            } else if(mouseEvent->button() == Qt::RightButton){
                if(current_line->previous != nullptr){
                    current_line = current_line->previous;
                    //this->removeItem(current_line->next);
                    current_line->next->deleteLater();
                    current_line->next = nullptr;
                } else {
                    this->removeItem(current_line);
                    current_line->deleteLater();
                    current_line = nullptr;
                    pressed = false;
                }
            } else if(mouseEvent->button() == Qt::MiddleButton){
                current_dir = !current_dir;
            }
        }
        qDebug() << mouseEvent->scenePos().x() << mouseEvent->scenePos().y();
    }
}

void graphicsscene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if(pressed){
        int newx = int(mouseEvent->scenePos().x() / line_pix_per_brick + 0.5)*line_pix_per_brick;
        int newy = int(mouseEvent->scenePos().y() / line_pix_per_brick + 0.5)*line_pix_per_brick;
        if(/*current_dir*/std::abs(newx - current_line->x[0]) < std::abs(newy - current_line->y[0])){
            current_line->x[1] = current_line->x[0];
            current_line->y[1] = newy;
        } else {
            current_line->x[1] = newx;
            current_line->y[1] = current_line->y[0];
        }
        this->removeItem(current_line);
        this->addItem(current_line);
        connect(current_line, SIGNAL(delete_this_line(line *)), this, SLOT(delete_from_list(line *)));
    }
}

void graphicsscene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent){
    qDebug()<<"dc "<<pressed;
    if(!deletemode && pressed && mouseEvent->buttons() == Qt::LeftButton){
        qDebug() << "finish line";
        current_line->previous->next = nullptr;
        this->removeItem(current_line);
        current_line->deleteLater();
        head->merge();
        alllines.append(head);
        pressed = false;
    }
}

void graphicsscene::AddTurnline(line *head){
    head->merge();
    line *current_seg = head;
    while(current_seg != nullptr){
        this->addItem(current_seg);
        connect(current_seg, SIGNAL(delete_this_line(line*)), this, SLOT(delete_from_list(line*)));
        current_seg = current_seg->next;
    }
    alllines.append(head);
    return;
}

void graphicsscene::delete_from_list(line *delete_line)
{
    qDebug()<<"delete"<<delete_line->x[0]<<delete_line->y[0];
    line* current_seg;
    //delete right hand side
    if(delete_line->next != nullptr){
        current_seg = delete_line->next;
        while(current_seg->next != nullptr){
            current_seg = current_seg->next;
            current_seg->previous->deleteLater();          
        }        
        current_seg->deleteLater();
    }
    //delete left hand side
    if(delete_line->previous != nullptr){
        current_seg = delete_line->previous;
        while(current_seg->previous != nullptr){
            current_seg = current_seg->previous;
            current_seg->next->deleteLater();
        }
        alllines.removeOne(current_seg);    //head
        current_seg->deleteLater();
        delete_line->deleteLater();
    } else {
        alllines.removeOne(delete_line);
        delete_line->deleteLater();
    }
    qDebug() << "num of heads" << alllines.size();
}
