#ifndef LIFE_H
#define LIFE_H
#include <QThread>
#include <math.h>
#include "idscam.h"
#include "testcam.h"
#include "windows.h"
#include <QVector>
#include <QQueue>
#include "debug.h"


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
    double pixelTo_mm(int p);
    bool statusCam();
    bool statusLife();

    void getCentre(int &x, int &) const;
    void getSections();
    enum class MethodCentre{CentrerMax, CentreIntegrall};
    float** frame;
    float** frameFinal;
    float** background;
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
    int getDiametr(int &x1, int &x2, int &y1, int &y2) const;
    int getLevel(int &levelX, int &levelY) const;

    int getAverageState() const;

    void setSetting(double &minFps, double &maxFps, double &fps,
                    double &minExp, double &maxExp, double &exp);
    void getSetting(double &minFps, double &maxFps, double &fps,
                    double &minExp, double &maxExp, double &exp);

    void initCamera(int c, QString &model, QString &serial);
    void startLife();

private:
    void run();
    bool stop;
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
    int diameterMas[6]; // 0 - x1, 1 - x2, 2 - y2, 3 - y2, 4 - levelX, 5 - levelY
    double sliceLevel;
    void diameter();
    void createAxis();
    QQueue<Sections> averageQueue;
    int nAverage;
    int averageState;
    void average();
    bool isAverage;
    void frameCopy( float**frame1, float **frame2);

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
