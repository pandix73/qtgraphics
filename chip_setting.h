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

    int chip_length_mm = 76;
    int chip_width_mm = 52;
    int chip_border_mm = 5;

    int cp_length_mm = 2;
    int cp_width_mm = 8;

    int de1_length_mm = 1;
    int de1_width_mm = 5;
    int de2_length_mm = 3;
    int de2_width_mm = 3;

    int de_spacing_um = 500;
    float cp_spacing_mm = 2.5;
    int line_width_um = 200;

signals:
    void reset(chip_setting *);

private slots:
    void on_setting_clicked();                  // "open new chip" button

    void on_setting_cancel_clicked();

private:
    Ui::chip_setting *ui;
};

#endif // CHIP_SETTING_H
