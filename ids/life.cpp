#include "life.h"

Life::Life()
{
    cam = 0;
    stop = 0;
    lifeRun = 0;
    nBackground = 0;
    ppFrame = 0;
    sBgnd = 0;
    methodCenter = MethodCentre::CentreIntegrall;
}

Life::~Life()
{
    stop = 1;

    while (lifeRun) {
        Sleep(10);
    }
    delete  cam;
    Sleep(100);
    qDebug() << "delete Life";
}

void Life::startLife()
{
    if(cam){
        width = cam->getWidth();
        height = cam->getHeight();
        ppFrame = new float*[width];
        ppBackground = new float*[width];
        pSectionX = new QVector<double>(width);
        pSectionY = new QVector<double>(height);
        pAxisX = new QVector<double>(width);
        pAxisY = new QVector<double>(height);
        createAxis();

        for(int i = 0; i < width; i++)
        {
            ppFrame[i] = new float[height];
            ppBackground[i] = new float[height];
            for (int j = 0; j < height; j++){
                ppFrame[i][j] = 0;
                ppBackground[i][j] = 0;
            }
        }
        stop = 0;
        cam->startLive();
        start();
        emit lifeStartOk();
    }
}

void Life::stopLife()
{
    if(ppFrame)
    {
        for (int i = 0; i < width;i++) {
            delete[] ppFrame[i];
            delete[] ppBackground[i];
        }
        delete[] ppFrame;
        delete[] ppBackground;
        delete pSectionX;
        delete pSectionY;
        delete pAxisX;
        delete pAxisY;
        ppFrame = 0;
        ppBackground = 0;
    }
    if(cam){
        cam->stopLive();
        stop = 1;
    }
}

void Life::initCamera()
{
    //IdsCam::initCum(&cam);
    TestCam::initCum(&cam);
}

void Life::saveBackground(int n)
{
    nBackground = n;

}

void Life::run() //доделать
{
    lifeRun = 1;
    qDebug() << "start Life";
    while (!stop)
    {
        if(cam) {
            if(nBackground > 0){
                setBackground();
            }
            else {
                getFrame();
            }
        }


    }
    lifeRun = 0;
    qDebug() << "stop Life";
    exit();
}



void Life::setSubtractBackground(bool value)
{
    sBgnd = value;
}

void Life::lookForCenter(MethodCentre method)
{
    switch (method) {
    case MethodCentre::CentrerMax:
        centreMax();
        break;
    case MethodCentre::CentreIntegrall:
        centreIntegrall();
        break;
    }
}

void Life::centreMax()
{
    double max;
    int maxX, maxY;
    max = maxX = maxY = 0;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if(ppFrame[i][j] > max){
                max = ppFrame[i][j];
                maxX = i; maxY = j;
            }
        }
    }
    centre[0] = maxX; centre[1] = maxY;
}

void Life::centreIntegrall()
{
    double *iX = new double[height];
    double *iY = new double[width];
    for (int i = 0; i < width; i++) {
        iY[i] = 0;
    }
    for (int j = 0; j < height; j++) {
        iX[j] = 0;
    }

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            iX[j] += ppFrame[i][j];
            iY[i] += ppFrame[i][j];
        }
    }

    double max;
    int maxX, maxY;
    max = maxX = maxY = 0;
    for (int i = 0; i < width; i++) {
        if (iY[i] > max) {
            max = iY[i];
            maxX = i;
        }
    }
    max = 0;
    for (int j = 0; j < height; j++) {
        if (iX[j] > max) {
            max = iX[j];
            maxY = j;
        }
    }
    delete [] iX;
    delete [] iY;
    centre[0] = maxX; centre[1] = maxY;
}

void Life::setBackground()
{
    qDebug() << "сохранение фона" << nBackground;
    for(int i = 0; i < width; i++){
        for (int j = 0; j < height; j++) {
            ppBackground[i][j]=0;
        }
    }
    for (int k = 0; k < nBackground; k++)
    {
        emit stateSaveBCGR(double(k) / nBackground*100);
        qDebug() << "фон" << k;
        getFrame();
        for(int i = 0; i < width; i++){
            for (int j = 0; j < height; j++) {
                ppBackground[i][j]+=ppFrame[i][j];
            }
        }
    }
    for(int i = 0; i < width; i++){
        for (int j = 0; j < height; j++) {
            ppBackground[i][j] = ppBackground[i][j] / nBackground;
        }
    }
    emit stateSaveBCGR(100);

    nBackground = 0;

}

void Life::getFrame()
{
    if (cam->getFrame(ppFrame)){
        //обработка кадра
        if(sBgnd && !nBackground) subtractBackground();
        lookForCenter(methodCenter);
//        int x, y;
//        getCentre(x,y);
        getSections();
//        qDebug() << "центр" << x << y;
        //qDebug() << "frame";
        emit updateFrame();
    }
}

void Life::subtractBackground()
{
    for(int i = 0; i < width; i++){
        for (int j = 0; j < height; j++) {
            ppFrame[i][j] -= ppBackground[i][j];
        }
    }
}

void Life::createAxis()
{
    for (int i = 0; i < width; i++) {
        (*pAxisX)[i] = (i + 0.5) * cam->getPSize_mkm() / 1000.;
    }
    for (int i = 0; i < height; i++) {
        (*pAxisY)[i] = (i + 0.5) * cam->getPSize_mkm() / 1000.;
    }

}


void Life::getCentre(int &x, int &y) const
{
    x = centre[0];
    y = centre[1];
}

void Life::getSections()
{
    int x, y;
    getCentre(x,y);
    for (int i = 0; i < width; i++) {
        (*pSectionX)[i] = ppFrame[i][y];
    }
    for (int j = 0; j < height; j++) {
        (*pSectionY)[j] = ppFrame[x][j];
    }
}

int Life::getWidth() const
{
    return width;
}

int Life::getWidth_mm() const
{
    return width * cam->getPSize_mkm() / 1000.;
}

int Life::getHeight() const
{
    return height;
}

int Life::getHeight_mm() const
{
    return height * cam->getPSize_mkm() / 1000.;

}

