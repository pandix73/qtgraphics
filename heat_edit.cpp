#include "heat_edit.h"
#include "ui_heat_edit.h"
#include <qdebug.h>
heat_edit::heat_edit(QString text, int width, int linewidth, int layer, int tilt, int clockwise, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::heat_edit)
{
    ui->setupUi(this);
    this->width = width;
    this->linewidth = linewidth;
    this->layer = layer;
    this->tilt = tilt;
    this->clockwise = clockwise;
    this->text = text;
    QString width_string = QString::number(width);
    QString linewidth_string = QString::number(linewidth);
    QString layer_string = QString::number(layer);
    QString tilt_string = QString::number(tilt);
    QString clockwise_string = QString::number(clockwise);
    ui->width_input->setPlaceholderText(width_string);
    ui->linewidth_input->setPlaceholderText(linewidth_string);
    ui->layer_input->setPlaceholderText(layer_string);
    ui->tilt_input->setPlaceholderText(tilt_string);
    if(clockwise) ui->clockwise_input->setChecked(true);
    else ui->counterclockwise_input->setChecked(true);

    ui->text_input->setPlaceholderText(text);

}

heat_edit::~heat_edit()
{
    delete ui;
}

void heat_edit::on_enter_clicked()
{
    if(!ui->width_input->text().isEmpty()) width = ui->width_input->text().toInt();
    if(!ui->linewidth_input->text().isEmpty()) linewidth = ui->linewidth_input->text().toInt();
    if(!ui->layer_input->text().isEmpty()) layer = ui->layer_input->text().toInt();
    if(!ui->tilt_input->text().isEmpty()) tilt = ui->tilt_input->text().toInt();
    if(ui->clockwise_input->isChecked()) clockwise = 1;
    else clockwise = 0;
    qDebug() << width << linewidth << layer << tilt << clockwise;
    if(!ui->text_input->text().isEmpty()) text = ui->text_input->text();
    emit update_this_label(text, width, linewidth, layer, tilt, clockwise);
    this->close();
}
