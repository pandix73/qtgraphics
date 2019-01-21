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
    explicit chip_setting(QWidget *parent = 0, float cl=0, float cw=0, float cb=0,
                          float cpl=0, float cpw=0,
                          float de1l=0, float de1w=0, float de2l=0, float de2w=0,
                          float des=0, float cps=0, float lw=0);
    ~chip_setting();


    float chip_length_mm;
    float chip_width_mm;
    float chip_border_mm;

    float cp_length_mm;
    float cp_width_mm;

    float de1_length_mm;
    float de1_width_mm;
    float de2_length_mm;
    float de2_width_mm;


    float de_spacing_mm;
    float cp_spacing_mm;
    float line_width_mm;


signals:
    void reset(chip_setting *);

private slots:
    void on_setting_clicked();                  // "open new chip" button

    void on_setting_cancel_clicked();

private:
    Ui::chip_setting *ui;
};

#endif // CHIP_SETTING_H
