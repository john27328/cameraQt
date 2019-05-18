#ifndef LIFE_H
#define LIFE_H
#include <QThread>
#include <math.h>
#include "idscam.h"
#include "testcam.h"
#include "windows.h"
#include <QVector>


class Life: public QThread
{
Q_OBJECT

public:
    Life();
    ~Life();


    int getWidth() const;
    int getWidth_mm() const;
    int getHeight() const;
    int getHeight_mm() const;


    void getCentre(int &x, int &) const;
    void getSections();
    enum class MethodCentre{CentrerMax, CentreIntegrall};
    float **ppFrame;
    float **ppBackground;
    QVector<double> *pSectionX;
    QVector<double> *pSectionY;
    QVector<double> *pAxisX;
    QVector<double> *pAxisY;
    void getMax(int &x, int &y, int &z);
    int getBits() const;


private:
    void run();
    bool stop;
    bool lifeRun;
    Cam *cam;
    int width;
    int height;
    bool sBgnd;
    int nBackground;
    void setBackground();
    void lookForCenter(MethodCentre method);
    void centreMax();
    void centreIntegrall();
    MethodCentre methodCenter;
    void getFrame();
    void subtractBackground();
    int centre[2];// 0 - x, 1 - y
    int maxP[3]; // 0 - x, 1 - y, P
    void createAxis();

public slots:
    void startLife();
    void stopLife();
    void initCamera(int c);
    void saveBackground(int n);
    void setSubtractBackground(bool value);
signals:
    void updateFrame();
    void stateSaveBCGR(int);
    void lifeStartOk();
};

#endif // LIFE_H
