#ifndef TESTCAM_H
#define TESTCAM_H
#include "cam.h"


class TestCam: public Cam
{
public:
    ~TestCam();
    static int initCum(Cam** cam);
    int setFPS(double &fps);
    int getRangeFPS(double &minFPS, double &maxFPS);
    int startLive();
    int stopLive();
    int getFrame(float **frame);
    bool statusCam(); // - 1 - ok
    bool statusLife(); // - 1 - ok
private:
    TestCam **thisCam;
    TestCam();
    int fpsCam;

};

#endif // TESTCAM_H
