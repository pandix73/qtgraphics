#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "unit.h"
#include "switch.h"
#include "chip_setting.h"
#include "svgreader.h"

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
QColor merge_color = QColor(255, 208, 166, 127);
QColor cycling_color = QColor(1, 96, 177, 127);
QColor moving_color = QColor(255, 184, 184, 127);
QColor dispenser_color = QColor(215, 230, 144, 127);
QColor heat_color = QColor(108, 137, 147, 127);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setAcceptDrops(true);
    ui->setupUi(this);

    QPen graypen(Qt::gray);
    QPen redpen(Qt::red);

    QGraphicsScene* scene = new QGraphicsScene(0, 0, 600, 400, ui->view);
    int tight = 0;
    int pix_length_brick = 600/((10*chip_length_cm*1000)/de_spacing_um);
    int pix_width_brick = 400/((10*chip_width_cm*1000)/de_spacing_um);
    if(pix_length_brick < pix_width_brick){
        pix_per_brick = pix_length_brick;
        tight = 1;
    }else{
        pix_per_brick = pix_width_brick;
        tight = 2;
    }

    scene->setBackgroundBrush(Qt::white);

    //outer border(nothing to do with real chip size, just the canvas size)
    scene->addLine(0, 0, scene->width(), 0, graypen);
    scene->addLine(0, scene->height(), scene->width(), scene->height(), graypen);
    scene->addLine(0, 0, 0, scene->height(), graypen);
    scene->addLine(scene->width(), 0, scene->width(), scene->height(), graypen);

    //chip background grid
    int border_px = chip_border_mm*1000/de_spacing_um*pix_per_brick;
    int brick_xnum = (10*chip_length_cm - 2*chip_border_mm)*1000/de_spacing_um;
    int brick_ynum = (10*chip_width_cm - 2*chip_border_mm)*1000/de_spacing_um;
    int chip_width_px = brick_xnum*pix_per_brick+border_px*2;
    int chip_height_px = brick_ynum*pix_per_brick+border_px*2;
    int brick_x_start;
    int brick_y_start;
    if(tight == 1){
        brick_x_start = (600 - chip_width_px)/2 + border_px;
        brick_y_start = (400 - chip_height_px)/2 + border_px;
    }else{
        brick_x_start = (600 - chip_width_px)/2 + border_px;
        brick_y_start = (400 - chip_height_px)/2 + border_px;
    }
    for(int i = 0; i <= brick_xnum; i++){
        for(int j = 0; j <= brick_ynum; j++){
            scene->addEllipse(brick_x_start+i*pix_per_brick, brick_y_start+j*pix_per_brick, 1, 1, graypen);
        }
    }

    //chip scale
    int cm_to_px = 10000/de_spacing_um*pix_per_brick;
    for(int i = 1; i < chip_length_cm; i++){
        scene->addEllipse(brick_x_start-border_px + i*cm_to_px, brick_y_start-border_px+chip_height_px-2, 1, 1, redpen);
    }
    for(int i = 1; i < chip_width_cm; i++){
        scene->addEllipse(brick_x_start-border_px+2, brick_y_start-border_px + i*cm_to_px, 1, 1, redpen);
    }
    //chip border
    scene->addRect(brick_x_start-border_px, brick_y_start-border_px, chip_width_px, chip_height_px, redpen);

//    1 cm = 10000/de_spacing_um*pix_per_brick

    //TOOGLE BUTTON
    SwitchControl *pSwitchControl = new SwitchControl(this->centralWidget());
    pSwitchControl->setFixedWidth(55);
    pSwitchControl->setFixedHeight(28);
    pSwitchControl->move(365, 633);
    scene->addWidget(pSwitchControl);
    pSwitchControl->setToggle(false);
    connect(pSwitchControl, SIGNAL(toggled(bool)), this, SLOT(label_2(bool)));

    //SAVE
    this->mainscene = scene;
    QAction *Save = ui->actionSave;
    connect(Save, SIGNAL(triggered()), this, SLOT(save_svg()));


    //LOAD
    QAction *Load = ui->actionOpen;
    connect(Load, SIGNAL(triggered()), this, SLOT(load_svg_clicked()));
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

//SAVE
void MainWindow::save_svg()
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

