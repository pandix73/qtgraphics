#include "dispenser_edit.h"
#include "ui_dispenser_edit.h"

dispenser_edit::dispenser_edit(QString text, int length, int width, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dispenser_edit)
{
    ui->setupUi(this);
    this->length = length;
    this->width = width;
    this->text = text;
    QString length_string = QString::number(length);
    QString width_string = QString::number(width);
    ui->size_input_length->setPlaceholderText(length_string);
    ui->size_input_width->setPlaceholderText(width_string);
    ui->text_input->setPlaceholderText(this->text);

}

dispenser_edit::~dispenser_edit()
{
    delete ui;
}


void dispenser_edit::on_enter_clicked()
{
    if(!ui->size_input_length->text().isEmpty()) length = ui->size_input_length->text().toInt();
    if(!ui->size_input_width->text().isEmpty()) width = ui->size_input_width->text().toInt();
    if(!ui->text_input->text().isEmpty()) text = ui->text_input->text();
    emit update_this_label(text, length, width);
    this->close();
}
