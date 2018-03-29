#include "line.h"
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
line::line() : QWidget()
{
    mPix = new QPixmap(600,400);

    mPix->detach();
    mPix->fill(Qt::transparent);

    //set everything to false as nothing has started yet
    mousePressed = false;
    drawStarted = false;
}


void line::mousePressEvent(QMouseEvent* event){
    //Mouse is pressed for the first time
    mousePressed = true;

    //set the initial line points, both are same

    mLine.setP1(event->pos());
    mLine.setP2(event->pos());

}

void line::mouseMoveEvent(QMouseEvent* event){

    //As mouse is moving set the second point again and again
    // and update continuously
    if(event->type() == QEvent::MouseMove){
        mLine.setP2(event->pos());
    }

    //it calls the paintEven() function continuously
    update();
}

void line::mouseReleaseEvent(QMouseEvent *event){

    //When mouse is released update for the one last time
    mousePressed = false;
    update();
}

void line::paintEvent(QPaintEvent *event){

    painter.begin(this);

    //When the mouse is pressed
    if(mousePressed){
        // we are taking QPixmap reference again and again
        //on mouse move and drawing a line again and again
        //hence the painter view has a feeling of dynamic drawing
        painter.drawPixmap(0,0,*mPix);

        painter.drawLine(mLine);

        drawStarted = true;
    }
    else if (drawStarted){
        // It created a QPainter object by taking  a reference
        // to the QPixmap object created earlier, then draws a line
        // using that object, then sets the earlier painter object
        // with the newly modified QPixmap object
        QPainter tempPainter(mPix);

        tempPainter.drawLine(mLine);

        painter.drawPixmap(0,0,*mPix);
    }

    painter.end();
}

line::~line()
{
    delete this;
}
