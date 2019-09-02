#ifndef CAM_H
#define CAM_H
#include <QDebug>
//#include <windows.h>
#include <math.h>
#include <QString>

class Cam
{

public:
    virtual ~Cam(){}
    static int initCum(Cam** cam, QString &model, QString &serial)
    {
        *cam = 0;
        qDebug() << "функция инициализации не определена";
        model = "----";
        serial = "----";
        return 0;
    }
    virtual int setFPS(double &fps) = 0;
    virtual int setExp(double &exp) = 0;
    virtual int getFPS(double &fps) = 0;
    virtual int getExp(double &exp) = 0;
    virtual int getRangeFPS(double &minFPS, double &maxFPS) = 0;
    virtual int getRangeExp(double &minExp, double &maxExp) = 0;
    virtual int startLive() = 0;
    virtual int stopLive() = 0;
    virtual int getFrame(double** frame) = 0;
    virtual bool statusCam() = 0; // - 1 - ok
    virtual bool statusLife() = 0; // - 1 - ok
    virtual QString getSerial() = 0;
    virtual QString getModel() = 0;
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

    int getBits() const
    {
        return bits;
    }
protected:
    Cam(){}
    int width;
    int height;
    int bits;
    bool isOK;
    bool isLife;
    double pSize_mkm;

};



#endif // CAM_H


