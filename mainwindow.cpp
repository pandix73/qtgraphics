#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsItem>

bool deletemode = false;
int chip_length_cm = 3;
int chip_width_cm = 2;
int chip_border_mm = 1;
int de1_length_mm = 1;
int de1_width_mm = 5;
int de_spacing_um = 500;
int cp_spacing_um = 300;
int pix_per_brick = 20;

int de1_width = 5;
int de1_length = 2;

int de2_width = 4;
int de2_length = 4;
int space_bw_de_l = 2;
int space_bw_de_w = 1;



class unit :public QGraphicsItem
{
public:
    unit();

    QRectF boundingRect() const;

    // overriding paint()
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);

    // item state
    bool Pressed;
    int xi = 0;
    int yi = 0;
    int length = 1;
    int width = 1;
    QString type;
    QColor color = Qt::white;
protected:
    // overriding mouse events
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

unit::unit()
{
    Pressed = false;
    setFlag(ItemIsMovable);
}

QRectF unit::boundingRect() const
{
    // outer most edges
    return QRectF(xi*pix_per_brick, yi*pix_per_brick, length*pix_per_brick, width*pix_per_brick);
}

void unit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();

    QBrush brush(this->color);
    painter->setBrush(brush);
    if(Pressed)
    {
        QPen pen(Qt::lightGray, 3);
        painter->setPen(pen);
        painter->drawRect(rect);
    }
    else
    {
        QPen pen(Qt::black, 3);
        painter->setPen(pen);
        painter->drawRect(rect);
    }
    painter->drawText(rect, type, Qt::AlignVCenter | Qt::AlignHCenter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void unit::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Pressed = true;
    update();
    QGraphicsItem::mousePressEvent(event);

}

void unit::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Pressed = false;
    int xindex = qRound(this->x()/pix_per_brick);
    int yindex = qRound(this->y()/pix_per_brick);
    this->setPos(xindex*pix_per_brick, yindex*pix_per_brick);
    update();
    QGraphicsItem::mouseReleaseEvent(event);
    if(deletemode) delete(this);
}





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setAcceptDrops(true);
    ui->setupUi(this);

    QPen graypen(Qt::gray);

    QGraphicsScene* scene = new QGraphicsScene(0, 0, 600, 400, ui->view);
    pix_per_brick = fmin(600/((10*chip_length_cm*2)),
                         400/((10*chip_width_cm*2)));
    scene->setBackgroundBrush(Qt::white);

    //chip border
    scene->addLine(0, 0, scene->width(), 0, graypen);
    scene->addLine(0, scene->height(), scene->width(), scene->height(), graypen);
    scene->addLine(0, 0, 0, scene->height(), graypen);
    scene->addLine(scene->width(), 0, scene->width(), scene->height(), graypen);

    int brick_start = pix_per_brick*chip_border_mm*2;
    for(int i = 0; i <= 20*chip_length_cm - 4*chip_border_mm; i++)
        scene->addLine(brick_start+i*pix_per_brick, brick_start, brick_start+i*pix_per_brick, scene->height()-brick_start, graypen);
    for(int i = 0; i <= 20*chip_width_cm - 4*chip_border_mm; i++)
        scene->addLine(brick_start, brick_start+i*pix_per_brick, scene->width()-brick_start, brick_start+i*pix_per_brick, graypen);

    ui->view->setScene(scene);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::on_pushButton_clicked()
//{
//    //Rectangle
//    QBrush blueBrush(Qt::blue);
//    QPen blackpen(Qt::black);
//    blackpen.setWidth(6);
//    unit *a = new unit();

//    // moving
//    unit *move = new unit();
//    move->length = ui->move_length->text().toInt();
//    move->width = ui->move_width->text().toInt();
//    move->xi = 1;
//    move->yi = 2;
//    move->type = ui->comboBox->currentText();
//    a->length = ui->lineEdit->text().toInt();
//    a->width = ui->lineEdit_2->text().toInt();
//    a->xi = ui->lineEdit_3->text().toInt();
//    a->yi = ui->lineEdit_4->text().toInt();
//    a->type = ui->comboBox->currentText();

//    if(ui->comboBox->currentIndex() == 0){
//        a->color = Qt::yellow;
//    } else if (ui->comboBox->currentIndex() == 1){
//        a->color = Qt::blue;
//    } else {
//        a->color = Qt::red;
//    }
//    ui->view->scene()->addItem(a);
//}

