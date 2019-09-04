#include "life.h"

void Life::lookForDiamter(MethodDiameter method)
{
    DBF("void Life::lookForCenter(MethodCentre method)");
    if(cam){
        switch (method) {
        case MethodDiameter::DiameterSlice:
            diameterSlice();
            diameter.width = edge.x2 - edge.x1;
            diameter.height = edge.y2 - edge.y1;
            break;
        case MethodDiameter::diameterSecondMoments:
            diameterSecondMoments();
            break;
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

    double ax = 0;
    long int ay = 0;
    long int axy = 0;
    long int b = 0;
    for (int x = 0; x < range.width; x++) {
        for (int y = 0; y < range.height; y++) {
            ax += pow(x-center.x,2) * frame[x][y];
            ay += pow(y-center.y,2) * frame[x][y];
            axy += (x - center.x) * (y - center.y) * frame[x][y];
            b += frame[x][y];
        }
    }

    //d = ) * sigma
    //dx = 4 * sigma_x
    //dy = 4 * sigma_y
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

}
