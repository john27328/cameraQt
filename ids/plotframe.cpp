#include "widget.h"
#include "ui_widget.h"


void Widget::RescaleCustomPlot(QCustomPlot *qPlot)
{
    DBF("void Widget::RescaleCustomPlot(QCustomPlot *qPlot)");
    // определяем ширину и высоту области для отрисовки
        QSize r=qPlot->axisRect()->size();
        // если ширина больше высоты - меняем ось X
        if(r.width() > r.height())
        {
            qPlot->xAxis->setScaleRatio(qPlot->yAxis, 1.);
        }
        // если ширина меньше высоты - меняем ось X
        else if(r.width() < r.height())
        {
            qPlot->yAxis->setScaleRatio(qPlot->xAxis, 1.);
        }
}



void Widget::createColorMap()
{
    DBF("void Widget::createColorMap()");
    // configure axis rect:
    auto customPlot = ui->colorMap;
    customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
    customPlot->axisRect()->setupFullAxesBox(true);
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");

    // set up the QCPColorMap:
    colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
    int nx = 2000;
    int ny = 2000;
    colorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
    colorMap->data()->setRange(QCPRange(-4, 4), QCPRange(-4, 4)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions
    // now we assign some data, by accessing the QCPColorMapData instance of the color map:
    double x, y, z;
    for (int xIndex=0; xIndex<nx; ++xIndex)
    {
        for (int yIndex=0; yIndex<ny; ++yIndex)
        {
            colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);
            double r = 3*qSqrt(x*x+y*y)+1e-2;
            z = 2*x*(qCos(r+2)/r-qSin(r+2)/r); // the B field strength of dipole radiation (modulo physical constants)
            colorMap->data()->setCell(xIndex, yIndex, z);
        }
    }

    // add a color scale:
    QCPColorScale *colorScale = new QCPColorScale(customPlot);
    customPlot->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
    colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    colorMap->setColorScale(colorScale); // associate the color map with the color scale
    colorScale->axis()->setLabel("Magnetic Field Strength");

    // set the color gradient of the color map to one of the presets:
    colorMap->setGradient(QCPColorGradient::gpThermal);
    // we could have also created a QCPColorGradient instance and added own colors to
    // the gradient, see the documentation of QCPColorGradient for what's possible.

    // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
    colorMap->rescaleDataRange();

    // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    QCPMarginGroup *marginGroup = new QCPMarginGroup(customPlot);
    customPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

    // rescale the key (x) and value (y) axes so the whole color map is visible:
    customPlot->rescaleAxes();
}

void Widget::plotColorMap()
{
    DBF("void Widget::plotColorMap()");
    colorMap->data()->clear();
    int nx = life->getWidth();
    int ny = life->getHeight();
    //qDebug()<<DBG(nx) << DBG(ny) << frame[100][100];
    colorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
    colorMap->data()->setRange(QCPRange(-4, 4), QCPRange(-4, 4)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions
    // now we assign some data, by accessing the QCPColorMapData instance of the color map:
    //srand(time(0));
    //auto begin = std::chrono::steady_clock::now();
    for(int xIndex=0; xIndex<nx; ++xIndex)
    {
        for(int yIndex=0; yIndex<ny; ++yIndex)
        {
            //colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);
            //double z = rand()%10+100;
            colorMap->data()->setCell(xIndex, yIndex, life->frame[xIndex][yIndex]);
        }
    }


    RescaleCustomPlot(ui->colorMap); //делаем квадрат
    //colorMap->rescaleDataRange();
    ui->colorMap->replot();
    //auto end = std::chrono::steady_clock::now();
    //auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    //qDebug() << "The time: " << elapsed_ms.count() << " ms\n";
    update();
}


void Widget::resetScale()
{
    DBF("void Widget::resetScale()");
    ui->colorMap->rescaleAxes();
    ui->colorMap->replot();
}

void Widget::resetColor()
{
    DBF("void Widget::resetColor()");
    double range = pow(2, life->getBits()) * ui->RangeColorSpinBox->value() / 100.;
    colorMap->setDataRange(QCPRange(0,range));
    ui->colorMap->replot();

}
