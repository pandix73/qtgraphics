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

void MainWindow::on_pushButton_clicked()
{
    //Rectangle
    QBrush blueBrush(Qt::blue);
    QPen blackpen(Qt::black);
    blackpen.setWidth(6);
    unit *a = new unit();
    a->length = ui->lineEdit->text().toInt();
    a->width = ui->lineEdit_2->text().toInt();
    a->xi = ui->lineEdit_3->text().toInt();
    a->yi = ui->lineEdit_4->text().toInt();
    a->type = ui->comboBox->currentText();
    if(ui->comboBox->currentIndex() == 0){
        a->color = Qt::yellow;
    } else if (ui->comboBox->currentIndex() == 1){
        a->color = Qt::blue;
    } else {
        a->color = Qt::red;
    }
    ui->view->scene()->addItem(a);
}

void MainWindow::on_toolButton_clicked()
{
    deletemode = 1-deletemode;
    if(deletemode == true){
        ui->toolButton->setStyleSheet("background-color: rgba(122,122,122,122); border:none;");
    } else {
        ui->toolButton->setStyleSheet("");
    }
}
