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
    explicit chip_setting(QWidget *parent = nullptr);
    ~chip_setting();

    int chip_length_cm = 3;
    int chip_width_cm = 2;
    int chip_border_mm = 3;

    int cp_length_mm = 2;
    int cp_width_mm = 4;

    int de1_length_mm = 2;
    int de1_width_mm = 4;
    int de2_length_mm = 1;
    int de2_width_mm = 2;

    int de_spacing_um = 500;
    int cp_spacing_um = 300;
    int line_width_um = 3;

signals:
    void reset(chip_setting *);

private slots:
    void on_setting_clicked();                  // "open new chip" button

    void on_setting_cancel_clicked();

private:
    Ui::chip_setting *ui;
};

#endif // CHIP_SETTING_H
