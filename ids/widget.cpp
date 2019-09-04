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
    connect(ui->startLifePB, SIGNAL(clicked()), this, SLOT(startLife()));
    connect(life, SIGNAL(lifeStartOk()), this, SLOT(resetScale()));
    connect(life, SIGNAL(lifeStartOk()), this, SLOT(setSliceLevel()));
    connect(life, SIGNAL(lifeStartOk()), this, SLOT(resetColor()));
    connect(life, SIGNAL(updateFrame()),this, SLOT(updateFrame()));
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
    DBF("void Widget::plotFrame()");
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

void Widget::startLife()
{
    life->startLife();
    setSetting();

}

void Widget::average()
{
    life->startStopAverage(ui->average->checkState(), ui->nAverage->value());
}

void Widget::setSliceLevel()
{
    DBF("void Widget::setSliceLevel()");
    if(life->statusCam()){
        double level = ui->level->value();
        life->setSliceLevel(level);
    }
}

void Widget::setSetting()
{
    DBF("void Widget::setSetting()");
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
    DBF("void Widget::getSetting()");
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

void Widget::CamDisconnect()
{
    ui->model->setText("Error");
    ui->serial->setText("Error");
}




void Widget::getMax()
{
    DBF("void Widget::getMax()");
    int x, y, z, bits;
    life->getMax(x,y,z);
    bits = life->getBits();
//    qDebug() << "max" << z <<  pow(2,bits) << bits;
    ui->MaxLcdNumber->display(double(z) / pow(2,bits) * 100);
}

void Widget::diametr()
{
    DBF("void Widget::diametr()");
    int method = ui->tabDiameter->currentIndex();
    life->setMerhodDiameter(method);
    switch (method) {
    case 0:{
        double dx = 0, dy = 0;
        life->getDiameter_mm(dx,dy);
        ui->dx->display(dx);
        ui->dy->display(dy);
        break;
    }
    case 1:{
        double d = 0, dx = 0, dy = 0, dBig = 0, dSmall = 0, phi = 0;
        life->getDiameterSigma_mm(d,dx,dy,dBig,dSmall,phi);
        ui->dx_sigma->display(dx);
        ui->dy_sigma->display(dy);
        ui->dBig->display(dBig);
        ui->dSmall->display(dSmall);
        ui->phi_sigma->display(phi);
        break;
    }
    default:
        break;
    }
    

}




void Widget::initCamera()
{
    DBF("void Widget::initCamera()");
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


