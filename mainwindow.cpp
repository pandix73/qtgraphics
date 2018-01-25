#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsItem>



bool deletemode = false;
bool detailmode = false;

//setting parameter
int chip_length_cm = 3;
int chip_width_cm = 2;
int chip_border_mm = 3;
int cp_length_mm = 2;
int cp_width_mm = 4;
int de1_length_mm = 1;
int de1_width_mm = 5;
int de2_length_mm = 3;
int de_spacing_um = 500;
int cp_spacing_um = 300;
int dispenser_mm = 10;
int pix_per_brick = 20;

//units color
QColor de1_color = QColor(255, 0, 0, 127);
QColor de2_color = QColor(0, 0, 255, 127);
QColor cycling_color = QColor(1, 96, 177, 127);
QColor moving_color = QColor(255, 184, 184, 127);
QColor dispenser_color = QColor(215, 230, 144, 127);

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
    int de_type = 0;
    int de_xnum = 0;
    int de_ynum = 0;
    QString type;
    QColor color = Qt::white;
protected:
    // overriding mouse events
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

QList<unit*> allunits;

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
        QPen pen(Qt::lightGray, 1);
        painter->setPen(pen);
        painter->drawRect(rect);
    }
    else if (detailmode)
    {
        QPen pen(Qt::black, 1);
        painter->setPen(pen);
        if(this->type == "move"){
            for(int i = 0; i < this->de_xnum; i++){
                if(this->de_type == 1){
                    painter->setBrush(QBrush(de1_color));
                    painter->drawRect(rect.x()+i*(pix_per_brick*de1_length_mm*1000/de_spacing_um + pix_per_brick),
                                      rect.y(),
                                      pix_per_brick*de1_length_mm*1000/de_spacing_um,
                                      pix_per_brick*de1_width_mm*1000/de_spacing_um);
                } else {
                    painter->setBrush(QBrush(de2_color));
                    painter->drawRect(rect.x()+i*(pix_per_brick*de2_length_mm*1000/de_spacing_um + pix_per_brick),
                                      rect.y(),
                                      pix_per_brick*de2_length_mm*1000/de_spacing_um,
                                      pix_per_brick*de2_length_mm*1000/de_spacing_um);
                }
            }
        } else if (this->type == "cycle"){
            painter->setBrush(QBrush(de2_color));
            for(int i = 0; i < this->de_xnum; i++){
                for(int j = 0; j < this->de_ynum; j++){
                    if(i!=0 && i!=this->de_xnum-1 && j!=0 && j!=this->de_ynum-1) continue;
                    painter->drawRect(rect.x()+i*(pix_per_brick*de2_length_mm*1000/de_spacing_um + pix_per_brick),
                                      rect.y()+j*(pix_per_brick*de2_length_mm*1000/de_spacing_um + pix_per_brick),
                                      pix_per_brick*de2_length_mm*1000/de_spacing_um,
                                      pix_per_brick*de2_length_mm*1000/de_spacing_um);
                }
            }
        } else {
            painter->drawRect(rect);
        }

    }
    else
    {
        QPen pen(Qt::black, 1);
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
    if(deletemode){
        delete(this);
    }
}





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setAcceptDrops(true);
    ui->setupUi(this);

    QPen graypen(Qt::gray);

    QGraphicsScene* scene = new QGraphicsScene(0, 0, 600, 400, ui->view);
    pix_per_brick = fmin(600/((10*chip_length_cm*1000/de_spacing_um)),
                         400/((10*chip_width_cm*1000/de_spacing_um)));
    scene->setBackgroundBrush(Qt::white);

    //chip border
    scene->addLine(0, 0, scene->width(), 0, graypen);
    scene->addLine(0, scene->height(), scene->width(), scene->height(), graypen);
    scene->addLine(0, 0, 0, scene->height(), graypen);
    scene->addLine(scene->width(), 0, scene->width(), scene->height(), graypen);

    int brick_start = pix_per_brick*chip_border_mm*2;
    int brick_xnum = (10*chip_length_cm - 2*chip_border_mm)*1000/de_spacing_um;
    int brick_ynum = (10*chip_width_cm - 2*chip_border_mm)*1000/de_spacing_um;
    for(int i = 0; i <= brick_xnum; i++)
        scene->addLine(brick_start+i*pix_per_brick, brick_start, brick_start+i*pix_per_brick, scene->height()-brick_start, graypen);
    for(int i = 0; i <= brick_ynum; i++)
        scene->addLine(brick_start, brick_start+i*pix_per_brick, scene->width()-brick_start, brick_start+i*pix_per_brick, graypen);

    /*//test
    detailmode = true;
    unit *te = new unit();
    te->length = 27;
    te->width = 20;
    te->xi = 3;
    te->yi = 3;
    te->de_xnum = 4;
    te->de_ynum = 3;
    te->de_type = 2;
    te->type = "cycle";
    te->color = Qt::yellow;
    scene->addItem(te);
    */
    ui->view->setScene(scene);

}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::on_move_create_clicked()
{
    unit *move = new unit();
    move->xi = 1;
    move->yi = 1;
    move->type = "move";
    move->de_type = ui->move_num->value();
    move->de_xnum = 4;
    move->de_ynum = 1;
    move->length = move->de_xnum*de1_length_mm*1000/de_spacing_um + move->de_xnum-1;
    move->width = move->de_ynum*de1_width_mm*1000/de_spacing_um;
    move->color = moving_color;
    ui->view->scene()->addItem(move);
    allunits.push_back(move);
}


