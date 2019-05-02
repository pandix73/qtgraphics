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
    explicit chip_setting(QWidget *parent = nullptr, double cl=0, double cw=0, double cb=0,
                          double cpl=0, double cpw=0,
                          double de1l=0, double de1w=0, double de2l=0, double de2w=0,
                          double des=0, double cps=0, double lw=0);
    ~chip_setting();


    double chip_length_mm;
    double chip_width_mm;
    double chip_border_mm;

    double cp_length_mm;
    double cp_width_mm;

    double de1_length_mm;
    double de1_width_mm;
    double de2_length_mm;
    double de2_width_mm;


    double de_spacing_mm;
    double cp_spacing_mm;
    double line_width_mm;


signals:
    void reset(chip_setting *);

private slots:
    void on_setting_clicked();                  // "open new chip" button

    void on_setting_cancel_clicked();

private:
    Ui::chip_setting *ui;
};

#endif // CHIP_SETTING_H
