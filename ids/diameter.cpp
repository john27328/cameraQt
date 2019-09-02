#include "life.h"

void Life::lookForDiamter(MethodDiameter method)
{
    DBF("void Life::lookForCenter(MethodCentre method)");
    if(cam){
        switch (method) {
        case MethodDiameter::DiameterSlice:
            diameterSlice();
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

    int ax = 0;
    int ay = 0;
    int axy = 0;
    int b = 0;
    for (int x = 0; x < range.width; x++) {
        for (int y = 0; y < range.height; y++) {
            ax += pow(x-center.x,2) * frame[x][y];
            ay += pow(y-center.y,2) * frame[x][y];
            axy += (x - center.x) * (y - center.y) * frame[x][y];
            b += frame[x][y];
        }
    }

    //d = 2 * sqrt(2) * sigma
    //dx = 4 * sigma_x
    //dy = 4 * sigma_y

    int dx = 4 * ax / b;
    int dy = 4 * ay / b;

}
