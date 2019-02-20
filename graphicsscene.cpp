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
int tempx;
int tempy;
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

    qDebug() << seg->x[0] << " " << seg->x[1] << " " << seg->y[0] << " " << seg->y[1];
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
        if(slope > 1 || slope < -1){    //vertical line
            seg->x[1] = seg->x[0];
            //find the closest grid
            if(roundy1 < round)
                seg->y[1] = tempy*line_pix_per_brick;
            else
                seg->y[1] = (tempy+1)*line_pix_per_brick;
        }
        else{                           //horizontal line
            //find the closest grid
            if(roundx1 < round)
                seg->x[1] = tempx*line_pix_per_brick;
            else
                seg->x[1] = (tempx+1)*line_pix_per_brick;
            seg->y[1] = seg->y[0] ;
        }
    }
}

// Start drawing line
void graphicsscene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    round = line_pix_per_brick / 2;
    tempx = mouseEvent->scenePos().x() / line_pix_per_brick;
    tempy = mouseEvent->scenePos().y() / line_pix_per_brick;
    roundx1 = mouseEvent->scenePos().x() - tempx*line_pix_per_brick;
    roundy1 = mouseEvent->scenePos().y() - tempy*line_pix_per_brick;
    if(deletemode){
        QGraphicsScene::mousePressEvent(mouseEvent);
    }
    if(!deletemode){
        if(!pressed){                                             // start a new turnning line         
            line *drawline = new line();
            if(roundx1 < round)
                drawline->x[0] = tempx*line_pix_per_brick;
            else
                drawline->x[0] = (tempx+1)*line_pix_per_brick;


            if(roundy1 < round)
                drawline->y[0] = tempy*line_pix_per_brick;
            else
                drawline->y[0] = (tempy+1)*line_pix_per_brick;

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
                MouseToSeg(segline, mouseEvent);
                segline2 = PassSegToTurn(segline);
                create_head = false;
            }
            else{
                MouseToSeg(segline2, mouseEvent);
                segline = PassSegToTurn(segline2);
                create_head = false;
            }
        }
        qDebug() << mouseEvent->scenePos().x() << mouseEvent->scenePos().y();
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

void graphicsscene::AddTurnline(line *head){
    head->merge();
    line *current_seg = (head);
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
