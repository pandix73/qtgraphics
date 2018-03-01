#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "unit.h"
#include "chip_setting.h"
#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsItem>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QList<unit*> allunits;

    explicit MainWindow(QWidget *parent = 0);
    QGraphicsScene* mainscene;
    ~MainWindow();
public slots:
    void delete_from_list(unit *a);
private slots:
    void on_eraser_clicked();

    void on_move_create_clicked();

    void on_dispenser_create_clicked();

    void on_merge_create_clicked();

    void on_cycling_create_clicked();

    void on_heater_create_clicked();

//    void on_checkBox_stateChanged(int arg1);

    void on_setting_update_clicked();

    void on_connect_btn_clicked();

    void mode_label(bool bChecked);

    void on_setting_btn_clicked();

    void save_svg();

    void load_svg_clicked();

    void export_clicked();
    //    void onToggled(bool bChecked);

    void debugging();

    void reset_setting(chip_setting *);
private:
    Ui::MainWindow *ui;
    QString path;
};

#endif // MAINWINDOW_H
