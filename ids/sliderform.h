#ifndef SLIDERFORM_H
#define SLIDERFORM_H

#include <QWidget>

namespace Ui {
class SliderForm;
}

class SliderForm : public QWidget
{
    Q_OBJECT

public:
    explicit SliderForm(QWidget *parent = nullptr);
    ~SliderForm();
    void setRange(double min, double max);
    void setValue(double value);
    double getValue();

private:
    Ui::SliderForm *ui;
};

#endif // SLIDERFORM_H
