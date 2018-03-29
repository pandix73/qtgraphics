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

private:
    Ui::save_yn_dialog *ui;
};

#endif // SAVE_YN_DIALOG_H
