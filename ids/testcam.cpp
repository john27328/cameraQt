#include "testcam.h"
#include <time.h>
#include <chrono>


TestCam::TestCam()
{
    width = height = 2000;
    pSize_mkm = 5.5;
    qDebug()<<"testCam создан";
    fpsCam = 2;
}

TestCam::~TestCam()
{
    qDebug()<<"testCam удален";
}

int TestCam::initCum(Cam **cam)
{
    qDebug() << DBG(*cam);
    if(!(*cam)){
        qDebug() << "init cam";
        *cam = new TestCam;
        (*cam)->thisCam = cam;
        if((*cam)->statusCam()){
            return 1;
        }
        else {
            delete *cam;
            *cam = 0;
            return 0;
        }
    }
    else {
        qDebug() << "delete cam";
        delete (*cam);
        *cam = 0;
        return 0;
    }
}

int TestCam::setFPS(double &fps)
{
    fpsCam = fps;
    return 1;
}

int TestCam::getRangeFPS(double &minFPS, double &maxFPS)
{
    minFPS = 1;
    maxFPS = 100;
    return 1;
}

int TestCam::startLive()
{
    return 1;
}

int TestCam::stopLive()
{
    return 1;
}


int TestCam::getFrame(float **frame)
{
    srand(time(0));
    auto begin = std::chrono::steady_clock::now();
    static int k = 0;
    k = (k + 10)%2000;
    Sleep(100);
    double x;
    double y;
    double a = 100;
    double x0;
    double y0 = 1000;
    double sigmaX = 20;
    double sigmaY = 20;
    double z;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            x = i; y = j; x0 = k;
            z = a * exp(-(pow((x - x0),2) / 2 / pow(sigmaX,2) +
                          pow((y - y0),2) / 2 /pow(sigmaY,2)));
            frame[i][j] = rand()%100 + 100 + z;
        }
    }
      auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    qDebug() << "getFrame time: " << elapsed_ms.count() << " ms\n";
    return 1;
}

bool TestCam::statusCam()
{
    return 1;
}

bool TestCam::statusLife()
{
    return 1;
}

