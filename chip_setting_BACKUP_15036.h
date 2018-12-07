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


    float chip_length_mm = 76;
    float chip_width_mm = 52;
    float chip_border_mm = 5;

    float cp_length_mm = 8;
    float cp_width_mm = 2;

    float de1_length_mm = 1;
    float de1_width_mm = 5;
    float de2_length_mm = 3;
    float de2_width_mm = 3;


    float de_spacing_mm = 0.5;
    float cp_spacing_mm = 2.54;
    float line_width_mm = 0.353;


signals:
    void reset(chip_setting *);

private slots:
    void on_setting_clicked();                  // "open new chip" button

    void on_setting_cancel_clicked();

private:
    Ui::chip_setting *ui;
};

#endif // CHIP_SETTING_H
