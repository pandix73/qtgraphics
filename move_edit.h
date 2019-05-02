#ifndef MOVE_EDIT_H
#define MOVE_EDIT_H

#include <QDialog>
//#include "unit.h"

namespace Ui {
class move_edit;
}

class move_edit : public QDialog
{
    Q_OBJECT

public:
    explicit move_edit(QString text, int size, int tilt, int de_type, QWidget *parent);
    ~move_edit();
    int size;
    int tilt;
    int de_type;
    QString text;

signals:
    void update_this_label(QString, int, int, int);

private slots:
    void on_enter_clicked();

private:
    Ui::move_edit *ui;
};

#endif // MOVE_EDIT_H
