#include "life.h"

void Life::lookForCenter(MethodCentre method)
{
    DBF("void Life::lookForCenter(MethodCentre method)");
    if(cam){
        switch (method) {
        case MethodCentre::CentreMoments:
            centreMoments();
            break;
        case MethodCentre::CentreIntegrall:
            centreIntegrall();
            break;
        }
    }
}


void Life::centreMax()
{
    DBF("void Life::centreMax()");
    double max;
    int maxX, maxY;
    max = maxX = maxY = 0;
    for (int i = 0; i < range.width; i++) {
        for (int j = 0; j < range.height; j++) {
            if((double)frame[i][j] > max){
                max = (double)frame[i][j];
                maxX = i; maxY = j;
            }
        }
    }
    maxPower.x = maxX; maxPower.y = maxY; maxPower.power = (int)(max + 0.5);

}

void Life::centreIntegrall()
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
        centre.x = maxX; centre.y = maxY;
    }
}

void Life::centreMoments()
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
            w = (double)frame[i][j] < levelMomets ? 0 : (double)frame[i][j];
            ax += i * w;
            ay += j * w;
            b += w;
        }
    }
    centre.x = ax / b;
    centre.y = ay / b;
}
