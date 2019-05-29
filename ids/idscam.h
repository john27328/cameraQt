#ifndef IDSCAM_H
#define IDSCAM_H

#include "cam.h"
class IdsCam: public Cam
{
public:
    static int initCum(Cam** cam, QString &model, QString &serial)
    {
        qDebug() << DBG(*cam);
        if(!(*cam)){
            qDebug() << "init cam";
            *cam = new IdsCam;
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
    ~IdsCam();
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
    int getFrame(float **frame);
    bool statusCam(); // - 1 - ok
    bool statusLife(); // - 1 - ok

private:
    IdsCam();
    HIDS hCam = 0;      // 0 for the next available camera. 1-254 to access by ID
    SENSORINFO sInfo;
    CAMINFO cInfo;
    HWND hWndDisplay;
    IS_RECT rectAOI;
    char* ppcImgMem; // кадр
    int memID;
    int pitch;
    HANDLE hEvent;

};

#endif // IDSCAM_H
