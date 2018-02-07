#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "switch.h"
#include "chip_setting.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsItem>
#include <QPoint>


#include <QMainWindow>
#include <QSvgGenerator>
#include <QFileDialog>
#include <QPainter>


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

    //TOOGLE BUTTON
    SwitchControl *pSwitchControl = new SwitchControl(this->centralWidget());
    pSwitchControl->setFixedWidth(55);
    pSwitchControl->setFixedHeight(28);
    pSwitchControl->move(365, 633);
    scene->addWidget(pSwitchControl);
    pSwitchControl->setToggle(false);
    connect(pSwitchControl, SIGNAL(toggled(bool)), this, SLOT(label_2(bool)));

    //EXPORT
    this->mainscene = scene;
    QAction *Save = ui->actionSave;
    connect(Save, SIGNAL(triggered()), this, SLOT(export_ai()));

    ui->view->setScene(scene);
}
void MainWindow::label_2(bool bChecked){
    if(bChecked){
        ui->label_2->setText("CUSTOMIZED");
    }
    else{
        ui->label_2->setText("SIMPLE");
    }
}

void MainWindow::export_ai()
{

    //Take file path and name that will create
    QString newPath = QFileDialog::getSaveFileName(this, "Save SVG",
        path, tr("SVG files (*.svg)"));

    if (newPath.isEmpty())
        return;

    path = newPath;

    QSvgGenerator generator;        // Create a file generator object
    generator.setFileName(path);    // We set the path to the file where to save vector graphics
    generator.setSize(QSize(mainscene->width(), mainscene->height()));  // Set the dimensions of the working area of the document in millimeters
    generator.setViewBox(QRect(0, 0, mainscene->width(), mainscene->height())); // Set the work area in the coordinates
    generator.setTitle("Drag");                          // The title document
    generator.setDescription("File created by SVG Example");

    QPainter painter;
    painter.begin(&generator);
    mainscene->render(&painter);
    painter.end();

     //At the end we get a vector drawing file with the contents of the graphic scenes

}

//void MainWindow::onToggled(bool bChecked)
//{
////    qDebug() << "State : " << bChecked;
//}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_toolButton_clicked()
{
    deletemode = 1-deletemode;
    QCursor cursorErase = QCursor(QPixmap(":/MainWindow/Icons/Icons/eraser_cursor.png"),0 , 0);
    if(deletemode == true){
        ui->toolButton->setStyleSheet("background-color: rgb(64, 72, 91);");
        ui->view->setCursor(cursorErase);
    } else {
        ui->toolButton->setStyleSheet("background-color: rgb(42, 48, 58);");
        ui->view->setCursor(Qt::ArrowCursor);
    }
}


//MOVE
void MainWindow::on_move_create_clicked()
{
    int number = ui->move_num->value();
    int position = 5;
    while(number--){
        unit *move = new unit();
        move->xi = position;
        move->yi = 1;
        move->type = "move";
        move->de_type = ui->move_num->value();
        move->de_xnum = ui->move_size->text().toInt();
        move->de_ynum = 1;
        move->length = move->de_xnum*de1_length_mm*1000/de_spacing_um + move->de_xnum-1;
        move->width = move->de_ynum*de1_width_mm*1000/de_spacing_um;
        move->color = moving_color;
        ui->view->scene()->addItem(move);
        allunits.push_back(move);
        position += ui->merge_length->text().toInt() + 3;
    }

}


//DISPENSER
void MainWindow::on_dispenser_create_clicked()
{

    int number = ui->dispenser_num->value();
    int position = 5;
    while(number--){
        unit *dispenser = new unit();
        dispenser->xi = position;
        dispenser->yi = 1;
        dispenser->type = "dispenser";
        dispenser->de_type = 0;
        dispenser->de_xnum = 1;
        dispenser->de_ynum = 1;
        dispenser->length = ui->dispenser_length->text().toInt()*1000/de_spacing_um;
        dispenser->width = ui->dispenser_width->text().toInt()*1000/de_spacing_um;
        dispenser->color = dispenser_color;
        ui->view->scene()->addItem(dispenser);
        allunits.push_back(dispenser);
        position += ui->merge_length->text().toInt() + 3;
    }


}


//MERGE
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
    int number = ui->cycling_num->value();
    int position = 5;
    while(number--){
        unit *cycle = new unit();
        cycle->xi = position;
        cycle->yi = 1;
        cycle->type = "cycle";
        cycle->de_type = 2; //ui->cycling_num->value();
        cycle->de_xnum = ui->cycling_length->text().toInt();
        cycle->de_ynum = ui->cycling_width->text().toInt();
        cycle->length = cycle->de_xnum*de2_length_mm*1000/de_spacing_um + cycle->de_xnum-1;
        cycle->width = cycle->de_ynum*de2_length_mm*1000/de_spacing_um + cycle->de_ynum-1;
        cycle->color = cycling_color;
        ui->view->scene()->addItem(cycle);
        allunits.push_back(cycle);
        position += ui->merge_length->text().toInt() + 3;
    }

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


void MainWindow::on_setting_btn_clicked()
{
    chip_setting *chip = new chip_setting(this);
    chip->setWindowTitle("Setting");
    chip->show();
}




