#ifndef IDSCAM_H
#define IDSCAM_H

#include "ueye.h"
#include "cam.h"
class IdsCam: public Cam
{
public:
    static int initCum(Cam** cam);
    ~IdsCam();
    int setFPS(double &fps);
    int getRangeFPS(double &minFPS, double &maxFPS);
    int startLive();
    int stopLive();
    int getFrame(float **frame);
    bool statusCam(); // - 1 - ok
    bool statusLife(); // - 1 - ok

private:
    IdsCam();
    IdsCam **thisCam;
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
