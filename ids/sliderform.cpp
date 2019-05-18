#include "sliderform.h"
#include "ui_sliderform.h"

SliderForm::SliderForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SliderForm)
{
    ui->setupUi(this);
}

SliderForm::~SliderForm()
{
    delete ui;
}

void SliderForm::setRange(double min, double max)
{
    ui->SB->setRange(min, max);
    //ui->Sl->setRange(min,max);
    ui->min->display(min);
    ui->max->display(max);
}

void SliderForm::setValue(double value)
{
    ui->SB->setValue(value);
    double min=ui->SB->minimum();
    double max=ui->SB->maximum();

    int intvalue = (value - min) / (max - min) * 99;
    ui->Sl->setValue(intvalue);
}
