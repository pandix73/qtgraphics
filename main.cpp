#include "mainwindow.h"
#include <QApplication>
#include <QtGui>
#include <QGraphicsView>
#include <QGraphicsItem>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow m;



    //scene->addText("Hello world");
    //w.show();
    m.show();
    return a.exec();
}



//    scene->addItem(elli);
//    scene->addItem(rect);
//    QGraphicsEllipseItem* elli = new QGraphicsEllipseItem(200, 200, 100, 100);
//    QGraphicsRectItem* rect = new QGraphicsRectItem(50, 50, 100, 100);
