#include "mainwindow.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QtGui>
#include <QGraphicsView>
#include <QGraphicsItem>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow m(nullptr);
    QDesktopWidget dw;
    m.GetScreenSize(dw.width(), dw.height());

//    m.showFullScreen();
//    m.showMaximized();
    m.show();
    return a.exec();
}
