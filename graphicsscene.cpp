#include "graphicsscene.h"
#include <QDebug>
#include <math.h>
extern int pix_per_brick;
extern bool deletemode;
// TODO:
// delete line
// right angle turnning instead of sloped line
graphicsscene::graphicsscene(QObject *parent) :QGraphicsScene(parent)
{

}


// Start drawing line
void graphicsscene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(deletemode){
        QGraphicsScene::mousePressEvent(mouseEvent);
    }
   if(!deletemode){
    if(!pressed){                                             // start a new turnning line
        int roundx1 = (int)mouseEvent->scenePos().x() % pix_per_brick;
        int roundy1 = (int)mouseEvent->scenePos().y() % pix_per_brick;

        line *drawline = new line();
        drawline->x[0] = mouseEvent->scenePos().x() - roundx1;
        drawline->y[0] = mouseEvent->scenePos().y() - roundy1;
        drawline->x[1] = drawline->x[0];
        drawline->y[1] = drawline->y[0];
        segline = drawline;                                                 // the first segment
        turnline = new line();
        pressed = true;
        qDebug() << "Start a new line";
//        qDebug() << "press" << drawline->x[turnline->segments] << drawline->x[turnline->segments] << drawline->y[turnline->segments+1] << drawline->y[turnline->segments+1] << pressed;
    }
    else if(pressed){
        if(turnline->segments % 2 != 0){
            to_destroy.push_back(segline2);
            this->addItem(segline2);
            connect(segline2, SIGNAL(delete_this_line(line *)), this, SLOT(delete_from_list(line *)));
            qDebug() << "Add a segment" << turnline->segments << "||" << segline2->x[0] << segline2->y[0] << segline2->x[1] << segline2->y[1];
            //update data to turnline
            turnline->x[turnline->segments] = segline2->x[0];
            turnline->y[turnline->segments] = segline2->y[0];
            turnline->x[turnline->segments+1] = segline2->x[1];
            turnline->y[turnline->segments+1] = segline2->y[1];
            turnline->segments++;

        // start a new segnemt
            line *drawline = new line();
            drawline->x[0] = turnline->x[turnline->segments];
            drawline->y[0] = turnline->y[turnline->segments];
            drawline->x[1] = turnline->x[turnline->segments];
            drawline->y[1] = turnline->y[turnline->segments];
            segline = drawline;
        }
        else{
            to_destroy.push_back(segline);
            this->addItem(segline);
            connect(segline, SIGNAL(delete_this_line(line *)), this, SLOT(delete_from_list(line *)));
            qDebug() << "Add a segment" << turnline->segments << "||" << segline->x[0] << segline->y[0] << segline->x[1] << segline->y[1];
            //update data to turnline
            turnline->x[turnline->segments] = segline->x[0];
            turnline->y[turnline->segments] = segline->y[0];
            turnline->x[turnline->segments+1] = segline->x[1];
            turnline->y[turnline->segments+1] = segline->y[1];
            turnline->segments++;

            line *drawline = new line();
            drawline->x[0] = turnline->x[turnline->segments];
            drawline->y[0] = turnline->y[turnline->segments];
            drawline->x[1] = turnline->x[turnline->segments];
            drawline->y[1] = turnline->y[turnline->segments];
            segline2 = drawline;
        }
    }
   }
}

// Move the line with the cursor while drawing line
void graphicsscene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(!deletemode){
    if(pressed){
        if(turnline->segments % 2 == 0){
            if(abs((mouseEvent->scenePos().x() - segline->x[0])) < 0.1){
                segline->x[1] = segline->x[0];
                segline->y[1] = mouseEvent->scenePos().y();
            }
            else{
                slope = (mouseEvent->scenePos().y() - segline->y[0]) / (mouseEvent->scenePos().x() - segline->x[0]);
                if(slope > 1 || slope < -1){
                    segline->x[1] = segline->x[0];
                    segline->y[1] = mouseEvent->scenePos().y();
                }
                else{
                    segline->x[1] = mouseEvent->scenePos().x();
                    segline->y[1] = segline->y[0];
                }
            }
            qDebug() << "A" << segline->x[0] << segline->y[0] << segline->x[1] << segline->y[1];
        }
        else{
            if(abs((mouseEvent->scenePos().x() - segline2->x[0])) < 0.1){
                segline2->x[1] = segline2->x[0];
                segline2->y[1] = mouseEvent->scenePos().y();
            }
            else{
                slope = (mouseEvent->scenePos().y() - segline2->y[0]) / (mouseEvent->scenePos().x() - segline2->x[0]);
                if(slope > 1 || slope < -1){
                    segline2->x[1] = segline2->x[0];
                    segline2->y[1] = mouseEvent->scenePos().y();
                }
                else{
                    segline2->x[1] = mouseEvent->scenePos().x();
                    segline2->y[1] = segline2->y[0];
                }
            }
            qDebug() << "B" << segline2->x[0] << segline2->y[0] << segline2->x[1] << segline2->y[1];
        }

    }
    }
}

// Draw the line following the grid
//void graphicsscene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
//{

//}

void graphicsscene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent){

    if(!deletemode){
        int size_to_destroy = to_destroy.count();
        for(int i=0; i<size_to_destroy; i++){
            delete to_destroy.last();
            to_destroy.pop_back();
        }
        qDebug() << turnline->segments;
        this->addItem(turnline);
        connect(turnline, SIGNAL(delete_this_line(line *)), this, SLOT(delete_from_list(line *)));
        alllines.prepend(turnline);
        pressed = false;
        qDebug() << "End a line";
    }
}

void graphicsscene::delete_from_list(line *delete_line)
{
    qDebug() << delete_line->x[0];
    alllines.removeOne(delete_line);
    delete delete_line;
}
