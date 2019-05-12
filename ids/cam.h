#ifndef CAM_H
#define CAM_H
#include <QDebug>
#include <windows.h>
#include <math.h>
#define DBG(x) #x << (x)

class Cam
{

public:
    Cam(){}
    virtual ~Cam(){}
    virtual int setFPS(double &fps) = 0;
    virtual int getRangeFPS(double &minFPS, double &maxFPS) = 0;
    virtual int startLive() = 0;
    virtual int stopLive() = 0;
    virtual int getFrame(float **frame) = 0;
    virtual bool statusCam() = 0; // - 1 - ok
    virtual bool statusLife() = 0; // - 1 - ok


    Cam **thisCam;

    int getWidth() const
    {
        return width;
    }
    int getHeight() const
    {
        return height;
    }

    double getPSize_mkm() const
    {
        return pSize_mkm;
    }
protected:
    int width;
    int height;
    bool isOK;
    bool isLife;
    double pSize_mkm;
};



#endif // CAM_H
