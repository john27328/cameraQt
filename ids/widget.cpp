#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //cam = new IdsCam;
    life = new Life();
    createColorMap();
    connect(ui->initPB, SIGNAL(clicked()), this, SLOT(initCamera()));
    connect(ui->startLifePB, SIGNAL(clicked()), life, SLOT(startLife()));
    connect(ui->startLifePB, SIGNAL(clicked()), this, SLOT(resetScale()));
    connect(ui->startLifePB, SIGNAL(clicked()), this, SLOT(setSliceLevel()));
    connect(ui->startLifePB, SIGNAL(clicked()), this, SLOT(resetColor()));
    connect(ui->stopLifePB, SIGNAL(clicked()), life, SLOT(stopLife()));
    connect(life, SIGNAL(updateFrame()),this, SLOT(updateFrame()));
//    connect(life, SIGNAL(updateFrame()),this, SLOT(plotSections()));
    connect(ui->resetScalePushButton, SIGNAL(clicked()), this, SLOT(resetScale()));
    connect(ui->resetColorPushButton, SIGNAL(clicked()), this, SLOT(resetColor()));
    connect(ui->BCGNDcheckBox,SIGNAL(stateChanged(int)), this, SLOT(background(int)));
    connect(ui->BCGNDpushButton, SIGNAL(clicked()), this, SLOT(saveBackground()));
    connect(life, SIGNAL(stateSaveBCGR(int)), ui->BCGNDprogressBar, SLOT(setValue(int)));
    connect(life, SIGNAL(lifeStartOk()),this, SLOT(createSections()));
    connect(ui->resetAxis, SIGNAL(clicked()), this, SLOT(rescaleSections()));
    timer1ms = new QTimer;
    timer1ms->start(1);
    plotFrame();
    connect(timer1ms,SIGNAL(timeout()),this, SLOT(plotFrame()));
    connect(ui->level,SIGNAL(valueChanged(double)),this,SLOT(setSliceLevel()));
}

Widget::~Widget()
{
    delete life;
    delete ui;
}

void Widget::updateFrame()
{
    updateFr = 1;
}

void Widget::plotFrame()
{
    if(updateFr && !plot && life->statusLife()){
//        qDebug() << "plotFrame";
        updateFr = 0;
        plot = 1;
        getMax();
        plotColorMap();
        plotSections();
        ui->averageState->setValue(life->getAverageState());
        diametr();
        plot = 0;

    }
}

void Widget::average()
{
    life->startStopAverage(ui->average->checkState(), ui->nAverage->value());
}

void Widget::setSliceLevel()
{

    if(life->statusCam()){
        double level = ui->level->value();
        life->setSliceLevel(level);
    }
}

void Widget::setSetting()
{
    disconnect(ui->expSB,SIGNAL(valueChanged(double)),this,SLOT(setSetting()));
    disconnect(ui->fpsSB,SIGNAL(valueChanged(double)),this,SLOT(setSetting()));
    double minFps;
    double maxFps;
    double fps = ui->fpsSB->value();
    double minExp;
    double maxExp;
    double exp = ui->expSB->value();
    life->setSetting(minFps, maxFps, fps, minExp, maxExp, exp);
    ui->fpsSB->setRange(minFps,maxFps);
    ui->fpsSB->setValue(fps);
    ui->minFps->display(minFps);
    ui->maxFps->display(maxFps);
    ui->expSB->setRange(minExp,maxExp);
    ui->expSB->setValue(exp);
    ui->minExp->display(minExp);
    ui->maxExp->display(maxExp);
    connect(ui->expSB,SIGNAL(valueChanged(double)),this,SLOT(setSetting()));
    connect(ui->fpsSB,SIGNAL(valueChanged(double)),this,SLOT(setSetting()));
}

void Widget::getSetting()
{
    disconnect(ui->expSB,SIGNAL(valueChanged(double)),this,SLOT(setSetting()));
    disconnect(ui->fpsSB,SIGNAL(valueChanged(double)),this,SLOT(setSetting()));
    double minFps;
    double maxFps;
    double fps;
    double minExp;
    double maxExp;
    double exp;
    life->getSetting(minFps, maxFps, fps, minExp, maxExp, exp);
    ui->fpsSB->setRange(minFps,maxFps);
    ui->fpsSB->setValue(fps);
    ui->minFps->display(minFps);
    ui->maxFps->display(maxFps);
    ui->expSB->setRange(minExp,maxExp);
    ui->expSB->setValue(exp);
    ui->minExp->display(minExp);
    ui->maxExp->display(maxExp);
    connect(ui->expSB,SIGNAL(valueChanged(double)),this,SLOT(setSetting()));
    connect(ui->fpsSB,SIGNAL(valueChanged(double)),this,SLOT(setSetting()));
}



void Widget::getMax()
{
    int x, y, z, bits;
    life->getMax(x,y,z);
    bits = life->getBits();
    qDebug() << "max" << z <<  pow(2,bits) << bits;
    ui->MaxLcdNumber->display(double(z) / pow(2,bits));
}

void Widget::diametr()
{
    int x1, x2, y1, y2;
    double dx, dy;
    if(life->getDiametr(x1,x2,y1,y2)){
        dx = life->pixelTo_mm(x2 - x1);
        dy = life->pixelTo_mm(y2 - y1);
    }
    else{
        dx = NAN;
        dy = NAN;
    }
    ui->dx->display(dx);
    ui->dy->display(dy);
}



void Widget::initCamera()
{
    QString model, serial;
    life->initCamera(!ui->testCheckBox->isChecked(), model, serial);
    ui->model->setText(model);
    ui->serial->setText(serial);
    getSetting();
}


void Widget::background(int state)
{
    qDebug() << "clicked вычитать фон" << state;
    life->setSubtractBackground(state);
}

void Widget::saveBackground()
{
    qDebug() << "clicked соранить фон" << ui->BCGND_SB->value();
    life->saveBackground(ui->BCGND_SB->value());
}


