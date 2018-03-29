#include "save_yn_dialog.h"
#include "ui_save_yn_dialog.h"

save_yn_dialog::save_yn_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::save_yn_dialog)
{
    ui->setupUi(this);
}

save_yn_dialog::~save_yn_dialog()
{
    delete ui;
}
