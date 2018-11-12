#include "graphicsscene.h"
#include <QDebug>
#include <math.h>
#define myqDebug() qDebug() << fixed << qSetRealNumberPrecision(2)
extern int pix_per_brick;
extern int line_width_um;
extern bool deletemode;
extern int cm_to_px;
extern int chip_width_px;

float offset;
int tempx1;
int tempy1;
int roundx1;
int roundy1;

bool create_head = true;
line *head;
line *current_seg;

graphicsscene::graphicsscene(QObject *parent) :QGraphicsScene(parent)
{

}

line* graphicsscene::PassSegToTurn(line *seg){
    if(create_head){
        head = new line();
        head = seg;
        head->previous = nullptr;
        head->next = nullptr;
        current_seg = head;
    }
    else{
        current_seg->next = seg;
        seg->previous = current_seg;
        current_seg = seg;
        current_seg->next = nullptr;
    }

    this->addItem(seg);
    connect(seg, SIGNAL(delete_this_line(line *)), this, SLOT(delete_from_list(line *)));

    turnline->segments++;

    //create next segment
    line *drawline = new line();
    drawline->x[0] = current_seg->x[1];
    drawline->y[0] = current_seg->y[1];
    drawline->x[1] = current_seg->x[1];
    drawline->y[1] = current_seg->y[1];

    return drawline;
}

void graphicsscene::MouseToSeg(line *seg, QGraphicsSceneMouseEvent *mouseEvent){
    if(abs((mouseEvent->scenePos().x() - seg->x[0])) < 0.1){
        seg->x[1] = seg->x[0];
        seg->y[1] = (float)mouseEvent->scenePos().y() - roundy1;
    }
    else{
        float slope = (mouseEvent->scenePos().y() - seg->y[0]) / (mouseEvent->scenePos().x() - seg->x[0]);
        if(slope > 1 || slope < -1){
            seg->x[1] = seg->x[0];
            seg->y[1] = mouseEvent->scenePos().y() - roundy1;
        }
        else{
            seg->x[1] = mouseEvent->scenePos().x() - roundx1;
            seg->y[1] = seg->y[0];
        }
    }
}

// Start drawing line
void graphicsscene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(deletemode){
        QGraphicsScene::mousePressEvent(mouseEvent);
    }
    if(!deletemode){
        if(!pressed){                                             // start a new turnning line
            offset = line_width_um * 0.0001 * cm_to_px/2;
            tempx1 = mouseEvent->scenePos().x() / pix_per_brick;
            tempy1 = mouseEvent->scenePos().y() / pix_per_brick;
            roundx1 = mouseEvent->scenePos().x() - tempx1*pix_per_brick + offset;
            roundy1 = mouseEvent->scenePos().y() - tempy1*pix_per_brick + offset;
            if((600 - chip_width_px)/2 % pix_per_brick != 0)
                roundx1 += pix_per_brick/2;
            line *drawline = new line();
            drawline->x[0] = mouseEvent->scenePos().x() - roundx1;
            drawline->y[0] = mouseEvent->scenePos().y() - roundy1;
            drawline->x[1] = drawline->x[0];
            drawline->y[1] = drawline->y[0];
            segline = drawline;                                                 // the first segment
            turnline = new line();
            pressed = true;
            qDebug() << "Start a new line";
            create_head = true;
        }
        else if(pressed){
            if(turnline->segments % 2 == 0){
                segline2 = PassSegToTurn(segline);
                create_head = false;
            }
            else{
                segline = PassSegToTurn(segline2);
                create_head = false;
            }
        }
    }
}

// Move the line with the cursor while drawing line
void graphicsscene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(!deletemode){
        if(pressed){
            tempx1 = mouseEvent->scenePos().x() / pix_per_brick;
            tempy1 = mouseEvent->scenePos().y() / pix_per_brick;
            roundx1 = mouseEvent->scenePos().x() - tempx1*pix_per_brick + offset;
            roundy1 = mouseEvent->scenePos().y() - tempy1*pix_per_brick + offset;
            if((600 - chip_width_px)/2 % pix_per_brick != 0)
                roundx1 -= pix_per_brick/2;
            if(turnline->segments % 2 == 0){
                MouseToSeg(segline, mouseEvent);
            }
            else{
                MouseToSeg(segline2, mouseEvent);
                //qDebug() << "B" << segline2->x[0] << segline2->y[0] << segline2->x[1] << segline2->y[1];
            }
        }
    }
}

void graphicsscene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if(!deletemode){
        qDebug() << "SEGMENTS " << turnline->segments;
        head->merge();
        alllines.append(head);
        pressed = false;
        create_head = true;
    }
}

//TODO:
void graphicsscene::AddTurnline(line *head){
    head->merge();
    line *current_seg = head;
    while(current_seg->next != nullptr){
        this->addItem(current_seg);
        current_seg = current_seg->next;
        connect(current_seg, SIGNAL(delete_this_line(line*)), this, SLOT(delete_from_list(line*)));
    }
    this->addItem(current_seg);
    connect(current_seg, SIGNAL(delete_this_line(line*)), this, SLOT(delete_from_list(line*)));
    alllines.append(head);
    return;
}

void graphicsscene::delete_from_list(line *delete_line)
{
    qDebug()<<delete_line->x[0]<<delete_line->y[0];
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
