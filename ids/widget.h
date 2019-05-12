#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "life.h"
#include "qcustomplot.h"
#include <QVector>

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

public slots:
    void createColorMap();
    void createSections();
    void plotColorMap();
    void resetScale();
    void resetColor();
    void background(int state);
    void saveBackground();
    void plotSections();


};

#endif // WIDGET_H
