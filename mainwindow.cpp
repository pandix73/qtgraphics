#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsSvgItem>
#define floatqDebug() qDebug() << fixed << qSetRealNumberPrecision(6)
int screen_width;
int screen_length;

/////////////////////////////////////  CHIP INFO  /////////////////////////////////////
bool deletemode = false;
bool linemode = false;
bool heat_line = false;
bool sense_line = false;
//Chip setting
float chip_length_mm = 76;
float chip_width_mm = 52;
float chip_border_mm = 6;

int chip_length_um;
int chip_width_um;
int chip_border_um;


float cp_length_mm = 8;
float cp_width_mm = 1.34;

int cp_length_um;
int cp_width_um;


float de1_length_mm = 1;
float de1_width_mm = 5;
float de2_length_mm = 3;
float de2_width_mm = 3;

int de1_length_um;
int de1_width_um;
int de2_length_um;
int de2_width_um;

float de_spacing_mm = 0.2;
float cp_spacing_mm = 1.2;
float line_width_mm = 0.353;
//float line_width_mm = 0.706;

int de_spacing_um;
int cp_spacing_um;
int line_width_um;

int line_width_pix = 10;
int line_pix_per_brick;

int sensor_offset_x;
int sensor_offset_y;

int dispenser_offset_up_down;
int dispenser_offset_left_right;

//Chip Parameters
int border_px;
int brick_xnum;
int brick_ynum;
int lbrick_xnum;
int lbrick_ynum;
int chip_width_px;
int chip_length_px;
int brick_x_start;
int brick_y_start;

int pix_per_brick;         //How many PIXELs on the screen should we show to represent de_spacing
int cm_to_px;              //how many PIXELS are in 1 cm
float mm_to_px;
//Unit color
QColor merge_color      = QColor(255, 208, 166, 127);
QColor cycling_color    = QColor(1, 96, 177, 127);
QColor moving_color     = QColor(255, 184, 184, 127);
QColor dispenser_color  = QColor(215, 230, 144, 127);
QColor heat_color       = QColor(108, 137, 147, 127);

//Pen setting
QPen graypen(Qt::gray, 0);
QPen redpen(Qt::red, 0);
//QPen blackpen(Qt::green);
QPen linepen(Qt::gray, 0);
QPen nopen(Qt::NoPen);
QPen outlinepen;


//Brush setting
QBrush nullitem(QColor(94, 94, 94, 54));
QBrush blackbrush(Qt::black);

//Number of units
int num_merge = 0;
int num_dispenser = 0;
int num_move = 0;
int num_cycling = 0;
int num_heater = 0;
int num_sensor = 0;
int num_de = 0;

//unit map
int unitmap[500][500];


