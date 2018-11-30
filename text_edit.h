#ifndef TEXT_EDIT_H
#define TEXT_EDIT_H

#include <QDialog>
#include "unit.h"

namespace Ui {
class text_edit;
}

class text_edit : public QDialog
{
    Q_OBJECT

public:
    explicit text_edit(QWidget *parent = 0);
    ~text_edit();
    QString temp_text;
    QString type;
signals:
    void update_this_label(QString);

private slots:
    void on_enter_clicked();

private:
    Ui::text_edit *ui;
};

#endif // TEXT_EDIT_H
