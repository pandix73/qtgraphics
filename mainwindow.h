#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_toolButton_clicked();

    void on_move_create_clicked();

    void on_dispenser_create_clicked();

    void on_merge_create_clicked();

    void on_cycling_create_clicked();

    void on_heater_create_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_setting_update_clicked();

    void on_toolButton_5_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
