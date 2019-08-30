#ifndef TESTCAM_H
#define TESTCAM_H
#include "cam.h"
#include "debug.h"


class TestCam: public Cam
{
public:
    ~TestCam();
    static int initCum(Cam** cam, QString &model, QString &serial)
    {
        qDebug() << DV(*cam);
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
    int getFrame(float** frame);
    bool statusCam(); // - 1 - ok
    bool statusLife(); // - 1 - ok
protected:
private:    TestCam();

    double fpsCam;
    double minFps;
    double maxFps;
    double expCam;
    double minExp;
    double maxExp;

};

#endif // TESTCAM_H