float px_to_cm;
float px_to_mm;
/////////////////////////////////////  MAINWINDOW  /////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow)
{
    setAcceptDrops(true);
    ui->setupUi(this);
    this->linescene = this->CreateLineScene();
    this->mainscene = this->CreateNewScene();

    //TOOGLE BUTTON
    SwitchControl *pSwitchControl = new SwitchControl(this->centralWidget());
    pSwitchControl->setFixedWidth(55);
    pSwitchControl->setFixedHeight(28);
    pSwitchControl->move(365, 633);
    mainscene->addWidget(pSwitchControl);
    pSwitchControl->setToggle(false);
    connect(pSwitchControl, SIGNAL(toggled(bool)), this, SLOT(mode_label(bool)));

    //NEW ACTION
    QAction *New = ui->actionNew;
    New->setShortcuts(QKeySequence::New);
    connect(New, SIGNAL(triggered()), this, SLOT(new_chip_clicked()));

    //SAVE ACTION
    QAction *Save = ui->actionSave;
    Save->setShortcuts(QKeySequence::Save);
    connect(Save, SIGNAL(triggered()), this, SLOT(save_svg()));

    //LOAD ACTION
    QAction *Load = ui->actionOpen;
    Load->setShortcuts(QKeySequence::Open);
    connect(Load, SIGNAL(triggered()), this, SLOT(load_svg_clicked()));

    //EXPORT ACTION
    QAction *Export = ui->actionExport;
    Export->setShortcuts(QKeySequence::Italic);
    connect(Export, SIGNAL(triggered()), this, SLOT(export_clicked()));

    //CLOSE ACTION
    QAction *Close = ui->actionClose;
    Close->setShortcut(QKeySequence::Close);
    connect(Close, SIGNAL(triggered()), this, SLOT(close_clicked()));

    //EXPORT PDF ACTION
    QAction *Pdf = ui->actionPdf;
//    Pdf->setShortcuts(QKeySequence::);
    connect(Pdf, SIGNAL(triggered()), this, SLOT(pdf_clicked()));

    ui->view->setScene(mainscene);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::GetScreenSize(int width, int length){
    screen_width = width;
    screen_length = length;
}

void MainWindow::ConvertAllToUM(){
    chip_length_um = chip_length_mm * 1000;
    chip_width_um = chip_width_mm * 1000;
    chip_border_um = chip_border_mm * 1000;

    cp_length_um = cp_length_mm * 1000;
    cp_width_um = cp_width_mm * 1000;

    de1_length_um = de1_length_mm * 1000;
    de1_width_um = de1_width_mm * 1000;
    de2_length_um = de2_length_mm * 1000;
    de2_width_um = de2_width_mm * 1000;

    de_spacing_um = de_spacing_mm * 1000;
    cp_spacing_um = cp_spacing_mm * 1000;
    line_width_um = line_width_mm * 1000;
}

void MainWindow::ChipParameters(){

    pix_per_brick = fmin(760/((chip_length_um/de_spacing_um)),                  //calculate the size of each brick that fits the screeen the most
                         520/((chip_width_um/de_spacing_um)));
    qDebug() << "pix per brick" << pix_per_brick;
    cm_to_px = 10000/de_spacing_um*pix_per_brick;
    mm_to_px = 1000.0/de_spacing_um*pix_per_brick;

//    border_px = chip_border_um/de_spacing_um*pix_per_brick;                        //border width in pixel
//    brick_xnum = (chip_length_um - 2*chip_border_um)/de_spacing_um;             //how many background dots are needed in x-azis
//    brick_ynum = (chip_width_um - 2*chip_border_um)/de_spacing_um;              //how many background dots are needed in y-axis


//    chip_length_px = brick_xnum*pix_per_brick+border_px*2;                              //chip_length in pixel
//    chip_width_px = brick_ynum*pix_per_brick+border_px*2;                               //chip_width in pixel


    chip_length_px = pix_per_brick * chip_length_um/de_spacing_um;
    chip_width_px = pix_per_brick * chip_width_um/de_spacing_um;

    border_px = chip_length_px * chip_border_um / chip_length_um;                        //border width in pixel
    brick_xnum = (chip_length_px - 2*border_px)/pix_per_brick;             //how many background dots are needed in x-azis
    brick_ynum = (chip_width_px - 2*border_px)/pix_per_brick;              //how many background dots are needed in y-axis

    brick_x_start = (760 - chip_length_px)/2 + border_px;                                //where to start drawing dots in y-axis
    brick_y_start = (520 - chip_width_px)/2 + border_px;
    while(brick_x_start%pix_per_brick != 0)
        brick_x_start++;
    while(brick_y_start%pix_per_brick != 0)
        brick_y_start++;
    line_width_pix = pix_per_brick*line_width_um/de_spacing_um;
    line_pix_per_brick = pix_per_brick * 1000 / de_spacing_um;

    px_to_cm = (float)9921/21;
    px_to_mm = (float)9921/210;

}

void MainWindow::OuterBorder(QGraphicsScene *scene){
    //1-1. outer border(nothing to do with real chip size, just the canvas size)
    scene->addLine(0, 0, scene->width(), 0, graypen);
    scene->addLine(0, scene->height(), scene->width(), scene->height(), graypen);
    scene->addLine(0, 0, 0, scene->height(), graypen);
    scene->addLine(scene->width(), 0, scene->width(), scene->height(), graypen);
}

void MainWindow::BackgroundGrid(QGraphicsScene *scene){
    //background grid (in dotted form)
    for(int i = 0; i <= brick_xnum; i+=5){
        for(int j = 0; j <= brick_ynum; j+=5){
            scene->addEllipse(brick_x_start+i*pix_per_brick, brick_y_start+j*pix_per_brick, 0.5, 0.5, graypen);
        }
    }
    //QPen pen;
    //QVector<qreal> dashes;
    //qreal space = pix_per_brick;
    //dashes << 0.1 << space;
    //pen.setDashPattern(dashes);
    //for(int i = 0; i<= brick_xnum; i++){
    //    scene->addLine(brick_x_start+i*pix_per_brick, brick_y_start, brick_x_start+i*pix_per_brick, brick_y_start+brick_ynum*pix_per_brick, pen);
    //}
}

void MainWindow::LineBackgroundGrid(QGraphicsScene *scene){
    //line background grid (in dotted form)
    //double line_pix_per_brick = double(pix_per_brick)*line_width_um/de_spacing_um;
    double line_pix_per_brick = double(pix_per_brick)*1000/de_spacing_um;
    int line_brick_xnum = double(brick_xnum)*de_spacing_um/1000;
    int line_brick_ynum = double(brick_ynum)*de_spacing_um/1000;
    for(int i = 0; i <= line_brick_xnum; i+=1){
        for(int j = 0; j <= line_brick_ynum; j+=1){
            scene->addEllipse(brick_x_start+i*line_pix_per_brick, brick_y_start+j*line_pix_per_brick, 0.5, 0.5, graypen);
        }
    }
}

void MainWindow::ChipScaleDots(QGraphicsScene *scene){
    //chip scale                                                                        //show dots of scale near the chip border, each interval stands for 1cm in real size
    for(int i = 10; i < chip_length_mm; i+=10){
        scene->addEllipse(brick_x_start-border_px + i*mm_to_px, brick_y_start-border_px+chip_width_px-2, 1, 1, redpen);
    }
    for(int i = 10; i < chip_width_mm; i+=10){
        scene->addEllipse(brick_x_start-border_px+2, brick_y_start-border_px + i*mm_to_px, 1, 1, redpen);
    }
}

void MainWindow::ChipBorder(QGraphicsScene *scene){
    //chip border
    scene->addRect(brick_x_start-border_px, brick_y_start-border_px, chip_length_px, chip_width_px, redpen);    //the exact border of the chip
}

void MainWindow::ChipScale(QGraphicsScene *scene){
    //scale item                                                                     //show a 1cm scale
    scene->addRect(brick_x_start - border_px + (chip_length_mm-10) * cm_to_px/10 , 460, cm_to_px, 2, redpen);
    QGraphicsTextItem *text = scene->addText("1cm");
    text->setPos(brick_x_start - border_px + (chip_length_mm-10) * cm_to_px/10 + (cm_to_px/2) - 12, 463);
}

void MainWindow::DefaultControlPad(QGraphicsScene *scene){
    float cp_length = float(cp_length_um) / de_spacing_um;
    float cp_width = float(cp_width_um) / de_spacing_um;
    float cp_space = float(cp_spacing_um) / de_spacing_um;
    float cp_xi_start = brick_x_start / pix_per_brick;
    float cp_xi_start_right = brick_x_start / pix_per_brick + brick_xnum  - cp_length;
    float cp_yi_start = brick_y_start / pix_per_brick;

    int num_control_pad = brick_ynum / (cp_width + cp_space);
    for(int i=0; i<=num_control_pad; i++){
        unit *cp = new unit();
        cp->setEnabled(false);
        cp->type = "controlpad";
        cp->xi = cp_xi_start;
        cp->yi = cp_yi_start;
        cp->length = cp_length;
        cp->width = cp_width;
        cp->actual_length = cp_length_mm;
        cp->actual_width = cp_width_mm;
        cp->color = Qt::black;
        scene->addItem(cp);
        allunits.prepend(cp);
        connect(cp, SIGNAL(delete_this_item(unit *)), this, SLOT(delete_from_list(unit *)));

        unit *cp_right = new unit();
        cp_right->setEnabled(false);
        cp_right->type = "controlpad";
        cp_right->xi = cp_xi_start_right;
        cp_right->yi = cp_yi_start;
        cp_right->length = cp_length;
        cp_right->width = cp_width;
        cp_right->actual_length = cp_length_mm;
        cp_right->actual_width = cp_width_mm;
        cp_right->color = Qt::black;
        scene->addItem(cp_right);
        allunits.prepend(cp_right);
        cp_yi_start += cp_width + cp_space;
        connect(cp_right, SIGNAL(delete_this_item(unit *)), this, SLOT(delete_from_list(unit *)));
    }
}

void MainWindow::Info(){
    //Information
    ui->chip_length_label->setText(QString::number(chip_length_mm));
    ui->chip_width_label->setText(QString::number(chip_width_mm));
    ui->chip_border_label->setText(QString::number(chip_border_mm));
    ui->line_width_label->setText(QString::number(line_width_mm));
    ui->cp_length_label->setText(QString::number(cp_length_mm));
    ui->cp_width_label->setText(QString::number(cp_width_mm));
    ui->cp_sapcing_label->setText(QString::number(cp_spacing_mm));
    ui->de1_length_label->setText(QString::number(de1_length_mm));
    ui->de1_width_label->setText(QString::number(de1_width_mm));
    ui->de2_length_label->setText(QString::number(de2_length_mm));
    ui->de2_width_label->setText(QString::number(de2_width_mm));
    ui->de_spacing->setText(QString::number(de_spacing_mm));

    ui->num_merge->setText(QString::number(num_merge));
    ui->num_dispenser->setText(QString::number(num_dispenser));
    ui->num_move->setText(QString::number(num_move));
    ui->num_cycling->setText(QString::number(num_cycling));
    ui->num_heater->setText(QString::number(num_heater));
    ui->num_sensor->setText(QString::number(num_sensor));
}

void MainWindow::EmptyMessage(){
    ui->message->setStyleSheet(" font: 15pt American Typewriter; color: rgb(187, 210, 211);");
    ui->message->setText("");
}

void MainWindow::WarningMessage(QString message){
    ui->message->setStyleSheet("background-color : red; color : white;  font: 15pt American Typewriter; margin-left : 3px;");
    ui->message->setText(message);
}

//Create New Scene
QGraphicsScene* MainWindow::CreateNewScene(){
    QGraphicsScene * scene = new QGraphicsScene(0, 0, 760, 520, ui->view);
    scene->setBackgroundBrush(Qt::white);

    //Draw chip basics
    ConvertAllToUM();
    ChipParameters();
    OuterBorder(scene);
    BackgroundGrid(scene);
    ChipScaleDots(scene);
    ChipBorder(scene);
    ChipScale(scene);
    DefaultControlPad(scene);

    //Update Info Board
    Info();
    zoom_factor /= 1.1;
    ui->view->setTransform(QTransform().scale(zoom_factor, zoom_factor));
    return scene;
}

graphicsscene* MainWindow::CreateLineScene(){
    graphicsscene* scene = new graphicsscene(ui->view);                                 //create a new scene
    scene->setSceneRect(QRectF(0, 0, 760, 520));
    scene->setBackgroundBrush(Qt::white);

    //Draw chip basics
    ConvertAllToUM();
    ChipParameters();
    OuterBorder(scene);
    LineBackgroundGrid(scene);
    ChipScaleDots(scene);
    ChipBorder(scene);
    ChipScale(scene);
    //DefaultControlPad(scene);

    return scene;
}

void MainWindow::EnableCreateUnit(bool enable){
    ui->merge_create->setEnabled(enable);
    ui->dispenser_create->setEnabled(enable);
    ui->move_create->setEnabled(enable);
    ui->cycling_create->setEnabled(enable);
    ui->heater_create->setEnabled(enable);
}

//MODE LABEL                                                                            //shows the mode its in
void MainWindow::mode_label(bool bChecked){
    /////// LINE MODE ///////
    if(bChecked){
        deletemode = false;
        ui->eraser->setStyleSheet("background-color: rgb(42, 48, 58);");
        ui->view->setCursor(Qt::ArrowCursor);
        for(unit *item : allunits){
            if(item->type == "move"){                   //Show detail components for "move"
                if(item->tilt == 90){
                    for(int i = 0; i < item->de_ynum; i++){
                        DestroyRect << linescene->addRect(item->xi*pix_per_brick,
                                   (item->yi+i*(item->child_length + item->child_gap))*pix_per_brick,
                                   pix_per_brick*item->child_width,
                                   pix_per_brick*item->child_length,
                                   redpen, nullitem);
                    }
                }
                else{
                    for(int i = 0; i < item->de_xnum; i++){
                        DestroyRect << linescene->addRect((item->xi+i*(item->child_length + item->child_gap))*pix_per_brick,
                                   item->yi*pix_per_brick,
                                   pix_per_brick*item->child_length,
                                   pix_per_brick*item->child_width,
                                   redpen, nullitem);
                    }
                }
            }
            else if(item->type == "merge"){
                //LEFT
                int i = 0;
                int middle = item->xi + item->length/2;
                while(middle+1 >= item->xi + i*8 + 15){
                     DestroyRect << linescene->addRect((item->xi+i*8)*pix_per_brick,
                                (item->yi+i*6)*pix_per_brick,
                                pix_per_brick*15,
                                pix_per_brick*5,
                                redpen, nullitem);
                    i++;
                }
                //RIGHT
                i = 0;
                int end = item->xi + item->length;
                while(middle+-1 <= end - i*8 - 15){
                     DestroyRect << linescene->addRect((end-i*8-15)*pix_per_brick,
                                (item->yi+i*6)*pix_per_brick,
                                pix_per_brick*15,
                                pix_per_brick*5,
                                redpen, nullitem);
                    i++;
                }
                //MIDDLE
                while(i*6 < item->width){
                    DestroyRect << linescene->addRect((middle - 8)*pix_per_brick,
                                (item->yi+i*6)*pix_per_brick,
                                pix_per_brick*15,
                                pix_per_brick*5,
                                redpen, nullitem);
                    i++;
                }
            }
            else if(item->type == "cycle"){             //Show detail components for 'cycling"
                for(int i = 0; i < item->de_xnum; i++){
                    for(int j = 0; j < item->de_ynum; j++){
                        if(i!=0 && i!=item->de_xnum-1 && j!=0 && j!=item->de_ynum-1) continue;
                            DestroyRect << linescene->addRect((item->xi+i*(de2_length_um/de_spacing_um + 1))*pix_per_brick,
                                      (item->yi+j*(de2_length_um/de_spacing_um + 1))*pix_per_brick,
                                      pix_per_brick*de2_length_um/de_spacing_um,
                                      pix_per_brick*de2_width_um/de_spacing_um,
                                      redpen, nullitem);
                    }
                }
            }
            else if(item->type == "heat"){
                int layer = item->zigzag_layer;
                double zigzag_width_um = item->zigzag_linewidth;
                double spacing_um = int((item->length*de_spacing_um-2*zigzag_width_um*layer)/(2*layer-1));
                double connect_line_um = 353;
                //draw parallel lines
                for(int i = 0; i < 2*layer; i++){
                    if(item->tilt == 0 || item->tilt == 180){
                        if (item->tilt == 0 && i == 2*layer-1){
                            DestroyRect << linescene->addRect((item->xi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick, (item->yi+(!item->clockwise)*item->width/2)*pix_per_brick, zigzag_width_um/de_spacing_um*pix_per_brick, item->width/2*pix_per_brick, redpen, nullitem);
                        } else if (item->tilt == 180 && i == 0){
                            DestroyRect << linescene->addRect((item->xi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick, (item->yi+(item->clockwise)*item->width/2)*pix_per_brick, zigzag_width_um/de_spacing_um*pix_per_brick, item->width/2*pix_per_brick, redpen, nullitem);
                        } else if (i != 0 && i != 2*layer-1){
                            int connect_line_space = ((item->tilt == 0 && item->clockwise == 0) || (item->tilt == 180 && item->clockwise == 1)) ? (2*connect_line_um+zigzag_width_um)/de_spacing_um*pix_per_brick : 0;
                            DestroyRect << linescene->addRect((item->xi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick, item->yi*pix_per_brick+connect_line_space, zigzag_width_um/de_spacing_um*pix_per_brick, (item->width-(2*connect_line_um+zigzag_width_um)/de_spacing_um)*pix_per_brick, redpen, nullitem);
                        } else
                            DestroyRect << linescene->addRect((item->xi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick, item->yi*pix_per_brick, zigzag_width_um/de_spacing_um*pix_per_brick, item->width*pix_per_brick, redpen, nullitem);
                    } else if (item->tilt == 90 || item->tilt == 270){
                        if (item->tilt == 90 && i == 2*layer-1){
                            DestroyRect << linescene->addRect((item->xi+(item->clockwise)*item->length/2)*pix_per_brick, (item->yi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick, item->length/2*pix_per_brick, zigzag_width_um/de_spacing_um*pix_per_brick, redpen, nullitem);
                        } else if (item->tilt == 270 && i == 0){
                            DestroyRect << linescene->addRect((item->xi+(!item->clockwise)*item->length/2)*pix_per_brick, (item->yi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick, item->length/2*pix_per_brick, zigzag_width_um/de_spacing_um*pix_per_brick, redpen, nullitem);
                        } else if (i != 0 && i != 2*layer-1){
                            int connect_line_space = ((item->tilt == 90 && item->clockwise == 1) || (item->tilt == 270 && item->clockwise == 0)) ? (2*connect_line_um+zigzag_width_um)/de_spacing_um*pix_per_brick : 0;
                            DestroyRect << linescene->addRect(item->xi*pix_per_brick+connect_line_space, (item->yi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick, (item->length-(2*connect_line_um+zigzag_width_um)/de_spacing_um)*pix_per_brick, zigzag_width_um/de_spacing_um*pix_per_brick, redpen, nullitem);
                        } else
                            DestroyRect << linescene->addRect(item->xi*pix_per_brick, (item->yi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick, item->length*pix_per_brick, zigzag_width_um/de_spacing_um*pix_per_brick, redpen, nullitem);
                    }
                }
                for(int i = 0; i < 2*layer-1; i++){
                    if(item->tilt == 0 || item->tilt == 180){
                        bool t = (item->tilt == 0) ? item->clockwise == i%2 : !item->clockwise == i%2;
                        int connect_line_space = ((item->tilt == 0 && item->clockwise == 0) || (item->tilt == 180 && item->clockwise == 1)) ? (2*connect_line_um+zigzag_width_um)/de_spacing_um*pix_per_brick : 0;
                        DestroyRect << linescene->addRect((item->xi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick,
                                          item->yi*pix_per_brick+t*(item->width-2*(connect_line_um+zigzag_width_um)/de_spacing_um)*pix_per_brick + connect_line_space,
                                          (zigzag_width_um+spacing_um)/de_spacing_um*pix_per_brick,
                                          zigzag_width_um/de_spacing_um*pix_per_brick,
                                          redpen, nullitem);
                    } else if (item->tilt == 90 || item->tilt == 270){
                        bool t = (item->tilt == 90) ? !item->clockwise == i%2 : item->clockwise == i%2;
                        int connect_line_space = ((item->tilt == 90 && item->clockwise == 1) || (item->tilt == 270 && item->clockwise == 0)) ? (2*connect_line_um+zigzag_width_um)/de_spacing_um*pix_per_brick : 0;
                        DestroyRect << linescene->addRect(item->xi*pix_per_brick+t*(item->length-2*(connect_line_um+zigzag_width_um)/de_spacing_um)*pix_per_brick + connect_line_space,
                                          (item->yi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick,
                                          zigzag_width_um/de_spacing_um*pix_per_brick,
                                          (zigzag_width_um+spacing_um)/de_spacing_um*pix_per_brick,
                                          redpen, nullitem);
                    }
                }
                if ((item->tilt == 0 && item->clockwise == 0) || (item->tilt == 180 && item->clockwise == 1))
                    DestroyRect << linescene->addRect(item->xi*pix_per_brick, item->yi*pix_per_brick, item->length*pix_per_brick, connect_line_um*2/de_spacing_um*pix_per_brick, redpen, nullitem);
                else if ((item->tilt == 0 && item->clockwise == 1) || (item->tilt == 180 && item->clockwise == 0))
                    DestroyRect << linescene->addRect(item->xi*pix_per_brick, (item->yi+item->width-2*connect_line_um/de_spacing_um)*pix_per_brick, item->length*pix_per_brick, connect_line_um*2/de_spacing_um*pix_per_brick, redpen, nullitem);
                else if ((item->tilt == 90 && item->clockwise == 1) || (item->tilt == 270 && item->clockwise == 0))
                    DestroyRect << linescene->addRect(item->xi*pix_per_brick, item->yi*pix_per_brick, connect_line_um*2/de_spacing_um*pix_per_brick, item->width*pix_per_brick, redpen, nullitem);
                else if ((item->tilt == 90 && item->clockwise == 0) || (item->tilt == 270 && item->clockwise == 1))
                    DestroyRect << linescene->addRect((item->xi+item->length-2*connect_line_um/de_spacing_um)*pix_per_brick, item->yi*pix_per_brick, connect_line_um*2/de_spacing_um*pix_per_brick, item->width*pix_per_brick, redpen, nullitem);
            }
            else if(item->type == "sensor"){
                DestroyRect << linescene->addRect(item->xi*pix_per_brick, item->yi*pix_per_brick, item->length*pix_per_brick, item->width*pix_per_brick, redpen, nullitem);
                DestroyRect << linescene->addRect(item->xi*pix_per_brick+sensor_offset_x, item->yi*pix_per_brick+sensor_offset_y, item->child_length*pix_per_brick, item->child_width*pix_per_brick, redpen, blackbrush);
            }
            else if(item->type == "dispenser"){
                switch(item->direction){
                    case 0 :    //up
                        for(int i = 0; i < item->de_xnum; i++){
                            DestroyRect << linescene->addRect((item->xi+dispenser_offset_up_down)*pix_per_brick,
                                (item->yi+(i*(item->child_length+item->child_gap)))*pix_per_brick,
                                pix_per_brick*item->child_width,
                                pix_per_brick*item->child_length,
                                redpen, nullitem);
                        }
                        DestroyRect << linescene->addRect(item->xi*pix_per_brick,
                                (item->yi+(item->de_xnum*(item->child_length+item->child_gap)))*pix_per_brick,
                                item->main_length*pix_per_brick,
                                item->main_width*pix_per_brick,
                                redpen, nullitem);
                        break;
                    case 1:     //right
                        DestroyRect << linescene->addRect(item->xi*pix_per_brick,
                                item->yi*pix_per_brick,
                                item->main_length*pix_per_brick,
                                item->main_width*pix_per_brick,
                                redpen, nullitem);
                        for(int i = 0; i < item->de_xnum; i++){
                            DestroyRect << linescene->addRect((item->xi+item->main_length+(i*(item->child_length+item->child_gap))+item->child_gap)*pix_per_brick,
                                (item->yi+dispenser_offset_left_right)*pix_per_brick,
                                pix_per_brick*item->child_length,
                                pix_per_brick*item->child_width,
                                redpen, nullitem);
                        }
                        break;
                    case 2:     //down
                        DestroyRect << linescene->addRect(item->xi*pix_per_brick,
                            item->yi*pix_per_brick,
                            item->main_length*pix_per_brick,
                            item->main_width*pix_per_brick,
                            redpen, nullitem);
                        for(int i = 0; i < item->de_xnum; i++){
                            DestroyRect << linescene->addRect((item->xi+dispenser_offset_up_down)*pix_per_brick,
                                (item->yi+item->main_width+(i*(item->child_length+item->child_gap))+item->child_gap)*pix_per_brick,
                                pix_per_brick*item->child_width,
                                pix_per_brick*item->child_length,
                                redpen, nullitem);
                        }
                        break;
                    case 3:     //left
                        for(int i = 0; i < item->de_xnum; i++){
                            DestroyRect << linescene->addRect((item->xi+(i*(item->child_length+item->child_gap)))*pix_per_brick,
                                (item->yi+dispenser_offset_left_right)*pix_per_brick,
                                pix_per_brick*item->child_length,
                                pix_per_brick*item->child_width,
                                redpen, nullitem);
                        }
                        DestroyRect << linescene->addRect((item->xi+(item->de_xnum*(item->child_length+item->child_gap)))*pix_per_brick,
                                item->yi*pix_per_brick,
                                item->main_length*pix_per_brick,
                                item->main_width*pix_per_brick,
                                redpen, nullitem);
                        break;
                }
            }
            else
                DestroyRect << linescene->addRect(item->xi*pix_per_brick, item->yi*pix_per_brick, item->length*pix_per_brick, item->width*pix_per_brick, redpen, nullitem);
        }
        ui->view->setScene(linescene);
        linemode = true;
        ui->mode_label->setText("LINE MODE");
        QPixmap *e = new QPixmap(":/MainWindow/Icons/Icons/cursor_reddot.png");
        QCursor pencil = QCursor(*e, -10, -10);
        ui->view->setCursor(pencil);
    }
    /////// UNIT MODE ///////
    else{
        EmptyMessage();
        deletemode = false;
        ui->eraser->setStyleSheet("background-color: rgb(42, 48, 58);");
        ui->view->setCursor(Qt::ArrowCursor);

        for(QGraphicsItem* rect: DestroyRect){
             delete rect;
        }
        for(QGraphicsLineItem* line: DestroyLine){
            delete line;
        }
        DestroyRect.clear();        
        DestroyLine.clear();
        linepen.setColor(Qt::gray);
        linepen.setWidth(line_width_pix);
        for(line *head : linescene->alllines){
            if(head->heater_line)linepen.setWidth(line_width_pix*2);
            line *current_seg = head;
            while(current_seg->next != NULL){
                DestroyLine << mainscene->addLine(current_seg->x[0], current_seg->y[0], current_seg->x[1], current_seg->y[1], linepen);
                current_seg = current_seg->next;
            }
            DestroyLine << mainscene->addLine(current_seg->x[0], current_seg->y[0], current_seg->x[1], current_seg->y[1], linepen);
            if(head->heater_line)linepen.setWidth(line_width_pix);
        }
        ui->view->setScene(mainscene);
        linemode = false;
        ui->mode_label->setText("UNIT MODE");
        ui->view->setCursor(Qt::ArrowCursor);
    }
}

////////////////////////////////////////  ACTIONS  /////////////////////////////////////
void MainWindow::new_chip_clicked()
{
    this->save_svg();
    QGraphicsScene *newscene = CreateNewScene();
    delete(ui->view->scene());
    allunits.clear();
    this->mainscene = newscene;
    ui->view->setScene(newscene);
}

//SAVE CHIP
void MainWindow::save_svg()
{
     QString newPath = QFileDialog::getSaveFileName(this, "Save Chip .txt", path, tr("TXT files (*.txt)"));
     if(newPath.isEmpty()) return;

      QFile file(newPath);
      if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
          return;

      QTextStream out(&file);

      out << chip_length_mm << "\n" << chip_width_mm << "\n" << chip_border_mm << "\n";
      out << cp_length_mm << "\n" << cp_width_mm << "\n";
      out << de1_length_mm << "\n" << de1_width_mm << "\n" << de2_length_mm  << "\n"<< de2_width_mm << "\n";

      out << de_spacing_mm << "\n";
      out << cp_spacing_mm << "\n";
      out << line_width_mm  << "\n";

      out << num_merge << "\n" << num_dispenser << "\n" << num_move << "\n" << num_cycling << "\n" <<num_heater << "\n" <<num_sensor << "\n" << num_de << "\n";
      out << cp_length_mm << "\n" << cp_width_mm << "\n" << cp_spacing_mm << "\n";
      out << allunits.size() << "\n";
      for(unit *item: allunits){
        out << item->type << "\n";
        out << item->xi << "\n" << item->yi  << "\n";
        if(item->type == "move" || item->type == "cycle"){
            out << item->de_type << "\n" << item->de_xnum << "\n" << item->de_ynum << "\n";
        }
        if(item->type == "move"){
            out << item->tilt << "\n";
        }
        if(item->type == "heat"){
            out << item->tilt << "\n";
            out << item->zigzag_linewidth << "\n";
            out << item->zigzag_layer << "\n";
        }
        out << item->length << "\n" << item->width << "\n";
        out << item->color.red() << "\n" << item->color.green() << "\n" << item->color.blue() << "\n" << item->color.alpha() << "\n";
      }

      out << linescene->alllines.size() << "\n";

      line *current_seg;
      for(line *head : linescene->alllines){
          current_seg = head;
          while(current_seg->next != nullptr){
              out << current_seg->x[0] << "\n" << current_seg->y[0] << "\n";
              current_seg = current_seg->next;
          }
          out << current_seg->x[0] << "\n" << current_seg->y[0] << "\n";
          out << current_seg->x[1] << "\n" << current_seg->y[1] << "\n";
          out << 1000 << "\n";  //END LINE
      }
}

void MainWindow::load_svg_clicked()
{
    DestroyRect.clear();
    allunits.clear();
    QString newPath = QFileDialog::getOpenFileName(this, "Open Chip", path, tr("TXT files (*.txt)"));
    if(newPath.isEmpty())return;

    QFile file(newPath);
       if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
           return;

    QTextStream in(&file);

    //Import Chip
    chip_length_mm = in.readLine().toFloat();
    chip_width_mm = in.readLine().toFloat();
    chip_border_mm = in.readLine().toFloat();
    cp_length_mm = in.readLine().toFloat();
    cp_width_mm = in.readLine().toFloat();

    de1_length_mm = in.readLine().toFloat();
    de1_width_mm = in.readLine().toFloat();
    de2_length_mm  = in.readLine().toFloat();
    de2_width_mm = in.readLine().toFloat();

    de_spacing_mm = in.readLine().toFloat();
    cp_spacing_mm = in.readLine().toFloat();
    line_width_mm  = in.readLine().toFloat();
    //line_width_pix = in.readLine().toFloat();

    num_merge = in.readLine().toInt();
    num_dispenser = in.readLine().toInt();
    num_move = in.readLine().toInt();
    num_cycling = in.readLine().toInt();
    num_heater = in.readLine().toInt();
    num_sensor = in.readLine().toInt();
    num_de = in.readLine().toInt();
    cp_length_mm = in.readLine().toFloat();
    cp_width_mm = in.readLine().toFloat();
    cp_spacing_mm = in.readLine().toFloat();

    //Set Chip to mainscene
    mainscene = CreateNewScene();
    linescene = CreateLineScene();

    //Import Units
    int num_of_units = in.readLine().toInt();
    while(num_of_units){
        unit *item = new unit();
        item->type = in.readLine();
        item->xi = in.readLine().toInt();
        item->yi = in.readLine().toInt();
        if(item->type == "move" || item->type == "cycle"){
            item->de_type = in.readLine().toInt();
            item->de_xnum = in.readLine().toInt();
            item->de_ynum = in.readLine().toInt();
        }
        if(item->type == "move"){
            item->tilt = in.readLine(). toInt();
        }
        if(item->type == "heat"){
            item->tilt = in.readLine(). toInt();
            item->zigzag_linewidth = in.readLine(). toInt();
            item->zigzag_layer = in.readLine(). toInt();
        }
        item->length = in.readLine().toInt();
        item->width = in.readLine().toInt();
        int R, G, B, Alpha;
        R = in.readLine().toInt();
        G = in.readLine().toInt();
        B = in.readLine().toInt();
        Alpha = in.readLine().toInt();
        item->color = QColor(R, G, B, Alpha);
        mainscene->addItem(item);
        allunits.prepend(item);
        connect(item, SIGNAL(delete_this_item(unit *)), this, SLOT(delete_from_list(unit *)));
        num_of_units--;
    }
    //Import Lines
    int num_of_lines = in.readLine().toInt();
    while(num_of_lines){
        line *head = new line();
        line *current_seg = new line();

        bool ishead = true;
        float x, y;
        while(1){
            x = in.readLine().toFloat();
            if(x == 1000.00){
                break;
            }
            y = in.readLine().toFloat();
            line *new_seg = new line();
            new_seg->x[0] = x;
            new_seg->y[0] = y;
            if(ishead){
                new_seg->previous = NULL;
                new_seg->next = NULL;
                head = new_seg;
                current_seg = new_seg;
                ishead = false;
            }
            else{
                current_seg->x[1] = x;
                current_seg->y[1] = y;
                current_seg->next = new_seg;
                new_seg->previous = current_seg;
                current_seg = new_seg;
            }
        }
        current_seg = current_seg->previous;
        current_seg->next = NULL;

        linescene->AddTurnline(head);
    num_of_lines--;
    }

    linepen.setColor(Qt::gray);
    linepen.setWidth(line_width_pix);

    for(line *head : linescene->alllines){
        if(head->heater_line)linepen.setWidth(line_width_pix*2);
        line *current_seg = head;
        while(current_seg->next != NULL){
            DestroyLine << mainscene->addLine(current_seg->x[0], current_seg->y[0], current_seg->x[1], current_seg->y[1], linepen);
            current_seg = current_seg->next;
        }
        DestroyLine << mainscene->addLine(current_seg->x[0], current_seg->y[0], current_seg->x[1], current_seg->y[1], linepen);
        if(head->heater_line)linepen.setWidth(line_width_pix);
    }

    ui->view->setScene(mainscene);
}

//EXPORT AI
void MainWindow::export_clicked()
{
    QString newPath = QFileDialog::getSaveFileName(this, "Save Ai", path, tr("AI files (*.ai)"));
    if (newPath.isEmpty()) return;
    path = newPath;

    QPrinter printer( QPrinter::HighResolution );
    printer.setPageSize( QPrinter::A4 );
    printer.setOrientation( QPrinter::Portrait );
    printer.setOutputFormat( QPrinter::NativeFormat );
    printer.setOutputFileName(path); // file will be created in your build directory (where debug/release directories are)

    QPainter p;
    p.setPen(Qt::NoPen);
   if( !p.begin( &printer ) )
   {
       qDebug() << "Error!";
       return;
   }


   // reset all sizes to REAL SIZE
   QGraphicsScene *export_scene = new QGraphicsScene(0, 0, printer.pageRect().width(), printer.pageRect().height());
   QGraphicsScene *second_export_scene = new QGraphicsScene(0, 0, printer.pageRect().width(), printer.pageRect().height());

   bool second_layer = true;
   float scale = de_spacing_um * px_to_mm / 1000;

   // border in REAL SIZE
   export_scene->addRect(0, 0, (float)chip_length_mm*px_to_mm, (float)chip_width_mm*px_to_mm, graypen);

   //lines in REAL SIZE
   linepen.setColor(Qt::black);
   linepen.setWidth(line_width_um * px_to_mm  / 1000);
   for(line *head : linescene->alllines){
       if(head->heater_line)linepen.setWidth(2 * line_width_um * px_to_mm / 1000);
       line *current_seg = head;
       while(current_seg->next != NULL){
           if(current_seg->sensor_line){
               second_export_scene->addLine(current_seg->x[0] * scale / pix_per_brick, current_seg->y[0]* scale / pix_per_brick, current_seg->x[1]* scale / pix_per_brick, current_seg->y[1]* scale / pix_per_brick, redpen);
           }
           else
               export_scene->addLine(current_seg->x[0] * scale / pix_per_brick, current_seg->y[0]* scale / pix_per_brick, current_seg->x[1]* scale / pix_per_brick, current_seg->y[1]* scale / pix_per_brick, linepen);
           current_seg = current_seg->next;
       }
       if(current_seg->sensor_line){
           for(unit *check_cp : allunits){
               if(check_cp->type == "controlpad"){
                   qDebug() << check_cp->yi*pix_per_brick << current_seg->y[1] << (check_cp->yi+check_cp->width)*pix_per_brick;
                   if(current_seg->y[1] >= check_cp->yi*pix_per_brick && current_seg->y[1] < (check_cp->yi+check_cp->width)*pix_per_brick){
                       second_export_scene->addRect(check_cp->xi * scale,
                                 check_cp->yi * scale,
                                 px_to_mm*check_cp->actual_length,
                                 px_to_mm*check_cp->actual_width,
                                 nopen, blackbrush);
                   }
               }
           }
//           for(int j=0; j<cp_width; j++){
//               sensor_line_map[0][(int)cp->yi+j] = 1;
//               sensor_line_map[1][(int)cp->yi+j] = 1;
//           }
           second_export_scene->addLine(current_seg->x[0]* scale / pix_per_brick, current_seg->y[0]* scale / pix_per_brick, current_seg->x[1]* scale / pix_per_brick, current_seg->y[1]* scale / pix_per_brick, redpen);
       }
       else
           export_scene->addLine(current_seg->x[0]* scale / pix_per_brick, current_seg->y[0]* scale / pix_per_brick, current_seg->x[1]* scale / pix_per_brick, current_seg->y[1]* scale / pix_per_brick, linepen);
       if(head->heater_line)linepen.setWidth(line_width_um * px_to_mm / 1000);
   }


   //units in REAL SIZE
   for(unit *item : allunits){
        if(item->type == "move"){
            if(item->tilt == 90){
                for(int i = 0; i < item->de_ynum; i++){
                    export_scene->addRect(item->xi * scale,
                               ((item->yi+i*(item->child_length + 1)) - (float)(brick_y_start-border_px)/pix_per_brick )* scale,
                               px_to_mm*item->child_width*de_spacing_um/1000,
                               px_to_mm*item->child_length*de_spacing_um/1000,
                               nopen, blackbrush);
//                    previewscene->addRect(item->xi*pix_per_brick,
//                               (item->yi+i*(item->child_length + 1))*pix_per_brick,
//                               pix_per_brick*item->child_width,
//                               pix_per_brick*item->child_length,
//                               nopen, nullitem);
                }
            }
            else {
                for(int i = 0; i < item->de_xnum; i++){
                    export_scene->addRect(((item->xi+i*(item->child_length + 1)) - (float)(brick_x_start-border_px)/pix_per_brick )* (float)de_spacing_um* px_to_cm  / 10000,
                               item->yi* scale,
                               px_to_mm*item->child_length*de_spacing_um/1000,
                               px_to_mm*item->child_width*de_spacing_um/1000,
                               nopen, blackbrush);
                }
            }
        }
        else if(item->type == "dispenser"){
            switch(item->direction){
                case 0 :
                    for(int i = 0; i < item->de_xnum; i++){
                        export_scene->addRect((item->xi+dispenser_offset_up_down)*scale,
                            (item->yi+(i*(item->child_length+item->child_gap)))*scale,
                            px_to_mm*item->child_width*de_spacing_um/1000,
                            px_to_mm*item->child_length*de_spacing_um/1000,
                            nopen, blackbrush);
                    }
                    export_scene->addRect(item->xi*scale,
                            (item->yi+(item->de_xnum*(item->child_length+item->child_gap)))*scale,
                            px_to_mm*item->main_length*de_spacing_um/1000,
                            px_to_mm*item->main_width*de_spacing_um/1000,
                            nopen, blackbrush);
                break;
                case 1 :    //up
                    export_scene->addRect(item->xi*scale,
                                item->yi*scale,
                                px_to_mm*item->actual_length,
                                px_to_mm*item->actual_width,
                                nopen, blackbrush);
                    for(int i = 0; i < item->de_xnum; i++){
                        export_scene->addRect((item->xi+item->main_length+(i*(item->child_length+item->child_gap))+item->child_gap)*scale,
                                (item->yi+dispenser_offset_left_right)*scale,
                                px_to_mm*item->child_length*de_spacing_um/1000,
                                px_to_mm*item->child_width*de_spacing_um/1000,
                                nopen, blackbrush);
                    }
                break;
                case 2:     //down
                    export_scene->addRect(item->xi*scale,
                            item->yi*scale,
                            px_to_mm*item->actual_length,
                            px_to_mm*item->actual_width,
                            nopen, blackbrush);
                    for(int i = 0; i < item->de_xnum; i++){
                        export_scene->addRect((item->xi+dispenser_offset_up_down)*scale,
                            (item->yi+item->main_width+(i*(item->child_length+item->child_gap))+item->child_gap)*scale,
                            px_to_mm*item->child_width*de_spacing_um/1000,
                            px_to_mm*item->child_length*de_spacing_um/1000,
                            nopen, blackbrush);
                    }
                break;
                case 3:     //left
                    for(int i = 0; i < item->de_xnum; i++){
                        export_scene->addRect((item->xi+(i*(item->child_length+item->child_gap)))*scale,
                            (item->yi+dispenser_offset_left_right)*scale,
                            px_to_mm*item->child_length*de_spacing_um/1000,
                            px_to_mm*item->child_width*de_spacing_um/1000,
                            nopen, blackbrush);
                    }
                    export_scene->addRect((item->xi+(item->de_xnum*(item->child_length+item->child_gap)))*scale,
                            item->yi*scale,
                            px_to_mm*item->actual_length,
                            px_to_mm*item->actual_width,
                            nopen, blackbrush);
                break;
            }
        }
        else if(item->type == "cycle"){
            for(int i = 0; i < item->de_xnum; i++){
                for(int j = 0; j < item->de_ynum; j++){
                    if(i!=0 && i!=item->de_xnum-1 && j!=0 && j!=item->de_ynum-1) continue;
                        export_scene->addRect(((item->xi+i*(de2_length_um/de_spacing_um + 1))- (brick_x_start-border_px)/pix_per_brick)* de_spacing_um* px_to_cm  / 10000,
                                  (item->yi+j*(de2_length_um/de_spacing_um + 1)) * scale,
                                  px_to_mm*de2_length_mm,
                                  px_to_mm*de2_length_mm,
                                  nopen, blackbrush);
                }
            }
        }
        else{
            export_scene->addRect(item->xi * scale,
                      item->yi * scale,
                      px_to_mm*item->actual_length,
                      px_to_mm*item->actual_width,
                      nopen, blackbrush);
        }

        if(item->type == "sensor"){
            second_layer = true;
            second_export_scene->addRect((item->xi+sensor_offset_x/pix_per_brick) * scale,
                     (item->yi+sensor_offset_y/pix_per_brick) * scale,
                     px_to_mm*item->child_length*de_spacing_um/1000,
                     px_to_mm*item->child_width*de_spacing_um/1000,
                     nopen, QBrush(Qt::red));
            export_scene->addRect((item->xi+sensor_offset_x/pix_per_brick) * scale,
                     (item->yi+sensor_offset_y/pix_per_brick) * scale,
                     px_to_mm*item->child_length*de_spacing_um/1000,
                     px_to_mm*item->child_width*de_spacing_um/1000,
                     nopen, QBrush(Qt::red));
        }
   }



   export_scene->render(&p);
   p.end();

   //check if the second layer is needed
   if(second_layer){
       QPrinter printer2( QPrinter::HighResolution );
       printer2.setPageSize( QPrinter::A4 );
       printer2.setOrientation( QPrinter::Portrait );
       printer2.setOutputFormat( QPrinter::NativeFormat );
       path.remove(path.length()-3, 3);
       path += "2.ai";
       printer2.setOutputFileName(path); // file will be created in your build directory (where debug/release directories are)

       QPainter p2;
       p2.setPen(Qt::NoPen);
      if( !p2.begin( &printer2 ) )
      {
          qDebug() << "Error!";
          return;
      }

      second_export_scene->addRect(0, 0, (float)chip_length_mm*px_to_mm, (float)chip_width_mm*px_to_mm, graypen);
      second_export_scene->render(&p2);
      p2.end();
   }
}
//EXPORT PDF
void MainWindow::pdf_clicked()
{
    QString newPath = QFileDialog::getSaveFileName(this, "Save Pdf", path, tr("PDF files (*.pdf)"));
    if (newPath.isEmpty()) return;
    path = newPath;

    QPrinter printer( QPrinter::HighResolution );
    printer.setPageSize( QPrinter::A4 );
    printer.setOrientation( QPrinter::Portrait );
    printer.setOutputFormat( QPrinter::NativeFormat );
    printer.setOutputFileName(path); // file will be created in your build directory (where debug/release directories are)

    QPainter p;

       if( !p.begin( &printer ) )
       {
           qDebug() << "Error!";
           return;
       }
        // reset all sizes to REAL SIZE
       QGraphicsScene *export_scene = new QGraphicsScene(0, 0, 9921, 14031);

       // border in REAL SIZE
       export_scene->addRect(0, 0, chip_length_mm*px_to_mm, chip_width_mm*px_to_mm, graypen);

       //units in REAL SIZE
       for(unit *item : allunits){
            unit *export_item = new unit();
            export_item->xi = (item->xi - (brick_x_start-border_px)/pix_per_brick) * de_spacing_um* px_to_cm  / 10000 / pix_per_brick;
            export_item->yi = (item->yi - (brick_y_start-border_px)/pix_per_brick) * de_spacing_um * px_to_cm  / 10000 / pix_per_brick;
            export_item->length = item->length * de_spacing_um * px_to_cm  / 10000 / pix_per_brick;
            export_item->width = item->width *de_spacing_um * px_to_cm  / 10000 / pix_per_brick;

            export_item->color = QColor(Qt::black);
            if(item->type == "move"){
                if(item->tilt == 90){
                    for(int i = 0; i < item->de_ynum; i++){
                        if(item->de_type == 1){
                            export_scene->addRect(item->xi * de_spacing_um * px_to_cm / 10000,
                                       ((item->yi+i*(de1_length_um/de_spacing_um + 1)) - (float)(brick_y_start-border_px)/pix_per_brick )* (float)de_spacing_um* px_to_cm / 10000,
                                       px_to_cm/10*de1_width_mm,
                                       px_to_cm/10*de1_length_mm,
                                       nopen, blackbrush);
                        } else {
                            export_scene->addRect(item->xi * de_spacing_um * px_to_cm / 10000,
                                       ((item->yi+i*(de2_length_um/de_spacing_um + 1)) - (float)(brick_y_start-border_px)/pix_per_brick )* (float)de_spacing_um* px_to_cm / 10000,
                                       px_to_cm/10*de2_width_mm,
                                       px_to_cm/10*de2_length_mm,
                                       nopen, blackbrush);
                        }
                    }
                }
                else {
                    for(int i = 0; i < item->de_xnum; i++){
                        if(item->de_type == 1){
                            export_scene->addRect(((item->xi+i*(de1_length_um/de_spacing_um + 1)) - (float)(brick_x_start-border_px)/pix_per_brick )* (float)de_spacing_um* px_to_cm  / 10000,
                                       item->yi* de_spacing_um * px_to_cm  / 10000,
                                       px_to_cm/10*de1_length_mm,
                                       px_to_cm/10*de1_width_mm,
                                       nopen, blackbrush);
                        } else {
                            export_scene->addRect(((item->xi+i*(de2_length_um/de_spacing_um + 1)) - (float)(brick_x_start-border_px)/pix_per_brick )* (float)de_spacing_um* px_to_cm  / 10000,
                                       item->yi* de_spacing_um * px_to_cm  / 10000,
                                       px_to_cm/10*de2_length_mm,
                                       px_to_cm/10*de2_width_mm,
                                       nopen, blackbrush);
                        }
                    }
                }
            }else if(item->type == "cycle"){
                for(int i = 0; i < item->de_xnum; i++){
                    for(int j = 0; j < item->de_ynum; j++){
                        if(i!=0 && i!=item->de_xnum-1 && j!=0 && j!=item->de_ynum-1) continue;
                                export_scene->addRect(((item->xi+i*(de2_length_um/de_spacing_um + 1))- (brick_x_start-border_px)/pix_per_brick)* de_spacing_um* px_to_cm  / 10000,
                                          (item->yi+j*(de2_length_um/de_spacing_um + 1))* de_spacing_um * px_to_cm  / 10000,
                                          px_to_cm / 10*de2_length_mm,
                                          px_to_cm / 10*de2_length_mm,
                                          nopen, blackbrush);
                    }
                }
            }
            else{
                export_scene->addItem(export_item);
            }
       }
       linepen.setColor(Qt::black);
       linepen.setWidth(line_width_um * px_to_cm  / 10000);
       for(line *head : linescene->alllines){
           if(head->heater_line)linepen.setWidth(2 * line_width_um * px_to_cm / 10000);
           line *current_seg = head;
           while(current_seg->next != NULL){
               export_scene->addLine(current_seg->x[0]* de_spacing_um * px_to_cm  / 10000 / pix_per_brick, current_seg->y[0]* de_spacing_um * px_to_cm  / 10000 / pix_per_brick, current_seg->x[1]* de_spacing_um * px_to_cm  / 10000 / pix_per_brick, current_seg->y[1]* de_spacing_um * px_to_cm  / 10000 / pix_per_brick, linepen);
               current_seg = current_seg->next;
           }
           export_scene->addLine(current_seg->x[0]* de_spacing_um * px_to_cm  / 10000 / pix_per_brick, current_seg->y[0]* de_spacing_um * px_to_cm  / 10000 / pix_per_brick, current_seg->x[1]* de_spacing_um * px_to_cm  / 10000 / pix_per_brick, current_seg->y[1]* de_spacing_um * px_to_cm  / 10000 / pix_per_brick, linepen);
           if(head->heater_line)linepen.setWidth(line_width_um * px_to_cm / 10000);
       }

       export_scene->render(&p);
       p.end();
}

// CLOSE
void MainWindow::close_clicked()
{
    save_yn_dialog *want = new save_yn_dialog(this);
    want->setWindowTitle("Do you want to save this chip?");
    connect(want, SIGNAL(yes_save(save_yn_dialog *)), this, SLOT(close_window(save_yn_dialog *)));
    want->show();
}

void MainWindow::close_window(save_yn_dialog *s)
{
    if(s->want_to_save){
        this->save_svg();
        this->close();
    }
    else{
        this->close();
    }

}

/////////////////////////////////////  TOOL  BAR  /////////////////////////////////////
//DELETE (ERASER)
void MainWindow::on_eraser_clicked()
{
    deletemode = 1-deletemode;

    if(deletemode == true){
        QPixmap *e = new QPixmap(":/MainWindow/Icons/Icons/eraser_cursor.png");
        QCursor cursorErase = QCursor(*e, -10, -4);
        ui->eraser->setStyleSheet("background-color: rgb(64, 72, 91);");
        ui->view->setCursor(cursorErase);
    }
    else {
        if(linemode){
            ui->eraser->setStyleSheet("background-color: rgb(42, 48, 58);");
            QPixmap *e = new QPixmap(":/MainWindow/Icons/Icons/cursor.png");
            QCursor pencil = QCursor(*e, -10, -10);
            ui->view->setCursor(pencil);
        }
        else{
            ui->eraser->setStyleSheet("background-color: rgb(42, 48, 58);");
            ui->view->setCursor(Qt::ArrowCursor);
        }
    }
}

//remove the deleted item from allunits list
void MainWindow::delete_from_list(unit *item)
{
    if(item->type == "merge"){ num_merge--; num_de -= 6;}
    if(item->type == "dispenser"){ num_dispenser--; num_de -= item->de_xnum;}
    if(item->type == "move"){ num_move--; num_de -= item->de_xnum*item->de_ynum;}
    if(item->type == "cycle"){ num_cycling--; num_de -= (item->de_xnum + item->de_ynum - 2) * 2;}
    if(item->type == "heat"){ num_heater--; num_de -= 2;}
    if(item->type == "sensor"){num_sensor--; num_de -= 1;}
    Info();
    allunits.removeOne(item);
    delete item;
}

void MainWindow::on_zoomin_clicked()
{
    zoom_factor *= 1.1;
    ui->view->setTransform(QTransform().scale(zoom_factor, zoom_factor));
}

void MainWindow::on_zoomout_clicked()
{
    zoom_factor /= 1.1;
    ui->view->setTransform(QTransform().scale(zoom_factor, zoom_factor));
}

//AUTO CONNECT
void MainWindow::on_connect_btn_clicked()
{

    //int xoff = (brick_x_start-border_px)/pix_per_brick;
    //int yoff = (brick_y_start-border_px)/pix_per_brick;
    int xsize = brick_xnum;// + xoff;
    int ysize = brick_ynum;// + yoff;
    float shift = chip_border_mm*1000 / de_spacing_um;

    struct edge{
        int flow, cap, cost, dir;
        unsigned to, rev;
        edge(unsigned to, int cap, int cost, unsigned rev, int dir){
            this->to = to;
            this->cap = cap;
            this->cost = cost;
            this->rev = rev;
            this->flow = 0;
            this->dir = dir;
        }
    };

    std::vector<std::vector<edge>> graph(2*unsigned(xsize*ysize)+2);
    auto addEdge = [&graph](unsigned s, unsigned t, int cost, int cap, int dir=0){
        graph[s].push_back(edge(t, cap, cost, graph[t].size(), dir));
        graph[t].push_back(edge(s, 0, -cost, graph[s].size()-1, dir));
    };

    // unitmap setup
    std::vector<line*>newlines = {};
    memset(unitmap, 0, sizeof(unitmap));
    int unit_id = 2;
    int cpad_id = -2;
    bool source[500][500] = {{false}};
    bool heatmap[500][500] = {{false}};
    int routmap[500][500];
    memset(routmap, 0, sizeof(routmap));

    for(unit *item : allunits){
        if(item->type == "move"){
            int child_length = (item->tilt == 0) ? item->child_length : item->child_width;
            int child_width =  (item->tilt == 0) ? item->child_width : item->child_length;
            int startx, starty;
            for(int i = 0, j = 0; i < item->de_xnum && j < item->de_ynum; i+=(item->tilt==0), j+=(item->tilt==90)){
                startx = int(item->xi - shift) + i*(child_length + item->child_gap);
                starty = int(item->yi - shift) + j*(child_width  + item->child_gap);
                for(int k = 0; k < child_length; k++)
                    for(int l = 0; l < child_width; l++)
                        unitmap[startx + k][starty + l] = unit_id;
                source[startx + child_length/2][starty + child_width/2] = true;
                unitmap[startx + child_length/2][starty + child_width*1] = 1;
                unitmap[startx + child_length/2][starty + child_width*0] = 1;
                unitmap[startx + child_length*1][starty + child_width/2] = 1;
                unitmap[startx + child_length*0][starty + child_width/2] = 1;
                unit_id ++;

            }

        } else if(item->type == "cycle"){
            int unit_length = de2_length_mm*1000/de_spacing_um;
            for(int i = 0; i < item->de_xnum; i++){
                for(int j = 0; j < item->de_ynum; j++){
                    if(i!=0 && i!=item->de_xnum-1 && j!=0 && j!=item->de_ynum-1)
                        continue;
                    for(int k = 0; k <= unit_length; k++)
                        for (int l = 0; l <= unit_length; l++)
                            unitmap[int(item->xi - shift)+i*(unit_length+1) + k][int(item->yi - shift)+j*(unit_length+1) + l] = unit_id;//(k == int(unit_length/2) || l == int(unit_length/2)) ? 1 : unit_id;
                    source[int(item->xi - shift)+i*(unit_length+1) + unit_length/2][int(item->yi - shift) +j*(unit_length+1)+ unit_length/2] = true;
                    unitmap[int(item->xi - shift)+i*(unit_length+1) + unit_length/2][int(item->yi - shift) +j*(unit_length+1)+ unit_length] = 1;
                    unitmap[int(item->xi - shift)+i*(unit_length+1) + unit_length/2][int(item->yi - shift) +j*(unit_length+1)+ 0] = 1;
                    unitmap[int(item->xi - shift)+i*(unit_length+1) + unit_length][int(item->yi - shift) +j*(unit_length+1)+ unit_length/2] = 1;
                    unitmap[int(item->xi - shift)+i*(unit_length+1) + 0][int(item->yi - shift) +j*(unit_length+1)+ unit_length/2] = 1;
                    unit_id ++;
                }
            }

        } else if(item->type == "dispenser"){
            int main_length = (item->direction%2) ? item->main_length : item->main_width;
            int main_width  = (item->direction%2) ? item->main_width : item->main_length;
            int child_length = (item->direction%2) ? item->child_length : item->child_width;
            int child_width  = (item->direction%2) ? item->child_width : item->child_length;

            int main_startx = (item->direction == 3) ? item->xi - shift + item->length - main_length : item->xi - shift;
            int main_starty = (item->direction == 0) ? item->yi - shift + item->width - main_width : item->yi - shift;
            int child_startx = (item->direction == 1) ? item->xi - shift + main_length :
                               (item->direction == 3) ? item->xi - shift : item->xi - shift + (main_length-child_length)/2;
            int child_starty = (item->direction == 2) ? item->yi - shift + main_width :
                               (item->direction == 0) ? item->yi - shift : item->yi - shift + (main_width-child_width)/2;
            int tilt = (item->direction % 2 == 0) ? 90 : 0;
            qDebug() << main_length << main_width << child_length << child_width;
            qDebug() << main_startx << main_starty << child_startx << child_starty;

            // main part
            for(int i = 0; i < item->main_length; i++)
                for(int j = 0; j < item->main_width; j++)
                    unitmap[main_startx + i][main_starty + j] = unit_id;
            source[main_startx  + main_length/2][main_starty + main_width/2] = true;
            unitmap[main_startx + main_length*1][main_starty + main_width/2] = 1;
            unitmap[main_startx + main_length*0][main_starty + main_width/2] = 1;
            unitmap[main_startx + main_length/2][main_starty + main_width*1] = 1;
            unitmap[main_startx + main_length/2][main_starty + main_width*0] = 1;
            unit_id ++;

            // child part
            for(int i = 0, j = 0; i < item->de_xnum && j < item->de_xnum; i+=(tilt==0), j+=(tilt==90)){
                for(int k = 0; k < child_length; k++)
                    for(int l = 0; l < child_width; l++)
                        unitmap[child_startx + k][child_starty+ l] = unit_id;
                source[child_startx  + child_length/2][child_starty + child_width/2] = true;
                unitmap[child_startx + child_length*1][child_starty + child_width/2] = 1;
                unitmap[child_startx + child_length*0][child_starty + child_width/2] = 1;
                unitmap[child_startx + child_length/2][child_starty + child_width*1] = 1;
                unitmap[child_startx + child_length/2][child_starty + child_width*0] = 1;
                unit_id ++;
                child_startx += (tilt==0)*(child_length + item->child_gap);
                child_starty += (tilt==90)*(child_width + item->child_gap);
            }

        } /*else if(item->type == "sensor"){
            int ynum = (item->yi+1)/5;
            for(int i = 0; i < ynum; i++){

            }


        } */else if(item->type == "merge"){
            for(int i = 0; i < 3; i++){
                int start1x = int(item->xi - shift) + i*8;
                int start2x = int(item->xi + item->length - shift) - 15 - i*8;
                int endx = int(item->xi + item->length - shift) - i*8;
                int starty = int(item->yi - shift) + i*6;
                for(int j = start1x; j <= endx; j++)
                    for(int k = 0; k <= 5; k++){
                        unitmap[j][starty + k] = unit_id;
                        line* newline = new line();
                        newline->previous = nullptr;
                        newline->next = nullptr;
                        newline->x[0] = (start1x + 7 + shift)*pix_per_brick;
                        newline->x[1] = (start2x + 7 + shift)*pix_per_brick;
                        newline->y[0] = (starty + 2 + shift)*pix_per_brick;
                        newline->y[1] = (starty + 2 + shift)*pix_per_brick;
                        newlines.push_back(newline);
                    }
//                for(int j = 0; j < 15; j++)
//                    for(int k = 0; k < 5; k++){
//                        unitmap[start1x + j][starty + k] = unit_id;
//                        unitmap[start2x + j][starty + k] = unit_id;
//                    }
                source[(start1x + endx)/2][starty + 2] = true;
                unitmap[start1x][starty + 2] = 1;
                unitmap[start2x+15][starty + 2] = 1;
                unit_id ++;
            }
            for(int i = 3; i < 6; i++){
                int startx = int(item->xi - shift) + 3*8;
                int starty = int(item->yi - shift) + i*6;
                for(int j = 0; j <= 15; j++)
                    for(int k = 0; k <= 5; k++){
                        unitmap[startx + j][starty + k] = unit_id;
                    }
                source[startx + 7][starty + 2] = true;
                unitmap[startx][starty + 2] = 1;
                unitmap[startx + 15][starty + 2] = 1;
                unit_id ++;
            }


//            for(int i = 0; i <= item->length; i++)
//                for(int j = 0; j <= item->width; j++)
//                    unitmap[int(item->xi - shift) + i][int(item->yi - shift) + j] = unit_id;
//            source[int(item->xi - shift) + int(item->length/2)][int(item->yi - shift) + int(item->width/2)] = true;
//            unitmap[int(item->xi - shift) + int(item->length*1)][int(item->yi - shift) + int(item->width/2)] = 1;
//            unitmap[int(item->xi - shift) + int(item->length*0)][int(item->yi - shift) + int(item->width/2)] = 1;
//            unitmap[int(item->xi - shift) + int(item->length/2)][int(item->yi - shift) + int(item->width*1)] = 1;
//            unitmap[int(item->xi - shift) + int(item->length/2)][int(item->yi - shift) + int(item->width*0)] = 1;
//            unit_id ++;
        } else if(item->type == "heat"){
            for(int i = 0; i <= item->length; i++)
                for(int j = 0; j <= item->width; j++){
                    unitmap[int(item->xi - shift) + i][int(item->yi - shift) + j] = unit_id;//(i == int(item->length/2) || j == int(item->width/2)) ? 1 : unit_id;                    
                }
            int side_point_x = int(item->xi-shift) + (item->length*((item->tilt == 0) ? 1 : (item->tilt == 180) ? 0 : 0.5));
            int side_point_y = int(item->yi-shift) + (item->width*((item->tilt == 90) ? 1 : (item->tilt == 270) ? 0 : 0.5));
            int vertex_x = int(item->xi-shift) + item->length*((item->clockwise && (item->tilt == 0 || item->tilt == 270)) || (!item->clockwise && (item->tilt == 0 || item->tilt == 90)));
            int vertex_y = int(item->yi-shift) + item->width*((item->clockwise && (item->tilt == 0 || item->tilt == 90)) || (!item->clockwise && (item->tilt == 90 || item->tilt == 180)));
            source[side_point_x][side_point_y] = true;
            heatmap[side_point_x][side_point_y] = true;
            source[vertex_x][vertex_y] = true;
            heatmap[vertex_x][vertex_y] = true;
            unitmap[side_point_x][side_point_y] = 1;
            unitmap[vertex_x][vertex_y] = 1;
            unit_id ++;
        } else if(item->type == "controlpad"){
            for(int i = -1; i <= item->length+1; i++)
                for(int j = 0; j <= item->width; j++){
                    if(int(item->xi - shift) + i < 0 || int(item->xi - shift) + i >= xsize)continue;
                    unitmap[int(item->xi - shift) + i][int(item->yi - shift) + j] = (j > 0+1 && j < int(item->width)-1) ? ((i == -1 || i == item->length+1) ? 0 : -1) : cpad_id;
                }
            cpad_id --;

        }
    }

    qDebug() << "total de is " << num_de;
    qDebug() << xsize << ysize;

    for(int j = 0; j < ysize; j++){
        QString debug;
        for(int i = 0; i < xsize; i++){
            //debug += QString::number(unitmap[i][j]);
            debug += (unitmap[i][j] > 0 ? '+' : (unitmap[i][j] < 0) ? '-' : '0');
            // line spacing adjustment
            if(unitmap[i][j] == 0 && i%2 && j%2)unitmap[i][j] = unit_id;
        }
        qDebug() << debug;
    }

    /*float transfer = float(de_spacing_um) / line_width_um;
    for(int i = 0; i < xsize; i++){
        for(int j = 0; j < ysize; j++){
            if(abs(routmap[int(i*transfer)][int(j*transfer)]) != 1)
                routmap[int(i*transfer)][int(j*transfer)] = unitmap[i][j];
        }
    }
    memset(unitmap, 0, sizeof(unitmap));
    for(int i = 0; i < xsize*transfer; i++){
        for(int j = 0; j < ysize*transfer; j++){
            unitmap[i][j] = routmap[i][j];
        }
    }
    qDebug()<<"routmap:";
    xsize *= transfer;
    ysize *= transfer;
    for(int i = 0; i < xsize; i++){
        QString debug;
        for(int j = 0; j < ysize; j++){
            debug += QString::number(routmap[i][j]);
        }
        qDebug() << debug;
    }*/

    // set source and target
    unsigned s = unsigned(xsize*ysize*2);
    unsigned t = unsigned(xsize*ysize*2+1);

    // flow node connection
    for(int i = 0; i < xsize; i++){
        for(int j = 0; j < ysize; j++){
            unsigned from = unsigned(i*ysize+j);
            unsigned to_d = unsigned((i+1)*ysize+j); //down (actually right on chip
            unsigned to_r = unsigned(i*ysize+j+1); //right (actually down on chip
            // self connection for node capacity
            addEdge(from+unsigned(xsize*ysize), from, 0, 1);

            // neighbors connection
            if (i != xsize-1) {
                if(unitmap[i][j] == unitmap[i+1][j] /*|| (unitmap[i][j] * unitmap[i+1][j] > 0)*/){
                    if(unitmap[i][j] == 1 && unitmap[i+1][j] == 1) continue;
                    addEdge(from, to_d+unsigned(xsize*ysize), (unitmap[i][j] == 0) ? 1 : 0, 1, 1);
                    addEdge(to_d, from+unsigned(xsize*ysize), (unitmap[i][j] == 0) ? 1 : 0, 1, 1);
                } else if(unitmap[i][j] > unitmap[i+1][j]){
                    if(unitmap[i][j] == 1 || unitmap[i+1][j] == -1 || unitmap[i+1][j] == 1)
                        //addEdge(from, to_d+unsigned(xsize*ysize), 1, 1, 1);
                        addEdge(to_d, from+unsigned(xsize*ysize), 1, 1, 1);
                    else
                        ;
                } else {
                    if(unitmap[i+1][j] == 1 || unitmap[i][j] == -1 || unitmap[i][j] == 1)
                        //addEdge(to_d, from+unsigned(xsize*ysize), 1, 1, 1);
                        addEdge(from, to_d+unsigned(xsize*ysize), 1, 1, 1);
                    else;
                }
            }
            if (j != ysize-1) {
                if(unitmap[i][j] == unitmap[i][j+1] /*|| (unitmap[i][j] * unitmap[i][j+1] > 0)*/){
                    if(unitmap[i][j] == 1 && unitmap[i][j+1] == 1) continue;
                    addEdge(from, to_r+unsigned(xsize*ysize), (unitmap[i][j] == 0) ? 1 : 0, 1, 2);
                    addEdge(to_r, from+unsigned(xsize*ysize), (unitmap[i][j] == 0) ? 1 : 0, 1, 2);
                } else if(unitmap[i][j] > unitmap[i][j+1]){
                    if(unitmap[i][j] == 1 || unitmap[i][j+1] == -1 || unitmap[i][j+1] == 1)
                        //addEdge(from, to_r+unsigned(xsize*ysize), 1, 1, 2);
                        addEdge(to_r, from+unsigned(xsize*ysize), 1, 1, 2);
                    else
                        ;
                } else {
                    if(unitmap[i][j+1] == 1 || unitmap[i][j] == -1 || unitmap[i][j] == 1)
                        //addEdge(to_r, from+unsigned(xsize*ysize), 1, 1, 2);
                        addEdge(from, to_r+unsigned(xsize*ysize), 1, 1, 2);
                    else
                        ;
                }
            }

            // s/t connection
            if(unitmap[i][j] >= 1){
                //if(!(i < xsize-1 && (unitmap[i+1][j] == 1 || unitmap[i+1][j] == unitmap[i][j])) &&
                  // !(j < ysize-1 && (unitmap[i][j+1] == 1 || unitmap[i][j+1] == unitmap[i][j])))
                if(source[i][j] == true){
                    // exchange st
                    //addEdge(s, from+unsigned(xsize*ysize), 1, 1);
                    addEdge(from+unsigned(xsize*ysize), t, 1, 1);
                }
            } else if(unitmap[i][j] <= -1){
                if(!(i < xsize-1 && (unitmap[i+1][j] == -1 || unitmap[i+1][j] == unitmap[i][j])) &&
                   !(j < ysize-1 && (unitmap[i][j+1] == -1 || unitmap[i][j+1] == unitmap[i][j]))){
                    //addEdge(from, t, 1, 1);
                    addEdge(s, from, 1, 1);
                }
            }
        }
    }

    int count = 0;
    for(int i = 0; i < xsize; i++)
        for(int j = 0; j < ysize; j++)
            if(unitmap[i][j] == unit_id)unitmap[i][j] = 0;

    for(unit* unit : allunits){
        count += unit->de_xnum*unit->de_ynum;
        qDebug() << unit->xi << unit->yi;
    }

    unsigned n = graph.size();
    std::vector<int> distance(n);
    std::vector<int> currflow(n);
    std::vector<int> prevedge(n);
    std::vector<int> prevnode(n);


    // find path by bellmanFord
    auto bellmanFord = [n, s, &graph, &distance, &currflow, &prevedge,&prevnode](){
        std::fill(distance.begin(), distance.end(), INT_MAX);
        distance[s] = 0;
        currflow[s] = INT_MAX;
        std::vector<bool> inqueue(n);
        std::queue<unsigned> q;
        q.push(s);
        while(!q.empty()){
            unsigned u = q.front();
            q.pop();
            inqueue[u] = false;
            for (unsigned i = 0; i < graph[u].size(); i++) {
                edge e = graph[u][i];
                if (e.flow >= e.cap)
                    continue;
                unsigned v = e.to;
                int ndist = distance[u] + e.cost;
                if (distance[v] > ndist) {
                    distance[v] = ndist;
                    prevnode[v] = int(u);
                    prevedge[v] = int(i);
                    currflow[v] = std::min(currflow[u], e.cap - e.flow);
                    if (!inqueue[v]) {
                        inqueue[v] = true;
                        q.push(v);
                    }
                }
            }
        }
    };

    // min cost flow
    int flow = 0;
    int flowCost = 0;
    int maxflow = num_de;

    while (flow < maxflow) {
        bellmanFord();
        if (distance[unsigned(t)] == INT_MAX)
            break;
        int df = std::min(currflow[t], maxflow - flow);
        int flow_dir = 0;
        flow += df;
        for (unsigned v = t; v != s;) {
            edge* e;
            e = &graph[unsigned(prevnode[v])][unsigned(prevedge[v])];
            e->flow += df;
            if(e->dir != 0)flow_dir = e->dir;
            graph[v][e->rev].flow -= df;
            flowCost += df * e->cost;
            v = unsigned(prevnode[v]);
            //delete(e);
        }
    }

    qDebug() << flow << flowCost;

    enum path{none=0, startDown=1, startLeft=2, startUp=3, startRight=4, endDown=5, endLeft=10, endUp=15, endRight=20, DownLeft=26, DownRight=27, UpLeft=28, UpRight=29};
    int pathmap[500][500] = {{none}};

    for(unsigned i = 0; i < xsize; i++){
        for(unsigned j = 0; j < ysize; j++){
            for(edge e : graph[unsigned(i*ysize+j)]){
                if(e.flow > 0 && e.to != s && e.to != t){
                    unsigned to_x = (e.to-unsigned(xsize*ysize)) / unsigned(ysize);
                    unsigned to_y = (e.to-unsigned(xsize*ysize)) % unsigned(ysize);
                    //qDebug()<<i<<j<<to_x<<to_y;
                    if(unitmap[i][j] * unitmap[to_x][to_y] > 0)
                        continue;
                    if(to_x > i){
                        pathmap[i][j] += startDown;
                        pathmap[to_x][to_y] += endUp;
                    } else if(to_x < i){
                        pathmap[i][j] += startUp;
                        pathmap[to_x][to_y] += endDown;
                    } else if(to_y > j){
                        pathmap[i][j] += startRight;
                        pathmap[to_x][to_y] += endLeft;
                    } else if(to_y < j){
                        pathmap[i][j] += startLeft;
                        pathmap[to_x][to_y] += endRight;
                    }
                }
            }            
        }
    }

    // rearrange pathmap
    for(int i = 0; i < xsize; i++){
        for(int j = 0; j < ysize; j++){
            if(pathmap[i][j] == startUp+endLeft || pathmap[i][j] == startLeft+endUp){
                pathmap[i][j] = UpLeft;
            } else if (pathmap[i][j] == startUp+endRight || pathmap[i][j] == startRight+endUp) {
                pathmap[i][j] = UpRight;
            } else if (pathmap[i][j] == startDown+endLeft || pathmap[i][j] == startLeft+endDown){
                pathmap[i][j] = DownLeft;
            } else if (pathmap[i][j] == startDown+endRight || pathmap[i][j] == startRight+endDown){
                pathmap[i][j] = DownRight;
            } else if (pathmap[i][j] % 5 == 0 || pathmap[i][j] / 5 == 0){
                pathmap[i][j] = pathmap[i][j];
            } else {
                pathmap[i][j] = none;
            }
        }
    }

    bool turning_checked[500][500] = {{false}};
    std::function<void (int, int)> checkturning = [&pathmap, &turning_checked, xsize, ysize, &checkturning](int x, int y){
        if(turning_checked[x][y] || pathmap[x][y] == none || pathmap[x][y]/5 == 0 || pathmap[x][y]%5 == 0){
            turning_checked[x][y] = true;
            return;
        }

        int dirx = (pathmap[x][y] == DownRight || pathmap[x][y] == DownLeft) ? 1 : -1;
        int diry = (pathmap[x][y] == UpRight || pathmap[x][y] == DownRight) ? 1 : -1;
        int endx = -1, endy = -1;
        for(int i = x+dirx; i >= 0 && i < xsize; i += dirx)
            if(pathmap[i][y] != none && pathmap[i][y]%5 != 0 && pathmap[i][y]/5 != 0){
                endx = i;
                break;
            }
        for(int i = y+diry; i >= 0 && i < ysize; i += diry)
            if(pathmap[x][i] != none && pathmap[x][i]%5 != 0 && pathmap[x][i]/5 != 0){
                endy = i;
                break;
            }

        if(endx == -1 || endy == -1 || pathmap[endx][y] != pathmap[x][endy]){
            turning_checked[x][y] = true;
            return ;
        }

        for(int i = x+dirx; abs(i-x)<=abs(endx-x); i += dirx){
            for(int j = y+diry; abs(j-y)<=abs(endy-y); j += diry){
                if(pathmap[i][j] != none){
                    if(turning_checked[i][j]){
                        turning_checked[x][y] = true;
                        return;
                    }
                    checkturning(i, j);
                }
            }
        }
        for(int i = x+dirx; abs(i-x)<=abs(endx-x); i += dirx)
            for(int j = y+diry; abs(j-y)<=abs(endy-y); j += diry)
                if(pathmap[i][j] != none || unitmap[i][j] != 0){
                    turning_checked[x][y] = true;
                    return;
                }
        // eliminate turning point
        pathmap[endx][endy] = pathmap[endx][y]; // == pathmap[x][endy]
        pathmap[endx][y] = none;
        pathmap[x][endy] = none;
        pathmap[x][y] = none;
        turning_checked[endx][endy] = false;
        qDebug() << x << y << "->" << endx << endy;

        checkturning(endx, endy);
        return;
    };

    for(int i = 0; i < xsize; i++){
        for(int j = 0; j < ysize; j++){
            checkturning(i, j);
        }
    }
    qDebug() << "end check";

    /*for(int i = 0; i < xsize; i++){
        QString debug;
        for(int j = 0; j < ysize; j++){
            qreal startx = (i+shift)*pix_per_brick;
            qreal starty = (j+shift)*pix_per_brick;
            qreal endx = pix_per_brick*(shift+i+0.2);
            qreal endy = pix_per_brick*(shift+j+0.2);
            if(pathmap[i][j]%5 != 0 || pathmap[i][j]/5 != 0)
                ui->view->scene()->addRect(startx, starty, endx-startx, endy-starty, QPen(Qt::black), QBrush(Qt::black));

            debug += QString::number(pathmap[i][j]);
        }
        qDebug() << debug;
    }*/


    for(int x = 0; x < xsize; x++){
        for(int y = 0; y < ysize; y++){
            if(pathmap[x][y] != 0 && pathmap[x][y] <= 4){
                int dirx = (pathmap[x][y] == startDown) ? 1 : (pathmap[x][y] == startUp) ? -1 : 0;
                int diry = (pathmap[x][y] == startLeft) ? -1 : (pathmap[x][y] == startRight) ? 1 : 0;
                bool heater_line = heatmap[x][y];
                line* newline = new line();
                newline->heater_line = heater_line;
                newline->previous = nullptr;
                newlines.push_back(newline);
                newline->x[0] = (x+shift)*pix_per_brick;
                newline->y[0] = (y+shift)*pix_per_brick;
                for(int i = x+dirx, j = y+diry; i >= 0 && i < xsize && j >= 0 && j < ysize ; i += dirx, j += diry){
                    if(pathmap[i][j] != none){
                        newline->x[1] = (i+shift)*pix_per_brick;
                        newline->y[1] = (j+shift)*pix_per_brick;
                        if(pathmap[i][j]/5 == 0 || pathmap[i][j]%5 == 0){
                            newline->next = nullptr;
                            break;
                        } else {
                            newline->next = new line();
                            newline->next->previous = newline;
                            newline = newline->next;
                            newline->heater_line = heater_line;
                            newline->x[0] = (i+shift)*pix_per_brick;
                            newline->y[0] = (j+shift)*pix_per_brick;
                            if (pathmap[i][j] == UpRight || pathmap[i][j] == DownLeft) {
                                int temp = dirx;
                                dirx = diry;
                                diry = temp;
                            } else if (pathmap[i][j] == UpLeft || pathmap[i][j] == DownRight) {
                                int temp = dirx;
                                dirx = -diry;
                                diry = -temp;
                            }
                        }
                    }
                }
            }
        }
    }

    qDebug()<<"end build line"<<newlines.size();

    for(auto line: linescene->alllines){
        linescene->delete_from_list(line);
    }

    for(auto newline: newlines){
        linescene->AddTurnline(newline);
    }

    qDebug() << "num of lines" << linescene->alllines.size();
    linescene->update();
    ui->view->scene()->update();
}

/////////////////////////////////////  CREATE UNITS  /////////////////////////////////////
//MERGE
void MainWindow::on_merge_create_clicked()
{
    if(linemode){
        WarningMessage("Cannot add components in LINE mode!");
    }
    else if(ui->merge_length->text().isEmpty() && ui->merge_width->text().isEmpty())
    {
        WarningMessage("Missing Length and Width");
    }
    else if(ui->merge_length->text().isEmpty()){
        WarningMessage("Missing Length");
    }
    else if(ui->merge_width->text().isEmpty()){
        WarningMessage("Missing Width");
    }
    else{
        EmptyMessage();
        int number = ui->merge_num->value();
        int position = 20;
        num_merge += number;
        while(number--){
            unit *merge = new unit();
            merge->type = "merge";
            merge->xi = position;
            merge->yi = 20;
            merge->actual_length = ui->merge_length->text().toInt();
            merge->actual_width = ui->merge_width->text().toInt();
            merge->length = ui->merge_length->text().toInt()*1000/de_spacing_um;
            merge->width = ui->merge_width->text().toInt()*1000/de_spacing_um;
            merge->color = merge_color;
            ui->view->scene()->addItem(merge);
            allunits.prepend(merge);
            position += ui->merge_length->text().toInt() + 3;
            connect(merge, SIGNAL(delete_this_item(unit *)), this, SLOT(delete_from_list(unit *)));
            num_de += 6;
        }
        ui->num_merge->setText(QString::number(num_merge));

        //Create 3 mall units to set From nodes and To node
//        unit *fromA = new unit();
//        fromA->xi = 20;
//        fromA->yi = 20;
//        fromA->length = 5;
//        fromA->width = 5;
//        fromA->color = merge_color;
//        ui->view->scene()->addItem(fromA);
//        unit *fromB = new unit();
//        fromB->xi = 30;
//        fromB->yi = 20;
//        fromB->length = 5;
//        fromB->width = 5;
//        fromB->color = merge_color;
//        ui->view->scene()->addItem(fromB);
//        unit *to = new unit();
//        to->xi = 40;
//        to->yi = 20;
//        to->length = 5;
//        to->width = 5;
//        to->color = dispenser_color;
//        ui->view->scene()->addItem(to);
    }
}

//DISPENSER
void MainWindow::on_dispenser_create_clicked()
{
    if(linemode){
        WarningMessage("Cannot add components in LINE mode!");
    }
    else if(ui->dispenser_length->text().isEmpty() && ui->dispenser_width->text().isEmpty())
    {
        WarningMessage("Missing Length and Width");
    }
    else if(ui->dispenser_length->text().isEmpty()){
        WarningMessage("Missing Length");
    }
    else if(ui->dispenser_width->text().isEmpty()){
        WarningMessage("Missing Width");
    }
    else{
        EmptyMessage();
        int number = ui->dispenser_num->value();
        int position = 20;
        num_dispenser += 1;
            unit *dispenser = new unit();
            dispenser->type = "dispenser";
            dispenser->xi = position;
            dispenser->yi = 20;
            dispenser->de_type = 0;

            // main dispenser
            dispenser->actual_length = ui->dispenser_length->text().toInt();
            dispenser->actual_width = ui->dispenser_width->text().toInt();
            dispenser->main_length = ui->dispenser_length->text().toInt()*1000/de_spacing_um;
            dispenser->main_width = ui->dispenser_width->text().toInt()*1000/de_spacing_um;

            // child
            dispenser->child_length = ui->dispenser_child_length->text().toInt()*1000/de_spacing_um;
            dispenser->child_width = ui->dispenser_child_width->text().toInt()*1000/de_spacing_um;
            dispenser->child_gap = ui->dispenser_child_gap->text().toInt()/de_spacing_um;
            dispenser->direction = ui->dispenser_dir->currentIndex();


            // the entire component
            switch(dispenser->direction){
                case 0 :
                    dispenser->length = ui->dispenser_length->text().toInt()*1000/de_spacing_um;
                    dispenser->width = ui->dispenser_width->text().toInt()*1000/de_spacing_um + number*(dispenser->child_length+dispenser->child_gap);
                    dispenser->de_xnum = number;
                    dispenser->de_ynum = number;
                    dispenser_offset_up_down = (dispenser->main_length - dispenser->child_width)/2;
                break;
                case 1 :
                    dispenser->length = ui->dispenser_length->text().toInt()*1000/de_spacing_um + number*(dispenser->child_length+dispenser->child_gap);
                    dispenser->width = ui->dispenser_width->text().toInt()*1000/de_spacing_um;
                    dispenser->de_xnum = number;
                    dispenser->de_ynum = number;
                    dispenser_offset_left_right = (dispenser->main_width - dispenser->child_width)/2;
                break;
                case 2:
                    dispenser->length = ui->dispenser_length->text().toInt()*1000/de_spacing_um;
                    dispenser->width = ui->dispenser_width->text().toInt()*1000/de_spacing_um + number*(dispenser->child_length+dispenser->child_gap);
                    dispenser->de_xnum = number;
                    dispenser->de_ynum = number;
                    dispenser_offset_up_down = (dispenser->main_length - dispenser->child_width)/2;
                break;
                case 3:
                    dispenser->length = ui->dispenser_length->text().toInt()*1000/de_spacing_um + number*(dispenser->child_length+dispenser->child_gap);
                    dispenser->width = ui->dispenser_width->text().toInt()*1000/de_spacing_um;
                    dispenser->de_xnum = number;
                    dispenser->de_ynum = number;
                    dispenser_offset_left_right = (dispenser->main_width - dispenser->child_width)/2;
                break;
            }

            dispenser->color = dispenser_color;

            ui->view->scene()->addItem(dispenser);
            allunits.prepend(dispenser);
            position += ui->merge_length->text().toInt() + 3;
            connect(dispenser, SIGNAL(delete_this_item(unit *)), this, SLOT(delete_from_list(unit *)));
            num_de += 1 + number;
        ui->num_dispenser->setText(QString::number(num_dispenser));
    }
}

//SENSOR
void MainWindow::on_sensor_create_clicked(){
    int position = 20;
    num_sensor += 1;
    unit *sensor = new unit();
    sensor->type = "sensor";
    sensor->xi = position;
    sensor->yi = 20;
    sensor->actual_length = ui->sensor_body_length->text().toInt();
    sensor->actual_width = ui->sensor_body_width->text().toInt();
    sensor->length = ui->sensor_body_length->text().toInt()*1000/de_spacing_um;
    sensor->width = ui->sensor_body_width->text().toInt()*1000/de_spacing_um;
    sensor->child_length = ui->sensor_length->text().toInt()*1000/de_spacing_um;
    sensor->child_width = ui->sensor_width->text().toInt()*1000/de_spacing_um;

    sensor_offset_x = (sensor->length - sensor->child_length)*pix_per_brick/2;
    sensor_offset_y = (sensor->width - sensor->child_width)*pix_per_brick/2;
    qDebug() << sensor->length << sensor->child_length;
    sensor->color = merge_color;
    ui->view->scene()->addItem(sensor);
    allunits.prepend(sensor);
    connect(sensor, SIGNAL(delete_this_item(unit *)), this, SLOT(delete_from_list(unit *)));
    num_de += 1;
    ui->num_sensor->setText(QString::number(num_sensor));
}
//SENSE_LINE
void MainWindow::on_sense_line_clicked()
{
    sense_line = ui->sense_line->isChecked();
}

//MOVE
void MainWindow::on_move_create_clicked()
{
    if(linemode){
        WarningMessage("Cannot add components in LINE mode!");
    }
//    else if(ui->move_size->text().isEmpty()){
//        WarningMessage("Missing Size");
//    }
    else{
        EmptyMessage();
        int number = ui->move_num->value();
        int position = 20;
        num_move += number;
        unit *move = new unit();
        move->type = "move";
        move->xi = position;
        move->yi = 20;
        move->child_length = ui->move_child_length->text().toInt()*1000/de_spacing_um;
        move->child_width = ui->move_child_width->text().toInt()*1000/de_spacing_um;
        move->child_gap = ui->move_gap->text().toInt()/de_spacing_um;
        move->tilt = ui->move_tilt->text().toInt();
        move->de_type = ui->move_electrod->value();
        if(move->tilt == 90){
            move->de_xnum = 1;
            move->de_ynum = number;
            move->length = move->de_xnum*move->child_width;
            move->width = move->de_ynum*move->child_length + move->child_gap*(move->de_ynum-1);
        }
        else{
            move->de_xnum = number;
            move->de_ynum = 1;
            move->length = move->de_xnum*move->child_length + move->child_gap*(move->de_xnum-1);
            move->width = move->de_ynum*move->child_width;
        }
        move->color = moving_color;
        ui->view->scene()->addItem(move);
        allunits.prepend(move);
        connect(move, SIGNAL(delete_this_item(unit *)), this, SLOT(delete_from_list(unit *)));
        num_de += move->de_xnum * move->de_ynum;
        ui->num_move->setText(QString::number(num_move));
    }
}

//CYCLING
void MainWindow::on_cycling_create_clicked()
{
    if(linemode){
        WarningMessage("Cannot add components in LINE mode!");
    }
    else if(ui->cycling_length->text().isEmpty() && ui->cycling_width->text().isEmpty())
    {
        WarningMessage("Missing Length and Width");
    }
    else if(ui->cycling_length->text().isEmpty()){
        WarningMessage("Missing Legth");
    }
    else if(ui->cycling_width->text().isEmpty()){
        WarningMessage("Missing Width");
    }
    else{
        EmptyMessage();
        int number = ui->cycling_num->value();
        int position = 20;
        num_cycling += number;
        while(number--){
            unit *cycle = new unit();
            cycle->type = "cycle";
            cycle->xi = position;
            cycle->yi = 20;
            cycle->de_type = 2;
            cycle->de_xnum = ui->cycling_length->text().toInt();
            cycle->de_ynum = ui->cycling_width->text().toInt();
            if(cycle->type == 1){
                cycle->length = cycle->de_xnum*de1_length_um/de_spacing_um + cycle->de_xnum-1;
                cycle->width = cycle->de_ynum*de1_length_um/de_spacing_um + cycle->de_ynum-1;
            }
            else{
                cycle->length = cycle->de_xnum*de2_length_um/de_spacing_um + cycle->de_xnum-1;
                cycle->width = cycle->de_ynum*de2_length_um/de_spacing_um + cycle->de_ynum-1;
            }
            cycle->color = cycling_color;
            ui->view->scene()->addItem(cycle);
            allunits.prepend(cycle);
            position += ui->merge_length->text().toInt() + 3;
            connect(cycle, SIGNAL(delete_this_item(unit *)), this, SLOT(delete_from_list(unit *)));
            num_de += (cycle->de_xnum + cycle->de_ynum - 2) * 2;
        }
        ui->num_cycling->setText(QString::number(num_cycling));
    }
}

//HEATER
void MainWindow::on_heater_create_clicked()
{
    if(linemode){
        WarningMessage("Cannot add components in LINE mode!");
    }
    else if(ui->heater_linewidth->text().isEmpty())
    {
        WarningMessage("Missing Number");
    }
    else{
        EmptyMessage();
        int number = 1;//ui->heater_num->text().toInt();
        int position = 20;
        num_heater += number;
        while(number--){
            unit *heat = new unit();
            heat->type = "heat";
            heat->xi = position;
            heat->yi = 20;
            heat->tilt = ui->heater_tilt->text().toInt();
//            heat->clockwise = ui->heater_clockwise->text().toInt();
            heat->clockwise = ui->clockwise->isChecked();
            heat->de_xnum = ui->heater_width->text().toInt();
            heat->zigzag_linewidth = ui->heater_linewidth->text().toInt();
            heat->zigzag_layer = ui->heater_layer->text().toInt();
            heat->length = 1+((4*heat->zigzag_layer-1)*heat->zigzag_linewidth-1)/de_spacing_um;
            heat->width = 1000*ui->heater_width->text().toInt()/de_spacing_um;
            if(heat->tilt == 90 || heat->tilt == 270)std::swap(heat->length, heat->width);
//            heat->de_xnum = 1;
//            heat->de_ynum = 1;
            heat->color = heat_color;        
            ui->view->scene()->addItem(heat);
            allunits.prepend(heat);
            position += 3;
            connect(heat, SIGNAL(delete_this_item(unit *)), this, SLOT(delete_from_list(unit *)));
            num_de += 2;
        }
        ui->num_heater->setText(QString::number(num_heater));
    }
}

//HEAT_LINE
void MainWindow::on_heat_line_clicked()
{
    heat_line = ui->heat_line->isChecked();
}

/////////////////////////////////////  SETTING  /////////////////////////////////////
void MainWindow::on_setting_btn_clicked()
{
    chip_setting *chip = new chip_setting(this, chip_length_mm, chip_width_mm, chip_border_mm,
                                          cp_length_mm, cp_width_mm,
                                          de1_length_mm, de1_width_mm, de2_length_mm, de2_width_mm,
                                          de_spacing_mm, cp_spacing_mm, line_width_mm);

    chip->setWindowTitle("Setting");
    connect(chip, SIGNAL(reset(chip_setting *)), this, SLOT(reset_setting(chip_setting *)));
    chip->show();
}

void MainWindow::clear_number_of_units(){
    //Number of units
    num_merge = 0;
    num_dispenser = 0;
    num_move = 0;
    num_cycling = 0;
    num_heater = 0;
    num_sensor = 0;
    num_de = 0;
}

void MainWindow::reset_setting(chip_setting *new_chip)
{
    clear_number_of_units();
    chip_length_mm = new_chip->chip_length_mm;
    chip_width_mm = new_chip->chip_width_mm;
    chip_border_mm = new_chip->chip_border_mm;

    cp_length_mm = new_chip->cp_length_mm;
    cp_width_mm = new_chip->cp_width_mm;

    de1_length_mm = new_chip->de1_length_mm;
    de1_width_mm = new_chip->de1_width_mm;
    de2_length_mm = new_chip->de2_length_mm;
    de2_width_mm = new_chip->de2_width_mm;

    de_spacing_mm = new_chip->de_spacing_mm;
    cp_spacing_mm = new_chip->cp_spacing_mm;
    line_width_mm = new_chip->line_width_mm;

    allunits.clear();
    QGraphicsScene *newscene = CreateNewScene();
    graphicsscene *newlinescene = CreateLineScene();

//    allunits.clear();
    this->mainscene = newscene;
    this->linescene = newlinescene;

    if(linemode)
        ui->view->setScene(newlinescene);
    else
        ui->view->setScene(newscene);

}

/////////////////////////////////////  PREVIEW  /////////////////////////////////////
void MainWindow::on_preview_clicked(bool checked)
{
    if(checked){
        checker *check = new checker(allunits, tempunits, passunits, errorunits);
        checker_map = check->checker_map;

        previewscene = new QGraphicsScene(0, 0, 760, 520, ui->view);
        previewscene->setBackgroundBrush(Qt::white);
        nullitem = QBrush(Qt::black);

        OuterBorder(previewscene);
        ChipBorder(previewscene);
        DefaultControlPad(previewscene);
        EnableCreateUnit(false);
        for(unit *item : allunits){
            nullitem = (item->error) ? QBrush(Qt::red) : QBrush(Qt::black);
            if(item->type == "move"){                   //Show detail components for "move"
                for(int i = 0; i < item->de_xnum; i++){
                    if(item->tilt == 90){
                        for(int i = 0; i < item->de_ynum; i++){
                            previewscene->addRect(item->xi*pix_per_brick,
                                       (item->yi+i*(item->child_length + 1))*pix_per_brick,
                                       pix_per_brick*item->child_width,
                                       pix_per_brick*item->child_length,
                                       nopen, nullitem);
                        }
                    }
                    else{
                        for(int i = 0; i < item->de_xnum; i++){
                            previewscene->addRect((item->xi+i*(item->child_length + 1))*pix_per_brick,
                                       item->yi*pix_per_brick,
                                       pix_per_brick*item->child_length,
                                       pix_per_brick*item->child_width,
                                       nopen, nullitem);
                        }
                    }
                }
            }
            else if(item->type == "dispenser"){
                switch(item->direction){
                    case 0 :    //up
                        for(int i = 0; i < item->de_xnum; i++){
                            previewscene->addRect((item->xi+dispenser_offset_up_down)*pix_per_brick,
                                (item->yi+(i*(item->child_length+item->child_gap)))*pix_per_brick,
                                pix_per_brick*item->child_width,
                                pix_per_brick*item->child_length,
                                nopen, nullitem);
                        }
                        previewscene->addRect(item->xi*pix_per_brick,
                                (item->yi+(item->de_xnum*(item->child_length+item->child_gap)))*pix_per_brick,
                                item->main_length*pix_per_brick,
                                item->main_width*pix_per_brick,
                                nopen, nullitem);
                        break;
                    case 1:     //right
                        previewscene->addRect(item->xi*pix_per_brick,
                                item->yi*pix_per_brick,
                                item->main_length*pix_per_brick,
                                item->main_width*pix_per_brick,
                                nopen, nullitem);
                        for(int i = 0; i < item->de_xnum; i++){
                            previewscene->addRect((item->xi+item->main_length+(i*(item->child_length+item->child_gap))+item->child_gap)*pix_per_brick,
                                (item->yi+dispenser_offset_left_right)*pix_per_brick,
                                pix_per_brick*item->child_length,
                                pix_per_brick*item->child_width,
                                nopen, nullitem);
                        }
                        break;
                    case 2:     //down
                        previewscene->addRect(item->xi*pix_per_brick,
                            item->yi*pix_per_brick,
                            item->main_length*pix_per_brick,
                            item->main_width*pix_per_brick,
                            redpen, nullitem);
                        for(int i = 0; i < item->de_xnum; i++){
                            previewscene->addRect((item->xi+dispenser_offset_up_down)*pix_per_brick,
                                (item->yi+item->main_width+(i*(item->child_length+item->child_gap))+item->child_gap)*pix_per_brick,
                                pix_per_brick*item->child_width,
                                pix_per_brick*item->child_length,
                                nopen, nullitem);
                        }
                        break;
                    case 3:     //left
                        for(int i = 0; i < item->de_xnum; i++){
                            previewscene->addRect((item->xi+(i*(item->child_length+item->child_gap)))*pix_per_brick,
                                (item->yi+dispenser_offset_left_right)*pix_per_brick,
                                pix_per_brick*item->child_length,
                                pix_per_brick*item->child_width,
                                nopen, nullitem);
                        }
                        previewscene->addRect((item->xi+(item->de_xnum*(item->child_length+item->child_gap)))*pix_per_brick,
                                item->yi*pix_per_brick,
                                item->main_length*pix_per_brick,
                                item->main_width*pix_per_brick,
                                nopen, nullitem);
                        break;
                }
            }
            else if(item->type == "cycle"){             //Show detail components for 'cycling"
                for(int i = 0; i < item->de_xnum; i++){
                    for(int j = 0; j < item->de_ynum; j++){
                        if(i!=0 && i!=item->de_xnum-1 && j!=0 && j!=item->de_ynum-1) continue;
                            previewscene->addRect((item->xi+i*(de2_length_um/de_spacing_um + 1))*pix_per_brick,
                                      (item->yi+j*(de2_length_um/de_spacing_um + 1))*pix_per_brick,
                                      pix_per_brick*de2_length_um/de_spacing_um,
                                      pix_per_brick*de2_width_um/de_spacing_um,
                                      nopen, nullitem);
                    }
                }
            }
            else if(item->type == "heat"){
                int layer = item->zigzag_layer;
                double zigzag_width_um = item->zigzag_linewidth;
                double spacing_um = 200;//int((item->length*de_spacing_um-2*zigzag_width_um*layer)/(2*layer-1));
                double connect_line_um = 353;
                //draw parallel lines
                for(int i = 0; i < 2*layer; i++){
                    if(item->tilt == 0 || item->tilt == 180){
                        if (item->tilt == 0 && i == 2*layer-1){
                            previewscene->addRect((item->xi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick, (item->yi+(!item->clockwise)*item->width/2)*pix_per_brick, zigzag_width_um/de_spacing_um*pix_per_brick, item->width/2*pix_per_brick, nopen, nullitem);
                        } else if (item->tilt == 180 && i == 0){
                            previewscene->addRect((item->xi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick, (item->yi+(item->clockwise)*item->width/2)*pix_per_brick, zigzag_width_um/de_spacing_um*pix_per_brick, item->width/2*pix_per_brick, nopen, nullitem);
                        } else if (i != 0 && i != 2*layer-1){
                            int connect_line_space = ((item->tilt == 0 && item->clockwise == 0) || (item->tilt == 180 && item->clockwise == 1)) ? (2*connect_line_um+zigzag_width_um)/de_spacing_um*pix_per_brick : 0;
                            previewscene->addRect((item->xi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick, item->yi*pix_per_brick+connect_line_space, zigzag_width_um/de_spacing_um*pix_per_brick, (item->width-(2*connect_line_um+zigzag_width_um)/de_spacing_um)*pix_per_brick, nopen, nullitem);
                        } else
                            previewscene->addRect((item->xi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick, item->yi*pix_per_brick, zigzag_width_um/de_spacing_um*pix_per_brick, item->width*pix_per_brick, nopen, nullitem);
                    } else if (item->tilt == 90 || item->tilt == 270){
                        if (item->tilt == 90 && i == 2*layer-1){
                            previewscene->addRect((item->xi+(item->clockwise)*item->length/2)*pix_per_brick, (item->yi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick, item->length/2*pix_per_brick, zigzag_width_um/de_spacing_um*pix_per_brick, nopen, nullitem);
                        } else if (item->tilt == 270 && i == 0){
                            previewscene->addRect((item->xi+(!item->clockwise)*item->length/2)*pix_per_brick, (item->yi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick, item->length/2*pix_per_brick, zigzag_width_um/de_spacing_um*pix_per_brick, nopen, nullitem);
                        } else if (i != 0 && i != 2*layer-1){
                            int connect_line_space = ((item->tilt == 90 && item->clockwise == 1) || (item->tilt == 270 && item->clockwise == 0)) ? (2*connect_line_um+zigzag_width_um)/de_spacing_um*pix_per_brick : 0;
                            previewscene->addRect(item->xi*pix_per_brick+connect_line_space, (item->yi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick, (item->length-(2*connect_line_um+zigzag_width_um)/de_spacing_um)*pix_per_brick, zigzag_width_um/de_spacing_um*pix_per_brick, nopen, nullitem);
                        } else
                            previewscene->addRect(item->xi*pix_per_brick, (item->yi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick, item->length*pix_per_brick, zigzag_width_um/de_spacing_um*pix_per_brick, nopen, nullitem);
                    }
                }
                for(int i = 0; i < 2*layer-1; i++){
                    if(item->tilt == 0 || item->tilt == 180){
                        bool t = (item->tilt == 0) ? item->clockwise == i%2 : !item->clockwise == i%2;
                        int connect_line_space = ((item->tilt == 0 && item->clockwise == 0) || (item->tilt == 180 && item->clockwise == 1)) ? (2*connect_line_um+zigzag_width_um)/de_spacing_um*pix_per_brick : 0;
                        previewscene->addRect((item->xi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick,
                                          item->yi*pix_per_brick+t*(item->width-2*(connect_line_um+zigzag_width_um)/de_spacing_um)*pix_per_brick + connect_line_space,
                                          (zigzag_width_um+spacing_um)/de_spacing_um*pix_per_brick,
                                          zigzag_width_um/de_spacing_um*pix_per_brick,
                                          nopen, nullitem);
                    } else if (item->tilt == 90 || item->tilt == 270){
                        bool t = (item->tilt == 90) ? !item->clockwise == i%2 : item->clockwise == i%2;
                        int connect_line_space = ((item->tilt == 90 && item->clockwise == 1) || (item->tilt == 270 && item->clockwise == 0)) ? (2*connect_line_um+zigzag_width_um)/de_spacing_um*pix_per_brick : 0;
                        previewscene->addRect(item->xi*pix_per_brick+t*(item->length-2*(connect_line_um+zigzag_width_um)/de_spacing_um)*pix_per_brick + connect_line_space,
                                          (item->yi+i*(zigzag_width_um+spacing_um)/de_spacing_um)*pix_per_brick,
                                          zigzag_width_um/de_spacing_um*pix_per_brick,
                                          (zigzag_width_um+spacing_um)/de_spacing_um*pix_per_brick,
                                          nopen, nullitem);
                    }
                }
                if ((item->tilt == 0 && item->clockwise == 0) || (item->tilt == 180 && item->clockwise == 1))
                    previewscene->addRect(item->xi*pix_per_brick, item->yi*pix_per_brick, item->length*pix_per_brick, connect_line_um*2/de_spacing_um*pix_per_brick, nopen, nullitem);
                else if ((item->tilt == 0 && item->clockwise == 1) || (item->tilt == 180 && item->clockwise == 0))
                    previewscene->addRect(item->xi*pix_per_brick, (item->yi+item->width-2*connect_line_um/de_spacing_um)*pix_per_brick, item->length*pix_per_brick, connect_line_um*2/de_spacing_um*pix_per_brick, nopen, nullitem);
                else if ((item->tilt == 90 && item->clockwise == 1) || (item->tilt == 270 && item->clockwise == 0))
                    previewscene->addRect(item->xi*pix_per_brick, item->yi*pix_per_brick, connect_line_um*2/de_spacing_um*pix_per_brick, item->width*pix_per_brick, nopen, nullitem);
                else if ((item->tilt == 90 && item->clockwise == 0) || (item->tilt == 270 && item->clockwise == 1))
                    previewscene->addRect((item->xi+item->length-2*connect_line_um/de_spacing_um)*pix_per_brick, item->yi*pix_per_brick, connect_line_um*2/de_spacing_um*pix_per_brick, item->width*pix_per_brick, nopen, nullitem);
            }
            else if(item->type == "sensor"){
                previewscene->addRect(item->xi*pix_per_brick, item->yi*pix_per_brick, item->length*pix_per_brick, item->width*pix_per_brick, nopen, nullitem);
                previewscene->addRect(item->xi*pix_per_brick+sensor_offset_x, item->yi*pix_per_brick+sensor_offset_y, item->child_length*pix_per_brick, item->child_width*pix_per_brick, nopen, blackbrush);
            }
            else
                previewscene->addRect(item->xi*pix_per_brick, item->yi*pix_per_brick, item->length*pix_per_brick, item->width*pix_per_brick, nopen, nullitem);
        }

        linepen = QPen(Qt::black);
        linepen.setWidth(line_width_pix);
        for(line *head : linescene->alllines){
            if(head->heater_line)linepen.setWidth(line_width_pix*2);
            line *current_seg = head;
            while(current_seg->next != NULL){
                previewscene->addLine(current_seg->x[0], current_seg->y[0], current_seg->x[1], current_seg->y[1], linepen);
                current_seg = current_seg->next;
            }
            previewscene->addLine(current_seg->x[0], current_seg->y[0], current_seg->x[1], current_seg->y[1], linepen);
            if(head->heater_line)linepen.setWidth(line_width_pix);
        }

        QGraphicsTextItem *new_text_item = previewscene->addText(text_edit);
        new_text_item->setPos(brick_x_start, 400);

        ui->view->setScene(previewscene);
    }
    else{
        EnableCreateUnit(true);
        linepen = QPen(Qt::gray, 0);
        linepen.setWidth(line_width_pix);
        nullitem = QBrush(QColor(94, 94, 94, 54));
        if(!linemode){
            ui->view->setScene(mainscene);
        }
        else{
            ui->view->setScene(linescene);
        }
    }
}

//unit m_selectedItems = m_source_scene->selectedItems();
//// The following slot is called when some action(paste) is triggered.
//void MainWindow::OnPaste()
//{
////m_selectedItems contains a list of selected graphic items
//    foreach(item,m_selectedItems)
//      m_target_scene->add(item);
//}
/////////////////////////////////////  TEXT EDIT  /////////////////////////////////////
void MainWindow::on_text_enter_clicked()
{
    if(text_edited){
        delete text_item;
    }

    if(!ui->chip_name->text().isEmpty()) text_edit = ui->chip_name->text() + "\n";
    if(!ui->creator_name->text().isEmpty()) text_edit += ui->creator_name->text() + "\n";

    text_edit += ui->date_time->date().toString() + " ";            //顯示日期
    text_edit += ui->date_time->time().toString("hh:mm:ss") + "\n";  //顯示時間

    if(!ui->text_input->toPlainText().isEmpty()) text_edit += ui->text_input->toPlainText();
    QGraphicsTextItem *new_text_item = mainscene->addText(text_edit);
    new_text_item->setPos(brick_x_start, 400);
    text_item = new_text_item;
    text_edited = true;

}

void MainWindow::on_text_delete_clicked()
{
    if(text_edited){
        delete text_item;
    }
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    QDateTime *myDateTime = new QDateTime(QDateTime::currentDateTime());
    ui->date_time->setDateTime(QDateTime::currentDateTime());
}
