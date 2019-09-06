#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "life.h"
#include "qcustomplot.h"
#include <QVector>
#include <QTimer>
#include <stdlib.h>
//#include <iostream.h>
#include <time.h>
#include <chrono>
#include <QDateTime>
#include <QFile>
#include <QDataStream>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    Life *life;
    QCPColorMap *colorMap;
    void RescaleCustomPlot(QCustomPlot *plt);
    bool updateFr = 0;
    bool plot = 0;
    QTimer *timer1ms;
    void plotColorMap();
    void plotSections();
    void getMax();
    void diametr();
    void saveScreen(QString name);
    void saveData(QString name);



public slots:
    void updateFrame();
    void createColorMap();
    void createSections();
    void resetScale();
    void resetColor();
    void background(int state);
    void saveBackground();
    void rescaleSections();
    void initCamera();
    void plotFrame();
    void startLife();
    void average();
    void setSliceLevel();
    void setSetting();
    void getSetting();
    void CamDisconnect();
    void screenShot();
    void setCutLevel();

};

#endif // WIDGET_H
