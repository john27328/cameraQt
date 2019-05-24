#include "widget.h"
#include "ui_widget.h"

void Widget::createSections()
{
    ui->sectionX->addGraph();
    // give the axes some labels:
    ui->sectionX->xAxis->setLabel("x, мм");
    ui->sectionX->yAxis->setLabel("сечение x");
    // set axes ranges, so we see all data:
    ui->sectionX->xAxis->setRange(0, life->getWidth_mm());
    ui->sectionX->yAxis->rescale();
    ui->sectionX->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
    ui->sectionX->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    ui->sectionX->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    ui->sectionX->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
    ui->sectionX->replot();

    ui->sectionY->addGraph();
    // give the axes some labels:
    ui->sectionY->xAxis->setLabel("y, мм");
    ui->sectionY->yAxis->setLabel("сечение y");
    // set axes ranges, so we see all data:
    ui->sectionY->xAxis->setRange(0, life->getWidth_mm());
    ui->sectionY->yAxis->rescale();
    ui->sectionY->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
    ui->sectionY->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    ui->sectionY->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    ui->sectionY->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
    ui->sectionY->replot();
}

void Widget::plotSections()
{
    ui->sectionX->graph(0)->setData(*life->pAxisX, life->averageSections.x);
    ui->sectionX->yAxis->rescale();
    ui->sectionX->replot();

    ui->sectionY->graph(0)->setData(*life->pAxisY, life->averageSections.y);
    ui->sectionY->yAxis->rescale();
    ui->sectionY->replot();
}

void Widget::rescaleSections()
{
    ui->sectionX->xAxis->setRange(0, life->getWidth_mm());
    ui->sectionY->xAxis->setRange(0, life->getWidth_mm());
}
