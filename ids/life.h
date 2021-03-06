﻿#ifndef LIFE_H
#define LIFE_H
#include <QThread>
#include <math.h>
#include "idscam.h"
#include "testcam.h"
#include "windows.h"
#include <QVector>
#include <QQueue>
#include <QDebug>



class Life: public QThread
{
Q_OBJECT

public:
    Life();
    ~Life();
	using Long = long long;

    int getWidth() const;
    int getWidth_mm() const;
    int getHeight() const;
    int getHeight_mm() const;
    double pixelTo_mm(double p) const;
    bool statusCam();
    bool statusLife();

    void getCentre(int &x, int &) const;
    void getSections();
    enum class MethodCenter{CentreIntegrall, CentreMoments};
    enum class MethodDiameter{DiameterSlice, diameterSecondMoments};
    double** frame;
    double** frameFinal;
    double** background;
    typedef QVector<double> Section;

    struct Sections{
        Section x;
        Section y;
        Sections operator+=(Sections s){
//            qDebug() << "operator+";
            for (int i = 0; i < this->x.size(); i++){
//                qDebug() << DBG(i) <<DBG(this->x[i]) << DBG(s.x[i]) <<DBG(this->y[i]) << DBG(s.y[i]);
                this->x[i] += s.x[i];
                this->y[i] += s.y[i];
            }
//            qDebug() << "operator+ end";
            return *this;

        }
        Sections operator-=(Sections s){
            for (int i = 0; i < this->x.size(); i++){
                this->x[i] -= s.x[i];
                this->y[i] -= s.y[i];
            }
            return *this;
        }
        Sections operator/(double const &n){
            Sections tmp = *this;
            for (int i = 0; i < this->x.size(); i++){
                tmp.x[i] /= n;
                tmp.y[i] /= n;
            }
            return tmp;
        }
    };

    Section *pSectionX;
    Section *pSectionY;
    Section *pAxisX;
    Section *pAxisY;
    Sections averageSections;

    void getMax(int &x, int &y, int &z) const;
    int getBits() const;
    int getEdge(int &x1, int &x2, int &y1, int &y2) const;
    int getLevel(int &levelX, int &levelY) const;
    int getDiameter(int &dx, int &dy) const;
    int getDiameter_mm(double &dx, double &dy) const;
    int getDiameterSigma(double &d, double &dx, double &dy, double &dBig, double &dSmall, double phi);
    int getDiameterSigma_mm(double &d, double &dx, double &dy, double &dBig, double &dSmall, double phi);

    int getAverageState() const;

    void setSetting(double &minFps, double &maxFps, double &fps,
                    double &minExp, double &maxExp, double &exp);
    void getSetting(double &minFps, double &maxFps, double &fps,
                    double &minExp, double &maxExp, double &exp);

    void initCamera(int c, QString &model, QString &serial);
    void startLife();
    int setMerhodDiameter(int i);

    int getCutLevel() const;

    void setCutLevel(int value);

private:
    void run();
    bool stop;
    Cam *cam;

    struct Range
    {
        Range():width(0), height(0) {}
        int width;
        int height;
    } range, diameter;

    bool sBgnd;
    int nBackground;
    void setBackground();
    void lookForCenter(MethodCenter method);
    //void max();
    void centerMax();
    void centerIntegrall();
    void centrerMoments();
    MethodCenter methodCenter;
    MethodDiameter methodDiameter;
    void getFrame();
    void subtractBackground();

    struct Point // в пикселях
    {
        Point(): x(0), y(0) {}
        int x;
        int y;
    } center;

    struct Max // в попугаях
    {
        Max(): x(0), y(0), power(0) {}
        int x;
        int y;
        int power;
    } maxPower;

    struct Edge
    {
        Edge(): x1(0), x2(0), y1(0), y2(0), levelX(0), levelY(0){}
        int x1;
        int x2;
        int y1;
        int y2;
        int levelX;
        int levelY;
    } edge;

    struct DiameterSigma
    {
        DiameterSigma() {}
        double d = 0;
        double dx = 0;
        double dy = 0;
        double dxy = 0;
        double dSigmaBig = 0;
        double dSigmaSmall = 0;
        double phi = 0;
    } diameterSigma;

    double sliceLevel;
    void lookForDiamter(MethodDiameter method);
    void diameterSlice();
    void diameterSecondMoments();
    void createAxis();
    QQueue<Sections> averageQueue;
    int nAverage;
    int averageState;
    void averageSec();
    void averageFrameFunction();
    void average(MethodDiameter method);
    void summFrame(const double **frame1, const double **frame2, double **frameOut);
    void minusFrame(const double **frame1, const double **frame2, double **frameOut);
    void multFrame(const double **frame, double d, double **frameOut);
    double ** averageFrame;
    QQueue<double **> frameQueue;
    bool isAverage;
    void frameCopy(double **frame1, double **frameOut);
    void frameClear(double **frame);
    int cutLevel;

    void summFrame(double **frame1, double **frame2, double **frameOut);
    void minusFrame(double **frame1, double **frame2, double **frameOut);
    void multFrame(double **frame, double d, double **frameOut);
    double **newFrame();
    void deleteFrame(double **frame);
public slots:
    void setSliceLevel(double slicelevel);
    void saveBackground(int n);
    void setSubtractBackground(bool value);
    void startStopAverage(bool start, int n);
signals:
    void updateFrame();
    void stateSaveBCGR(int);
    void lifeStartOk();
    void disconnectCam();
};


#endif // LIFE_H
