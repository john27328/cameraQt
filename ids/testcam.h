#ifndef TESTCAM_H
#define TESTCAM_H
#include "cam.h"


class TestCam: public Cam
{
public:
    ~TestCam();
    static int initCum(Cam** cam, QString &model, QString &serial)
    {
        qDebug() << DBG(*cam);
        if(!(*cam)){
            qDebug() << "init cam";
            *cam = new TestCam;
            (*cam)->thisCam = cam;
            if((*cam)->statusCam()){
                model = (*cam)->getModel();
                serial = (*cam)->getSerial();
                return 1;
            }
            else {
                delete *cam;
                *cam = 0;
                model = "error";
                serial = "----";
                return 0;
            }
        }
        else {
            qDebug() << "delete cam";
            delete (*cam);
            *cam = 0;
            model = "----";
            serial = "----";
            return 0;
        }
    }
    QString getModel();
    QString getSerial();
    int setFPS(double &fps);
    int setExp(double &exp);
    int getFPS(double &fps);
    int getExp(double &exp);
    int getRangeFPS(double &minFPS, double &maxFPS);
    int getRangeExp(double &minExp, double &maxExp);
    int startLive();
    int stopLive();
    int getFrame(Frame &frame);
    bool statusCam(); // - 1 - ok
    bool statusLife(); // - 1 - ok
protected:
private:    TestCam();

    double fpsCam;
    double minFps=1;
    double maxFps=5;
    double expCam;
    double minExp=1;
    double maxExp=200;

};

#endif // TESTCAM_H