//LOAD
void MainWindow::load_svg_clicked()
{
    QString newPath = QFileDialog::getOpenFileName(this, "Open SVG", path, tr("SVG files (*.svg)"));
    if (newPath.isEmpty())
        return;

    path = newPath;
    mainscene->clear();

    mainscene->setSceneRect(SvgReader::getSizes(path));


    int flag = 2;
    QPen graypen(Qt::gray);
    foreach (unit *item, SvgReader::getElements(path)) {
        unit *rect = item;
        if(flag==2){
            flag--;
        }else if(flag==1){
            int brick_start = pix_per_brick*chip_border_mm*2;
            int brick_xnum = (10*rect->length - 2*chip_border_mm)*1000/de_spacing_um;
            int brick_ynum = (10*rect->width - 2*chip_border_mm)*1000/de_spacing_um;
            for(int i = 0; i <= brick_xnum; i++){
                for(int j = 0; j <= brick_ynum; j++){
                    mainscene->addEllipse(brick_start+i*pix_per_brick, brick_start+j*pix_per_brick, 1, 1, graypen);
                }
            }
                        qDebug() << "x :" << 10*rect->width/rect->unit_pix_per_brick;
                        qDebug() << "y :" << 10*rect->length/rect->unit_pix_per_brick;
            flag--;
        }else{
            mainscene->addItem(rect);

        }

    }



}

//void MainWindow::onToggled(bool bChecked)
//{
////    qDebug() << "State : " << bChecked;
//}

MainWindow::~MainWindow()
{
    delete ui;
}
/////////////////////////////////////  TOOL  BAR  /////////////////////////////////////
//DELETE (ERASER)
void MainWindow::on_eraser_clicked()
{
    deletemode = 1-deletemode;
    qDebug() << "number : " << allunits.count();
    qDebug() << "GLOBAL MODE : " <<deletemode;

    QCursor cursorErase = QCursor(QPixmap(":/MainWindow/Icons/Icons/eraser_cursor.png"),0 , 0);
    if(deletemode == true){
        for(unit* unit : allunits){
            unit->setDeleteMode(deletemode);
            qDebug() << "MODE : " << unit->unit_deletemode;
        }
        ui->eraser->setStyleSheet("background-color: rgb(64, 72, 91);");
        ui->view->setCursor(cursorErase);
    } else {
        for(unit* unit : allunits){
            unit->setDeleteMode(deletemode);
            qDebug() << "MODE : " << unit->unit_deletemode;
        }
        ui->eraser->setStyleSheet("background-color: rgb(42, 48, 58);");
        ui->view->setCursor(Qt::ArrowCursor);
    }
}

void MainWindow::delete_from_list(unit *item)
{
    allunits.removeOne(item);
    delete item;
}

/////////////////////////////////////  CREATE COMPONENTS  /////////////////////////////////////
//MOVE
void MainWindow::on_move_create_clicked()
{
    int number = ui->move_num->value();
    int position = 5;
    while(number--){
        unit *move = new unit();
        move->type = "move";
        move->xi = position;
        move->yi = 0;
        move->de_type = ui->move_num->value();
        move->de_xnum = ui->move_size->text().toInt();
        move->de_ynum = 1;
        move->length = move->de_xnum*de1_length_mm*1000/de_spacing_um + move->de_xnum-1;
        move->width = move->de_ynum*de1_width_mm*1000/de_spacing_um;
        move->color = moving_color;
        ui->view->scene()->addItem(move);
        allunits.prepend(move);
        position += 5;
        move->setParent(this);
        connect(move, SIGNAL(delete_this_item(unit *)), this, SLOT(delete_from_list(unit *)));
    }
}


//DISPENSER
void MainWindow::on_dispenser_create_clicked()
{

    int number = ui->dispenser_num->value();
    int position = 5;
    while(number--){
        unit *dispenser = new unit();
        dispenser->type = "dispenser";
        dispenser->xi = position;
        dispenser->yi = 5;
        dispenser->de_type = 0;
        dispenser->de_xnum = 1;
        dispenser->de_ynum = 1;
        dispenser->length = ui->dispenser_length->text().toInt()*1000/de_spacing_um;
        dispenser->width = ui->dispenser_width->text().toInt()*1000/de_spacing_um;
        dispenser->color = dispenser_color;
        ui->view->scene()->addItem(dispenser);
        allunits.prepend(dispenser);
        position += ui->merge_length->text().toInt() + 3;
        connect(dispenser, SIGNAL(delete_this_item(unit *)), this, SLOT(delete_from_list(unit *)));
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
        merge->type = "merge";
        merge->length = ui->merge_length->text().toInt();
        merge->width = ui->merge_width->text().toInt();
        merge->xi = position;
        merge->yi = 10;

        merge->color = merge_color;

        ui->view->scene()->addItem(merge);
        allunits.prepend(merge);
        position += ui->merge_length->text().toInt() + 3;
        connect(merge, SIGNAL(delete_this_item(unit *)), this, SLOT(delete_from_list(unit *)));
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
        cycle->yi = 15;
        cycle->type = "cycle";
        cycle->de_type = 2; //ui->cycling_num->value();
        cycle->de_xnum = ui->cycling_length->text().toInt();
        cycle->de_ynum = ui->cycling_width->text().toInt();
        cycle->length = cycle->de_xnum*de2_length_mm*1000/de_spacing_um + cycle->de_xnum-1;
        cycle->width = cycle->de_ynum*de2_length_mm*1000/de_spacing_um + cycle->de_ynum-1;
        cycle->color = cycling_color;
        ui->view->scene()->addItem(cycle);
        allunits.prepend(cycle);
        position += ui->merge_length->text().toInt() + 3;
        connect(cycle, SIGNAL(delete_this_item(unit *)), this, SLOT(delete_from_list(unit *)));
    }

}

