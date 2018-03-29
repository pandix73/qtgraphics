#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "unit.h"
#include "chip_setting.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtCore>
#include <QtGui>


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
    ~MainWindow();

public slots:
    void delete_from_list(unit *a);                 // remove the unit from list when it's deleted
    QGraphicsScene* CreateNewScene();
private slots:

    // TOOLBAR
    void on_eraser_clicked();                       // "eraser" clicked

    void on_setting_btn_clicked();                  // "setting button" clicked, open new setting window
    void on_connect_btn_clicked();
    void on_temp_line_clicked();

    // CREATE UNITS
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

    void debugging();

    void reset_setting(chip_setting *);
//protected:
//    void mousePressEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
private:
    Ui::MainWindow *ui;
    QString path;
//    QLine mLine;
//    QPoint P1;
//    QPoint P2;
};


#endif // MAINWINDOW_H
