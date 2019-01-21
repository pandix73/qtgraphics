#include "chip_setting.h"
#include "ui_chip_setting.h"

chip_setting::chip_setting(QWidget *parent, float cl, float cw, float cb,
                           float cpl, float cpw,
                           float de1l, float de1w, float de2l, float de2w,
                           float des, float cps, float lw) : QDialog(parent), ui(new Ui::chip_setting)
{
    ui->setupUi(this);
    chip_length_mm = cl;
    chip_width_mm = cw;
    chip_border_mm = cb;

    cp_length_mm = cpl;
    cp_width_mm = cpw;

    de1_length_mm = de1l;
    de1_width_mm = de1w;
    de2_length_mm = de2l;
    de2_width_mm =  de2w;

    de_spacing_mm = des;
    cp_spacing_mm = cps;
    line_width_mm = lw;

    QString chip_length_string = QString::number(chip_length_mm);
    QString chip_width_string = QString::number(chip_width_mm);
    QString chip_border_string = QString::number(chip_border_mm);
    ui->chip_length->setPlaceholderText(chip_length_string);
    ui->chip_width->setPlaceholderText(chip_width_string);
    ui->chip_border->setPlaceholderText(chip_border_string);

    QString cp_length_string = QString::number(cp_length_mm);
    QString cp_width_string = QString::number(cp_width_mm);
    ui->cp_length->setPlaceholderText(cp_length_string);
    ui->cp_width->setPlaceholderText(cp_width_string);

    QString de1_length_string = QString::number(de1_length_mm);
    QString de1_width_string = QString::number(de1_width_mm);
    QString de2_length_string = QString::number(de2_length_mm);
    QString de2_width_string = QString::number(de2_width_mm);
    ui->de1_length->setPlaceholderText(de1_length_string);
    ui->de1_width->setPlaceholderText(de1_width_string);
    ui->de2_length->setPlaceholderText(de2_length_string);
    ui->de2_width->setPlaceholderText(de2_width_string);

    QString de_spacing_string = QString::number(de_spacing_mm);
    QString cp_spacing_string = QString::number(cp_spacing_mm);
    QString line_width_string = QString::number(line_width_mm);
    ui->de_spacing->setPlaceholderText(de_spacing_string);
    ui->cp_spacing->setPlaceholderText(cp_spacing_string);
    ui->line_width->setPlaceholderText(line_width_string);
}

chip_setting::~chip_setting()
{
    delete ui;
}

// "open new chip" button clicked
void chip_setting::on_setting_clicked()
{
    if(!ui->chip_length->text().isEmpty()) chip_length_mm = ui->chip_length->text().toFloat();
    if(!ui->chip_width->text().isEmpty()) chip_width_mm = ui->chip_width->text().toFloat();
    if(!ui->chip_border->text().isEmpty()) chip_border_mm = ui->chip_border->text().toFloat();

    if(!ui->cp_length->text().isEmpty()) cp_length_mm = ui->cp_length->text().toFloat();
    if(!ui->cp_width->text().isEmpty()) cp_width_mm = ui->cp_width->text().toFloat();

    if(!ui->de1_length->text().isEmpty()) de1_length_mm = ui->de1_length->text().toFloat();
    if(!ui->de1_width->text().isEmpty()) de1_width_mm = ui->de1_width->text().toFloat();
    if(!ui->de2_length->text().isEmpty()) de2_length_mm = ui->de2_length->text().toFloat();
    if(!ui->de2_width->text().isEmpty()) de2_width_mm = ui->de2_width->text().toFloat();

    if(!ui->de_spacing->text().isEmpty()) de_spacing_mm = ui->de_spacing->text().toFloat();
    if(!ui->cp_spacing->text().isEmpty()) cp_spacing_mm = ui->cp_spacing->text().toFloat();
    if(!ui->line_width->text().isEmpty()) line_width_mm = ui->line_width->text().toFloat();
    emit reset(this);
    this->close();
}

void chip_setting::on_setting_cancel_clicked()
{
    this->close();
}