void MainWindow::on_dispenser_create_clicked()
{
    unit *dispenser = new unit();
    dispenser->xi = 1;
    dispenser->yi = 1;
    dispenser->type = "dispenser";
    dispenser->de_type = 0;
    dispenser->de_xnum = 1;
    dispenser->de_ynum = 1;
    dispenser->length = dispenser->de_xnum*dispenser_mm*1000/de_spacing_um;
    dispenser->width = dispenser->de_ynum*dispenser_mm*1000/de_spacing_um;
    dispenser->color = dispenser_color;
    ui->view->scene()->addItem(dispenser);
    allunits.push_back(dispenser);
}

void MainWindow::on_merge_create_clicked()
{
    /*
    unit *merge = new unit();
    merge->xi = 1;
    merge->yi = 1;
    merge->type = "merge";
    //
    merge->color = moving_color;
    ui->view->scene()->addItem(merge);
    */
}

void MainWindow::on_cycling_create_clicked()
{
    unit *cycle = new unit();
    cycle->xi = 1;
    cycle->yi = 1;
    cycle->type = "cycle";
    cycle->de_type = 2; //ui->cycling_num->value();
    cycle->de_xnum = 3;
    cycle->de_ynum = 3;
    cycle->length = cycle->de_xnum*de2_length_mm*1000/de_spacing_um + cycle->de_xnum-1;
    cycle->width = cycle->de_ynum*de2_length_mm*1000/de_spacing_um + cycle->de_ynum-1;
    cycle->color = cycling_color;
    ui->view->scene()->addItem(cycle);
    allunits.push_back(cycle);
}

void MainWindow::on_heater_create_clicked()
{
    unit *heat = new unit();
    heat->xi = 1;
    heat->yi = 1;
    heat->type = "heat";
    heat->de_xnum = 1;
    heat->de_ynum = 2;
    ui->view->scene()->addItem(heat);
    allunits.push_back(heat);
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    detailmode = arg1;
    ui->view->scene()->update(0, 0, ui->view->scene()->width(), ui->view->scene()->height());
}

void MainWindow::on_setting_update_clicked()
{
    if(!ui->chip_length->text().isEmpty()) chip_length_cm = ui->chip_length->text().toInt();
    if(!ui->chip_width->text().isEmpty()) chip_width_cm = ui->chip_width->text().toInt();
    if(!ui->chip_border->text().isEmpty()) chip_border_mm = ui->chip_border->text().toInt();
    if(!ui->de1_length->text().isEmpty()) de1_length_mm = ui->de1_length->text().toInt();
    if(!ui->de1_width->text().isEmpty()) de1_width_mm = ui->de1_width->text().toInt();
    if(!ui->de2_length->text().isEmpty()) de2_length_mm = ui->de2_length->text().toInt();
    if(!ui->de_spacing->text().isEmpty()) de_spacing_um = ui->de_spacing->text().toInt();
    if(!ui->cp_spacing->text().isEmpty()) cp_spacing_um = ui->cp_spacing->text().toInt();

    QPen graypen(Qt::gray);

    QGraphicsScene* newscene = new QGraphicsScene(0, 0, 600, 400, ui->view);
    pix_per_brick = fmin(600/((10*chip_length_cm*1000/de_spacing_um)),
                         400/((10*chip_width_cm*1000/de_spacing_um)));
    newscene->setBackgroundBrush(Qt::white);

    //chip border
    newscene->addLine(0, 0, newscene->width(), 0, graypen);
    newscene->addLine(0, newscene->height(), newscene->width(), newscene->height(), graypen);
    newscene->addLine(0, 0, 0, newscene->height(), graypen);
    newscene->addLine(newscene->width(), 0, newscene->width(), newscene->height(), graypen);

    int brick_start = pix_per_brick*chip_border_mm*2;
    int brick_xnum = (10*chip_length_cm - 2*chip_border_mm)*1000/de_spacing_um;
    int brick_ynum = (10*chip_width_cm - 2*chip_border_mm)*1000/de_spacing_um;
    for(int i = 0; i <= brick_xnum; i++)
        newscene->addLine(brick_start+i*pix_per_brick, brick_start, brick_start+i*pix_per_brick, newscene->height()-brick_start, graypen);
    for(int i = 0; i <= brick_ynum; i++)
        newscene->addLine(brick_start, brick_start+i*pix_per_brick, newscene->width()-brick_start, brick_start+i*pix_per_brick, graypen);

    delete(ui->view->scene());
    allunits.clear();
    ui->view->setScene(newscene);
}

void MainWindow::on_toolButton_5_clicked()
{
    int count = 0;
    for(unit* unit : allunits){
        count += unit->de_xnum*unit->de_ynum;
    }
    int top = count / 2;
    int bot = count / 2 + count % 2;
    int start = chip_length_cm*10 - top*cp_length_mm;
    QPen graypen(Qt::gray);
    QBrush blackgr(Qt::black);

    for(int i = 0, x = start*pix_per_brick; i < top; i++){
        ui->view->scene()->addRect(x, 0.3*pix_per_brick, pix_per_brick*cp_length_mm*1000/de_spacing_um,
                                   pix_per_brick*cp_width_mm*1000/de_spacing_um, graypen, blackgr);
        x += pix_per_brick*(cp_length_mm*1000 + cp_spacing_um)/de_spacing_um;
    }
    for(int i = 0, x = start*pix_per_brick; i < bot; i++){
        ui->view->scene()->addRect(x, (chip_width_cm*10-cp_width_mm)*1000/de_spacing_um*pix_per_brick, pix_per_brick*cp_length_mm*1000/de_spacing_um,
                                   pix_per_brick*cp_width_mm*1000/de_spacing_um, graypen, blackgr);
        x += pix_per_brick*(cp_length_mm*1000 + cp_spacing_um)/de_spacing_um;
    }

    ui->view->scene()->update();
}
