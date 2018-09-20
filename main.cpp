#include "mainwindow.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QtGui>
#include <QGraphicsView>
#include <QGraphicsItem>

int width;
int length;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDesktopWidget dw;
    width = dw.width();
    length = dw.height();
    MainWindow m;
    m.GetScreenSize(width, length);

//    m.showFullScreen();
//    m.showMaximized();
    m.show();
    return a.exec();
}
