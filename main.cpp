#include "mainwindow.h"
#include <QApplication>
#include <QtGui>
#include <QGraphicsView>
#include <QGraphicsItem>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow m;
    QGraphicsView w;
    QGraphicsScene* scene = new QGraphicsScene(0, 0, 500, 500, &w);
    scene->setBackgroundBrush(Qt::gray);
    w.setScene(scene);

    //BURSHES
    QBrush redBrush(Qt::red);
    QBrush blueBrush(Qt::blue);
    QPen blackpen(Qt::black);
    blackpen.setWidth(6);

    //Ellipse
    QGraphicsEllipseItem* elli = new QGraphicsEllipseItem();
    elli = scene->addEllipse(50, 50, 100, 100, blackpen, redBrush);
    elli->setFlag(QGraphicsItem::ItemIsMovable);

    //Rectangle
    QGraphicsRectItem* rect = new QGraphicsRectItem();
    rect = scene->addRect(100, 100, 200, 200, blackpen, blueBrush);
    rect->setFlag(QGraphicsItem::ItemIsMovable);


    scene->addText("Hello world");
    w.show();
    m.show();
    return a.exec();
}



//    scene->addItem(elli);
//    scene->addItem(rect);
//    QGraphicsEllipseItem* elli = new QGraphicsEllipseItem(200, 200, 100, 100);
//    QGraphicsRectItem* rect = new QGraphicsRectItem(50, 50, 100, 100);
