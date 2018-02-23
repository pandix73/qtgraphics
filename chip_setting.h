#ifndef CHIP_SETTING_H
#define CHIP_SETTING_H

#include <QDialog>

namespace Ui {
    class chip_setting;
}

class chip_setting : public QDialog
{
    Q_OBJECT

public:
    explicit chip_setting(QWidget *parent = 0);
    ~chip_setting();

private:
    Ui::chip_setting *ui;
};

#endif // CHIP_SETTING_H
