#include "merge_edit.h"
#include "ui_merge_edit.h"
#include <QDebug>
merge_edit::merge_edit(QString text, int length, int width, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::merge_edit)
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

merge_edit::~merge_edit()
{
    delete ui;
}

void merge_edit::on_enter_clicked()
{
//    unit_type_label->setText(type);
    if(!ui->size_input_length->text().isEmpty()) length = ui->size_input_length->text().toInt();
    if(!ui->size_input_width->text().isEmpty()) width = ui->size_input_width->text().toInt();
    if(!ui->text_input->text().isEmpty()) text = ui->text_input->text();
    emit update_this_label(text, length, width);
    this->close();
}
