#include "text_edit.h"
#include "ui_text_edit.h"

text_edit::text_edit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::text_edit)
{
    ui->setupUi(this);
}

text_edit::~text_edit()
{
    delete ui;
}

void text_edit::on_enter_clicked()
{
    if(!ui->text_input->text().isEmpty()) temp_text = ui->text_input->text();
    emit update_this_label(temp_text);
    this->close();
}
