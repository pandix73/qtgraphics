#include "merge_edit.h"
#include "ui_merge_edit.h"

merge_edit::merge_edit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::merge_edit)
{
    ui->setupUi(this);
}

merge_edit::~merge_edit()
{
    delete ui;
}

void merge_edit::on_enter_clicked()
{
//    unit_type_label->setText(type);
    if(!ui->text_input->text().isEmpty()) temp_text = ui->text_input->text();
    if(!ui->text_input->text().isEmpty()) temp_text = ui->text_input->text();
    if(!ui->text_input->text().isEmpty()) temp_text = ui->text_input->text();
    emit update_this_label(temp_text);
    this->close();
}
