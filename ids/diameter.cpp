#include "life.h"

void Life::lookForDiamter(MethodDiameter method)
{
    DBF("void Life::lookForCenter(MethodCentre method)");
    if(cam){
        if(center.x >= 0 && center.y >=0) {
            switch (method) {
            case MethodDiameter::DiameterSlice:
                diameterSlice();
                diameter.width = edge.x2 - edge.x1;
                diameter.height = edge.y2 - edge.y1;
                break;
            case MethodDiameter::diameterSecondMoments:
                diameterSlice();
                diameterSecondMoments();
                break;
            }
        } else {
            diameter.width = -1;
            diameter.height = -1;
        }

    }
}


void Life::diameterSlice()
{
    DBF("void Life::diameterSlice()");

    int maxX = 0;
    for (int i = 0; i < averageSections.x.size(); i++){
        if(averageSections.x[i] > maxX){
            maxX = averageSections.x[i];
        }
    }
    int levelX = maxX * sliceLevel / 100.;

    for (int i = 0; i < averageSections.x.size()-1; i++) {
        if(averageSections.x[i] <= levelX && averageSections.x[i+1] >= levelX) {
            edge.x1 = i;
            break;
        }
    }
    for (int i = averageSections.x.size()-1; i > 0; i--) {
        if(averageSections.x[i] <= levelX && averageSections.x[i-1]  >= levelX) {
            edge.x2 = i;
            break;
        }
    }

    int maxY = 0;
    for (int i = 0; i < averageSections.y.size(); i++){
        if(averageSections.y[i] > maxY){
            maxY = averageSections.y[i];
        }
    }
    int levelY = maxY * sliceLevel / 100.;
    for (int i = 0; i < averageSections.y.size()-1; i++) {
        if(averageSections.y[i] <= levelY && averageSections.y[i+1] >= levelY) {
            edge.y1 = i;
            break;
        }
    }
    for (int i = averageSections.y.size()-1; i > 0; i--) {
        if(averageSections.y[i] <= levelY && averageSections.y[i-1]  >= levelY) {
            edge.y2 = i;
            break;
        }
    }
    edge.levelX = levelX;
    edge.levelY = levelY;
}


void Life::diameterSecondMoments()
{
    //sigma^2 = int(rho^2 * w(rho, phi), rho, phi) /  int(w(rho, phi), rho, phi)
    //sigma^2_x = int((x - x0)^2 * w(x,y), x, y) / int(w(x,y), x, y)
    //sigma^2_y = int((y - y0)^2 * w(x,y), x, y) / int(w(x,y), x, y)
    //sigma^2_x_y = int((x - x0) * (y - y0) * w(x,y), x, y) / int(w(x,y), x, y)

    //a0 = int(rho^2 * w(rho, phi), rho, phi)
    //b0 = int(w(rho, phi), rho, phi)
    //ax =  int((x - x0)^2 * w(x,y), x, y)
    //ay = int((y - y0)^2 * w(x,y), x, y)
    //axy =  int((x - x0) * (y - y0) * w(x,y), x, y)
    //b = int(w(x,y), x, y)

    int dx = edge.x2 - edge.x1;
    int dy = edge.y2 - edge.y1;

    int cut = pow(2, getBits()) * cutLevel / 100.;
    __int128 ax = 0;
    __int128 ay = 0;
    __int128 axy = 0;
    __int128 b = 0;
    int x1 = center.x - dx / 2 * 3;
    int x2 = center.x + dx / 2 * 3;
    int y1 = center.y - dy / 2 * 3;
    int y2 = center.y + dy / 2 * 3;
    if (x1 < 0) x1 = 0;
    if (x2 > range.width) x2 = range.width;
    if (y1 < 0) y1 = 0;
    if (y2 > range.height) y2 = range.height;
    for (int x = x1; x < x2; x++) {
        for (int y = y1; y < y2; y++) {
            if (frame[x][y] > cut) {
                ax += static_cast<__int128>(pow(x-center.x,2) * frame[x][y]);
                ay += static_cast<__int128>(pow(y-center.y,2) * frame[x][y]);
                axy += (x - center.x) * (y - center.y) * frame[x][y];
                b += frame[x][y];
            }

        }
        //qDebug() << ax<<ay<<axy;
    }

    //d = ) * sigma
    //dx = 4 * sigma_x
    //dy = 4 * sigma_y
    if (b > 0){
        double sigma2x = ax / b;
        double sigma2y = ay / b;
        double sigma2xy = axy / b;

        diameterSigma.dx = 4 * sqrt(sigma2x);
        diameterSigma.dy = 4 * sqrt(sigma2x);

        double k = pow(sigma2x - sigma2y,2) + 4 * pow(sigma2xy,2);
        diameterSigma.dSigmaBig = 2 * sqrt(2) * sqrt(sigma2x + sigma2y + sqrt(k));
        diameterSigma.dSigmaSmall = 2 * sqrt(2) * sqrt(sigma2x + sigma2y - sqrt(k));

        //phi = 1/2 arctan(2 sigma^2_xy / (sigma^2_x - sigma^2_y)
        //phi = pi / * sgn(sigma^2_xy)
        static const double pi = acos(-1.);
        double phiRad = 0;
        if (sigma2x == sigma2y){
            phiRad = pi  / 4 * copysign(1, sigma2xy);
        }
        else {
            phiRad = 1 / 2 * atan(2 * sigma2xy / (sigma2x - sigma2y));
        }

        diameterSigma.phi = phiRad / pi * 180;
    } else {
        diameterSigma.dx = NAN;
        diameterSigma.dy = NAN;
        diameterSigma.dSigmaBig = NAN;
        diameterSigma.dSigmaSmall = NAN;
        diameterSigma.phi = NAN;
}



}
