#include "testcam.h"
#include <time.h>
#include <chrono>
#include <debug.h>


TestCam::TestCam(): minFps(1),  maxFps(5),  minExp(1),maxExp(200)
{
    DBF("TestCam::TestCam()");
    width = height = 2000;
    pSize_mkm = 5.5;
    qDebug()<<"testCam создан";
    fpsCam = 2;
    bits = 12;
}

TestCam::~TestCam()
{
    DBF("TestCam::~TestCam()");
    *thisCam = nullptr;
    qDebug()<<"testCam удален";
}

QString TestCam::getModel()
{
    return  "testCam";
}

QString TestCam::getSerial()
{
    return "000";
}



int TestCam::setFPS(double &fps)
{
    fpsCam = fps;
    fps+=0.1;
    return 1;
}

int TestCam::setExp(double &exp)
{
    expCam = exp;

    exp+=0.2;
    return 1;
}

int TestCam::getFPS(double &fps)
{
    fps = fpsCam;
    return 1;
}

int TestCam::getExp(double &exp)
{
    exp = expCam;
    return 1;
}

int TestCam::getRangeFPS(double &minFPS, double &maxFPS)
{
    minFPS = minFps;
    maxFPS = maxFps;
    return 1;
}

int TestCam::getRangeExp(double &minExp, double &maxExp)
{
    minExp = this->minExp;
    maxExp = this->maxExp;
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

#include <unistd.h>
int TestCam::getFrame(double **frame)
{
    DBF("int TestCam::getFrame(float **frame)");

    srand(time(nullptr));
    auto begin = std::chrono::steady_clock::now();
    static int k = 0;
    k = (k + 10)%2000;
    sleep(0);
    double x;
    double y;
    double a = 10 * expCam;
    double x0 = 1000;
    double y0 = 1000;
    double sigmaX = 20;
    double sigmaY = 20;
    double z;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            x = i; y = j;
            z = a * exp(-(pow((x - x0),2) / 2 / pow(sigmaX,2) +
                          pow((y - y0),2) / 2 /pow(sigmaY,2)));
            frame[i][j] = rand()%100 + 100 + z;
//            frame[i][j] = 0 + z;

//            frame[i][j] = i + j + 100;
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

