#ifndef merge_edit_H
#define merge_edit_H

#include <QDialog>
#include "unit.h"

namespace Ui {
class merge_edit;
}

class merge_edit : public QDialog
{
    Q_OBJECT

public:
    explicit merge_edit(QString text, int actuallength, int actualwidth, QWidget *parent = 0);
    ~merge_edit();
    int actual_length;
    int actual_width;
    int length;
    int width;
    QString text;

signals:
    void update_this_label(QString, int, int, int, int);

private slots:
    void on_enter_clicked();

private:
    Ui::merge_edit *ui;
};

#endif // merge_edit_H
