#ifndef HEAT_EDIT_H
#define HEAT_EDIT_H

#include <QDialog>
//#include "unit.h"

namespace Ui {
class heat_edit;
}

class heat_edit : public QDialog
{
    Q_OBJECT

public:
    explicit heat_edit(QString text, int width, int linewidth, int layer, int tilt, int clockwise, QWidget *parent = 0);
    ~heat_edit();
    int width;
    int linewidth;
    int layer;
    int tilt;
    int clockwise;
    QString text;

signals:
    void update_this_label(QString, int, int, int, int, int);

private slots:
    void on_enter_clicked();

private:
    Ui::heat_edit *ui;
};

#endif // HEAT_EDIT_H
