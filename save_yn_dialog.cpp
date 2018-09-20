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

void save_yn_dialog::on_yes_clicked()
{
    want_to_save = true;
    this->close();
    emit yes_save(this);
}

void save_yn_dialog::on_no_clicked()
{
    want_to_save = false;
    this->close();
    emit yes_save(this);
}
