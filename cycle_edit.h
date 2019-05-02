#ifndef CYCLE_EDIT_H
#define CYCLE_EDIT_H

#include <QDialog>

namespace Ui {
class cycle_edit;
}

class cycle_edit : public QDialog
{
    Q_OBJECT

public:
    explicit cycle_edit(QString text, int de_xnum, int de_ynum, QWidget *parent = nullptr);
    ~cycle_edit();
    int de_xnum;
    int de_ynum;
    QString text;

signals:
    void update_this_label(QString, int, int);

private slots:
    void on_enter_clicked();

private:
    Ui::cycle_edit *ui;
};

#endif // CYCLE_EDIT_H
