#include "widget.h"
#include "ui_widget.h"

void Widget::createSections()
{
    ui->sectionX->addGraph();
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
    int levelX, levelY;
    //ui->sectionX->clearGraphs();
    ui->sectionX->graph(0)->setData(*life->pAxisX, life->averageSections.x);
    QVector<double> xx, yx;

    ui->sectionX->yAxis->rescale();
    ui->sectionX->replot();

    ui->sectionY->graph(0)->setData(*life->pAxisY, life->averageSections.y);
    QVector<double> xy, yy;

    if(life->getLevel(levelX, levelY)){
        ui->sectionX->graph(1)->setData(xx,yx);
        ui->sectionX->graph(1)->addData(0,levelX);
        ui->sectionX->graph(1)->addData(life->getWidth_mm(), levelX);
        ui->sectionY->graph(1)->setData(xy,yy);
        ui->sectionY->graph(1)->addData(0,levelY);
        ui->sectionY->graph(1)->addData(life->getHeight_mm(), levelY);
    }


    ui->sectionY->yAxis->rescale();
    ui->sectionY->replot();
}

void Widget::rescaleSections()
{
    ui->sectionX->xAxis->setRange(0, life->getWidth_mm());
    ui->sectionY->xAxis->setRange(0, life->getWidth_mm());
}
