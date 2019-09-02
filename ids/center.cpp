#include "life.h"

void Life::lookForCenter(MethodCenter method)
{
    DBF("void Life::lookForCenter(MethodCentre method)");
    if(cam){
        switch (method) {
        case MethodCenter::CentreMoments:
            centrerMoments();
            break;
        case MethodCenter::CentreIntegrall:
            centerIntegrall();
            break;
        }
    }
}


void Life::centerMax()
{
    DBF("void Life::centreMax()");
    double max;
    int maxX, maxY;
    max = maxX = maxY = 0;
    for (int i = 0; i < range.width; i++) {
        for (int j = 0; j < range.height; j++) {
            if(frame[i][j] > max){
                max = frame[i][j];
                maxX = i; maxY = j;
            }
        }
    }
    maxPower.x = maxX; maxPower.y = maxY; maxPower.power = static_cast<int>(max + 0.5);

}

void Life::centerIntegrall()
{
    DBF("void Life::centreIntegrall()");

    if(cam){
        double *iX = new double[range.height];
        double *iY = new double[range.width];
        for (int i = 0; i < range.width; i++) {
            iY[i] = 0;
        }
        for (int j = 0; j < range.height; j++) {
            iX[j] = 0;
        }

        double max;
        int maxX, maxY;
        max = maxX = maxY = 0;
        for (int i = 0; i < range.width; i++) {
            for (int j = 0; j < range.height; j++) {
                iX[j] += frame[i][j];
                iY[i] += frame[i][j];
            }
        }

        max = 0;
        for (int i = 0; i < range.width; i++) {
            if (iY[i] > max) {
                max = iY[i];
                maxX = i;
            }
        }
        max = 0;
        for (int j = 0; j < range.height; j++) {
            if (iX[j] > max) {
                max = iX[j];
                maxY = j;
            }
        }
        delete [] iX;
        delete [] iY;
        center.x = maxX; center.y = maxY;
    }
}

void Life::centrerMoments()
{
    double levelMomets = 0;
    // x0 = integral(x * w(x,y), x,y) / int(w(x,y),x,y)
    // a = integral(x * w(x,y), x,y)
    // b = integral(w(x,y),x,y)
    double ax = 0;
    double b = 0;
    double ay = 0;
    double w = 0;
    for (int i = 0; i < range.width; i++) {
        for (int j = 0; j < range.height ; j++) {
            w = frame[i][j] < levelMomets ? 0 : frame[i][j];
            ax += i * w;
            ay += j * w;
            b += w;
        }
    }
    center.x = ax / b;
    center.y = ay / b;
}
