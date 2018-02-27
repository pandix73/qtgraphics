#include "chip_setting.h"
#include "ui_chip_setting.h"

chip_setting::chip_setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chip_setting)
{
    ui->setupUi(this);
}

chip_setting::~chip_setting()
{
    delete ui;
}

void chip_setting::on_setting_clicked()
{
    if(!ui->chip_length->text().isEmpty()) chip_length_cm = ui->chip_length->text().toInt();
    if(!ui->chip_width->text().isEmpty()) chip_width_cm = ui->chip_width->text().toInt();
    if(!ui->chip_border->text().isEmpty()) chip_border_mm = ui->chip_border->text().toInt();
    if(!ui->de1_length->text().isEmpty()) de1_length_mm = ui->de1_length->text().toInt();
    if(!ui->de1_width->text().isEmpty()) de1_width_mm = ui->de1_width->text().toInt();
    if(!ui->de2_length->text().isEmpty()) de2_length_mm = ui->de2_length->text().toInt();
    if(!ui->de2_width->text().isEmpty()) de2_width_mm = ui->de2_width->text().toInt();
    if(!ui->de_spacing->text().isEmpty()) de_spacing_um = ui->de_spacing->text().toInt();
    if(!ui->cp_spacing->text().isEmpty()) cp_spacing_um = ui->cp_spacing->text().toInt();
    emit reset(this);
}
