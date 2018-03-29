#ifndef SAVE_YN_DIALOG_H
#define SAVE_YN_DIALOG_H

#include <QDialog>

namespace Ui {
class save_yn_dialog;
}

class save_yn_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit save_yn_dialog(QWidget *parent = 0);
    ~save_yn_dialog();
    bool want_to_save = false;

signals:
    void yes_save(save_yn_dialog *);

private slots:
    void on_yes_clicked();

    void on_no_clicked();

private:
    Ui::save_yn_dialog *ui;
};

#endif // SAVE_YN_DIALOG_H
