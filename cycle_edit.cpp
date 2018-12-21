#include "cycle_edit.h"
#include "ui_cycle_edit.h"

cycle_edit::cycle_edit(QString text, int de_xnum, int de_ynum, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cycle_edit)
{
    ui->setupUi(this);
    this->de_xnum = de_xnum;
    this->de_ynum = de_ynum;
    this->text = text;
    QString xnum_string = QString::number(de_xnum);
    QString ynum_string = QString::number(de_ynum);
    ui->size_input_length->setPlaceholderText(xnum_string);
    ui->size_input_width->setPlaceholderText(ynum_string);
    ui->text_input->setPlaceholderText(this->text);
}

cycle_edit::~cycle_edit()
{
    delete ui;
}

void cycle_edit::on_enter_clicked()
{
    if(!ui->size_input_length->text().isEmpty()) de_xnum = ui->size_input_length->text().toInt();
    if(!ui->size_input_width->text().isEmpty()) de_ynum = ui->size_input_width->text().toInt();
    if(!ui->text_input->text().isEmpty()) text = ui->text_input->text();
    emit update_this_label(text, de_xnum, de_ynum);
    this->close();
}
