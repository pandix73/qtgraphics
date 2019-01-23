#include "dispenser_edit.h"
#include "ui_dispenser_edit.h"
extern int de_spacing_um;
dispenser_edit::dispenser_edit(QString text, int actuallength, int actualwidth, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dispenser_edit)
{
    ui->setupUi(this);
    this->actual_length = actuallength;
    this->actual_width = actualwidth;
    this->text = text;
    QString length_string = QString::number(actuallength);
    QString width_string = QString::number(actualwidth);
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
    if(!ui->size_input_length->text().isEmpty())
        actual_length = ui->size_input_length->text().toInt();
    if(!ui->size_input_width->text().isEmpty())
        actual_width = ui->size_input_width->text().toInt();
    length = actual_length * 1000 / de_spacing_um;
    width = actual_width * 1000 / de_spacing_um;
    if(!ui->text_input->text().isEmpty()) text = ui->text_input->text();
    emit update_this_label(text, actual_length, actual_width, length, width);
    this->close();
}
