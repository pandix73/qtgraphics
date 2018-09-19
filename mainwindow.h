#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "unit.h"
#include "chip_setting.h"
#include "save_yn_dialog.h"
#include "switch.h"
#include "svgreader.h"
#include "graphicsscene.h"
#include "line.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtCore>
#include <QtGui>
#include <QGraphicsView>
#include <QPainter>
#include <QPoint>
#include <QSvgGenerator>
#include <QFileDialog>
#include <QPrinter>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QList<unit*> allunits;                          // save all present units in list

    explicit MainWindow(QWidget *parent = 0);
    QGraphicsScene* mainscene;
    graphicsscene* linescene;
    ~MainWindow();

public slots:
    void delete_from_list(unit *a);                 // remove the unit from list when it's deleted
    QGraphicsScene* CreateNewScene();
    graphicsscene* CreateLineScene();

private slots:
    // LINESCENE
    void RefreshLineScene();
    // TOOLBAR
    void on_eraser_clicked();                       // "eraser" clicked

    void on_setting_btn_clicked();                  // "setting button" clicked, open new setting window
    void on_connect_btn_clicked();
    void on_controlpad_btn_clicked();

    // CREATE UNITS
    void EnableCreateUnit(bool enable);
    void on_move_create_clicked();                  // "create move" clicked
    void on_dispenser_create_clicked();             // "create dispenser" clicked
    void on_merge_create_clicked();                 // "create merge" clicked
    void on_cycling_create_clicked();               // "create cycling" clicked
    void on_heater_create_clicked();                // "create heater" clicked

    void mode_label(bool bChecked);

    // ACTIONS
    void new_chip_clicked();
    void save_svg();
    void load_svg_clicked();
    void export_clicked();
    void close_clicked();
    void pdf_clicked();
    void debugging();

    void close_window(save_yn_dialog *);
    void reset_setting(chip_setting *);

private:
    Ui::MainWindow *ui;
    QString path;
    QGraphicsLineItem *myline;
};


#endif // MAINWINDOW_H
