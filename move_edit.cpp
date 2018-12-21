#include "move_edit.h"
#include "ui_move_edit.h"

move_edit::move_edit(QString text, int size, int tilt, int de_type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::move_edit)
{
    ui->setupUi(this);
    this->text = text;
    this->size = size;
    this->tilt = tilt;
    this->de_type = de_type;
    QString size_string = QString::number(size);
    QString tilt_string = QString::number(tilt);
    QString de_type_string = QString::number(de_type);
    ui->size_input->setPlaceholderText(size_string);
    ui->tilt_input->setPlaceholderText(tilt_string);
    ui->de_type_input->setPlaceholderText(de_type_string);
    ui->text_input->setPlaceholderText(this->text);
}

move_edit::~move_edit()
{
    delete ui;
}

void move_edit::on_enter_clicked()
{
    if(!ui->size_input->text().isEmpty()) size = ui->size_input->text().toInt();
    if(!ui->tilt_input->text().isEmpty()) tilt = ui->tilt_input->text().toInt();
    if(!ui->de_type_input->text().isEmpty()) de_type = ui->de_type_input->text().toInt();
    if(!ui->text_input->text().isEmpty()) text = ui->text_input->text();
    emit update_this_label(text, size, tilt, de_type);
    this->close();
}
