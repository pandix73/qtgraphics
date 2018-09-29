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
graphicsscene::graphicsscene(QObject *parent) :QGraphicsScene(parent)
{

}

line* graphicsscene::PassSegToTurn(line *seg){
    to_destroy.push_back(seg);
    this->addItem(seg);
    //connect(seg, SIGNAL(delete_this_line(line *)), this, SLOT(delete_from_list(line *)));
    qDebug() << "Add a segment" << turnline->segments << "||" << seg->x[0] << seg->y[0] << seg->x[1] << seg->y[1];
    //update data to turnline
    turnline->x[turnline->segments] = seg->x[0];
    turnline->y[turnline->segments] = seg->y[0];
    turnline->x[turnline->segments+1] = seg->x[1];
    turnline->y[turnline->segments+1] = seg->y[1];
    turnline->segments++;

    //create next segment
    line *drawline = new line();
    drawline->x[0] = turnline->x[turnline->segments];
    drawline->y[0] = turnline->y[turnline->segments];
    drawline->x[1] = turnline->x[turnline->segments];
    drawline->y[1] = turnline->y[turnline->segments];

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
        }
        else if(pressed){
            if(turnline->segments % 2 == 0){
                segline2 = PassSegToTurn(segline);
            }
            else{
                segline = PassSegToTurn(segline2);
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
        int size_to_destroy = to_destroy.count();
        for(int i=0; i<size_to_destroy; i++){
            delete to_destroy.last();
            to_destroy.pop_back();
        }
        qDebug() << "SEGMENTS " << turnline->segments;
        this->addItem(turnline);
        connect(turnline, SIGNAL(delete_this_line(line *)), this, SLOT(delete_from_list(line *)));
        alllines.append(turnline);
        pressed = false;
        qDebug() << "End a line";
    }
}

void graphicsscene::AddTurnline(line *turnline){
    this->addItem(turnline);
    connect(turnline, SIGNAL(delete_this_line(line *)), this, SLOT(delete_from_list(line *)));
    alllines.append(turnline);
    return ;
}

void graphicsscene::delete_from_list(line *delete_line)
{
    alllines.removeOne(delete_line);
    delete delete_line;
}
