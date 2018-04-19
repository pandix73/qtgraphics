#include "graphicsscene.h"
#include <QDebug>
extern int pix_per_brick;
extern int deletemode;
// TODO:
// delete line
// right angle turnning instead of sloped line
graphicsscene::graphicsscene(QObject *parent) :QGraphicsScene(parent)
{

}

// Start drawing line
void graphicsscene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
        int roundx1 = (int)mouseEvent->scenePos().x() % pix_per_brick;
        int roundy1 = (int)mouseEvent->scenePos().y() % pix_per_brick;
        myline = new QGraphicsLineItem(mouseEvent->scenePos().x() - roundx1, mouseEvent->scenePos().y() - roundy1,
                                       mouseEvent->scenePos().x() - roundx1, mouseEvent->scenePos().y() - roundy1);
        myline->setPen(QPen(Qt::black, 2));         // TODO: the later version needs to deal with the EXACT line width
        this->addItem(myline);
        pressed = true;

}

// Move the line with the cursor while drawing line
void graphicsscene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

//    turnline= new QGraphicsLineItem(mouseEvent->scenePos().x() - roundx1, mouseEvent->scenePos().y() - roundy1,
//                                        mouseEvent->scenePos().x() - roundx1, mouseEvent->scenePos().y() - roundy1);
    if(pressed){
        if((mouseEvent->scenePos().x() - myline->line().p1().y()) == 0){
            myline->setLine(QLineF(myline->line().p1().x(), myline->line().p1().y(), myline->line().p1().x(), mouseEvent->scenePos().y()));
        }
        else{
            slope = (mouseEvent->scenePos().y() - myline->line().p1().y()) / (mouseEvent->scenePos().x() - myline->line().p1().x());
            if(slope > 1 || slope < -1)
                myline->setLine(QLineF(myline->line().p1().x(), myline->line().p1().y(), myline->line().p1().x(), mouseEvent->scenePos().y()));
            else
                myline->setLine(QLineF(myline->line().p1().x(), myline->line().p1().y(), mouseEvent->scenePos().x(), myline->line().p1().y()));
        }
    }
}

// Draw the line following the grid
void graphicsscene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
//        int roundx2 = (int)mouseEvent->scenePos().x() % pix_per_brick;
//        int roundy2 = (int)mouseEvent->scenePos().y() % pix_per_brick;
        int roundx2 = (int)myline->line().p2().x() % pix_per_brick;
        int roundy2 = (int)myline->line().p2().y() % pix_per_brick;
        QLineF *newline = new QLineF(myline->line().p1().x(), myline->line().p1().y(), (myline->line().p2().x() - roundx2), (myline->line().p2().y() - roundy2));

        //qDebug() << newline->x1() << newline->y1() << newline->x2() << newline->y2();
        myline->setLine(*newline);
//        myline->setLine(QLineF(0 , 0, 10, 10));
        line *thisline = new line();
        thisline->x1 = newline->x1();
        thisline->y1 = newline->y1();
        thisline->x2 = newline->x2();
        thisline->y2 = newline->y2();
        alllines.prepend(newline);
        pressed = false;
//        if(deletemode){
//            alllines.removeOne()
//        }
        //        QLineF newline(myline->line().p1().x(), myline->line().p1().y(), (mouseEvent->scenePos().x() - roundx2), (mouseEvent->scenePos().y() - roundy2));
        //        qDebug() << myline->line().p1().x() <<  myline->line().p1().y() << (mouseEvent->scenePos().x() - roundx2) <<  (mouseEvent->scenePos().y() - roundy2);
}