//HEATER
void MainWindow::on_heater_create_clicked()
{
    int number = ui->heater_num->text().toInt();
    int position = 5;
    while(number--){
        unit *heat = new unit();
        heat->xi = position;
        heat->yi = 20;
        heat->type = "heat";
        heat->de_xnum = 1;
        heat->de_ynum = 2;
        heat->color = heat_color;
        ui->view->scene()->addItem(heat);
        allunits.prepend(heat);
        position += 3;
        connect(heat, SIGNAL(delete_this_item(unit *)), this, SLOT(delete_from_list(unit *)));
    }
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    detailmode = arg1;
    ui->view->scene()->update(0, 0, ui->view->scene()->width(), ui->view->scene()->height());
}

/////////////////////////////////////  SETTING  /////////////////////////////////////
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
    QPen redpen(Qt::red);
    QGraphicsScene* newscene = new QGraphicsScene(0, 0, 600, 400, ui->view);
//    pix_per_brick = fmin(600/((10*chip_length_cm*1000/de_spacing_um)),
//                         400/((10*chip_width_cm*1000/de_spacing_um)));
    int tight = 0;
    int pix_length_brick = 600/((10*chip_length_cm*1000)/de_spacing_um);
    int pix_width_brick = 400/((10*chip_width_cm*1000)/de_spacing_um);
    if(pix_length_brick < pix_width_brick){
        pix_per_brick = pix_length_brick;
        tight = 1;
    }else{
        pix_per_brick = pix_width_brick;
        tight = 2;
    }
    newscene->setBackgroundBrush(Qt::white);

    //outer border
    newscene->addLine(0, 0, newscene->width(), 0, graypen);
    newscene->addLine(0, newscene->height(), newscene->width(), newscene->height(), graypen);
    newscene->addLine(0, 0, 0, newscene->height(), graypen);
    newscene->addLine(newscene->width(), 0, newscene->width(), newscene->height(), graypen);


//    int brick_start = pix_per_brick*chip_border_mm*2;
//    int brick_xnum = (10*chip_length_cm - 2*chip_border_mm)*1000/de_spacing_um;
//    int brick_ynum = (10*chip_width_cm - 2*chip_border_mm)*1000/de_spacing_um;
//    for(int i = 0; i <= brick_xnum; i++)
//        newscene->addLine(brick_start+i*pix_per_brick, brick_start, brick_start+i*pix_per_brick, newscene->height()-brick_start, graypen);
//    for(int i = 0; i <= brick_ynum; i++)
//        newscene->addLine(brick_start, brick_start+i*pix_per_brick, newscene->width()-brick_start, brick_start+i*pix_per_brick, graypen);
//    int brick_start = pix_per_brick*chip_border_mm*2;
    int border_px = chip_border_mm*1000/de_spacing_um*pix_per_brick;
    int brick_xnum = (10*chip_length_cm - 2*chip_border_mm)*1000/de_spacing_um;
    int brick_ynum = (10*chip_width_cm - 2*chip_border_mm)*1000/de_spacing_um;
    int chip_width_px = brick_xnum*pix_per_brick+border_px*2;
    int chip_height_px = brick_ynum*pix_per_brick+border_px*2;
    int brick_x_start;
    int brick_y_start;
    if(tight == 1){
        brick_x_start = (600 - chip_width_px)/2 + border_px;
        brick_y_start = (400 - chip_height_px)/2 + border_px;
    }else{
        brick_x_start = (600 - chip_width_px)/2 + border_px;
        brick_y_start = (400 - chip_height_px)/2 + border_px;
    }
    for(int i = 0; i <= brick_xnum; i++){
        for(int j = 0; j <= brick_ynum; j++){
            newscene->addEllipse(brick_x_start+i*pix_per_brick, brick_y_start+j*pix_per_brick, 1, 1, graypen);
        }
    }

    //chip scale
    int cm_to_px = 10000/de_spacing_um*pix_per_brick;
    for(int i = 1; i < chip_length_cm; i++){
        newscene->addEllipse(brick_x_start-border_px + i*cm_to_px, brick_y_start-border_px+chip_height_px-2, 1, 1, redpen);
    }
    for(int i = 1; i < chip_width_cm; i++){
        newscene->addEllipse(brick_x_start-border_px+2, brick_y_start-border_px + i*cm_to_px, 1, 1, redpen);
    }

    //chip border
    newscene->addRect(brick_x_start-border_px, brick_y_start-border_px, chip_width_px, chip_height_px, redpen);
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
