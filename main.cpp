#include "mainwindow.h"
#include <QApplication>
#include <QtGui>
#include <QGraphicsView>
#include <QGraphicsItem>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow m;
    m.showMaximized();
    //m.show();
    return a.exec();
}
