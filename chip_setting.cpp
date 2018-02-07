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