void MainWindow::on_toolButton_clicked()
{
    deletemode = 1-deletemode;
    if(deletemode == true){
        ui->toolButton->setStyleSheet("background-color: rgba(122,122,122,122); border:none;");
    } else {
        ui->toolButton->setStyleSheet("");
    }
}


//MOVE
void MainWindow::on_move_create_clicked()
{
    QBrush blueBrush(Qt::blue);
    QPen blackpen(Qt::black);
    blackpen.setWidth(6);

    int number = ui->move_num->value();
    int position = 5;
    while(number--){
        unit *move = new unit();
//        here
        if(ui->move_electrod->value() == 1){
            move->length = ui->move_size->text().toInt() * (de1_length + space_bw_de_l) - space_bw_de_l;
            move->width =  de1_width;
        }
        else if(ui->move_electrod->value() == 2){
            move->length = ui->move_size->text().toInt() * (de2_length + space_bw_de_l) - space_bw_de_l;
            move->width =  de2_width;
        }

        move->setRotation(ui->move_tilt->text().toInt());
        move->xi = position;
        move->yi = 5;

        move->color = QColor(249, 141, 151);

        ui->view->scene()->addItem(move);

        position += 3;
    }
}


//DISPENSER
void MainWindow::on_dispenser_create_clicked()
{
    QBrush blueBrush(Qt::blue);
    QPen blackpen(Qt::black);
    blackpen.setWidth(6);

    int number = ui->dispenser_num->value();
    int position = 5;
    while(number--){
        unit *dispenser = new unit();
        dispenser->length = ui->dispenser_length->text().toInt();
        dispenser->width = ui->dispenser_width->text().toInt();
        dispenser->xi = position;
        dispenser->yi = 10;

        dispenser->color = QColor(149, 196, 190);

        ui->view->scene()->addItem(dispenser);

        position += ui->dispenser_length->text().toInt() + 3;
    }
}


//MERGE
void MainWindow::on_merge_create_clicked()
{
    QBrush blueBrush(Qt::blue);
    QPen blackpen(Qt::black);
    blackpen.setWidth(6);

    int number = ui->merge_num->value();
    int position = 5;
    while(number--){
        unit *merge = new unit();
        merge->length = ui->merge_length->text().toInt();
        merge->width = ui->merge_width->text().toInt();
        merge->xi = position;
        merge->yi = 15;

        merge->color = QColor(255, 208, 166);

        ui->view->scene()->addItem(merge);

        position += ui->merge_length->text().toInt() + 3;
    }
}


//CYCLING
void MainWindow::on_cycling_create_clicked()
{
    QBrush blueBrush(Qt::blue);
    QPen blackpen(Qt::black);
    blackpen.setWidth(6);

    int number = ui->cycling_num->value();
    int position = 5;
    while(number--){
        unit *cycling = new unit();
//      here
        if(ui->cycling_electrod->value() == 1){
            cycling->length = ui->cycling_length->text().toInt() * (de1_length + space_bw_de_l) - space_bw_de_l;
            cycling->width =  ui->cycling_width->text().toInt() * (de2_width + space_bw_de_w) - space_bw_de_w;
        }
        else if(ui->cycling_electrod->value() == 2){
            cycling->length = ui->cycling_length->text().toInt() * (de2_length + space_bw_de_l) - space_bw_de_l;
            cycling->width =  ui->cycling_width->text().toInt() * (de2_width + space_bw_de_w) - space_bw_de_w;
        }

        cycling->xi = position;
        cycling->yi = 20;

        cycling->color = QColor(198, 218, 219);

        ui->view->scene()->addItem(cycling);

        position += ui->cycling_length->text().toInt() + 3;

    }

}


//CREATE
void MainWindow::on_heater_create_clicked()
{
    QBrush blueBrush(Qt::blue);
    QPen blackpen(Qt::black);
    blackpen.setWidth(6);

    int number = ui->heater_num->text().toInt();
    int position = 5;
    while(number--){
        unit *heater = new unit();
        heater->length = 5;
        heater->width = 5;
        heater->xi = position;
        heater->yi = 25;

        heater->color = QColor(108, 138, 148);

        ui->view->scene()->addItem(heater);

        position += 8;
    }
}
