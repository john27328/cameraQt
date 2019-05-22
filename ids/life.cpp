#include "life.h"

Life::Life()
{
    cam = 0;
    stop = 1;
    frameRun = 0;
    nBackground = 0;
    ppFrame = nullptr;
    ppBackground = nullptr;
    sBgnd = 0;
    methodCenter = MethodCentre::CentreIntegrall;
}

Life::~Life()
{
    stop = 1;

    while (frameRun) {
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
    if(stop == 0){
        if(cam){
            cam->stopLive();
            stop = 1;
        }
        if(ppFrame)
        {
//            for (int i = 0; i < width;i++) {
//                delete[] ppFrame[i];
//            }
//            delete[] ppFrame;
            delete pSectionX;
            delete pSectionY;
            delete pAxisX;
            delete pAxisY;
            ppFrame = nullptr;
        }
        if(ppBackground)
        {
//            for (int i = 0; i < width;i++) {
//                delete[] ppBackground[i];
//            }
//            delete[] ppBackground;
            ppBackground = nullptr;

        }
    }
}

void Life::initCamera(int c, QString &model, QString &serial)
{
    if (c)
    {
    }
    else {
        TestCam::initCum(&cam, model, serial);
    }

}

void Life::saveBackground(int n)
{
    nBackground = n;

}

void Life::run()
{
    frameRun = 1;
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
    frameRun = 0;
    qDebug() << "stop Life";
    exit();
}




void Life::setSubtractBackground(bool value)
{
    sBgnd = value;
}

void Life::startStopAverage(bool start, int n)
{
    qDebug()<<"startStopAverage" << start << n;
    if(start){
        averageQueue.clear();
        nAverage = n;
        isAverage = 1;
        //averageQueue = new QQueue<Sections>;
        for (int i = 0; i < n; i++){
            Sections tmp = {*pSectionX, *pSectionY};
            averageQueue.enqueue(tmp);
            averageState = 0;
        }
    }
    else {
        isAverage = 0;
        averageQueue.clear();
        averageState = 100;
    }
    averageSections.x = *pSectionX;
    averageSections.y = *pSectionY;
    qDebug()<<"startStopAverage end";
}


void Life::average()
{
    Sections tmp = {*pSectionX, *pSectionY};
    qDebug() << DBG(tmp.y.size());
    averageQueue.enqueue(tmp);
    Sections tmp2 = tmp / nAverage;
    averageSections+=tmp / nAverage;
    averageSections-=averageQueue.dequeue() / nAverage;
    averageState = averageState <= 100 ? averageState + 100 / nAverage : 0;
}

void Life::setSliceLevel(double sliceLevel)
{
    if (cam)
        this->sliceLevel = sliceLevel;
}


void Life::lookForCenter(MethodCentre method)
{
    if(cam){
        switch (method) {
        case MethodCentre::CentrerMax:
            centreMax();
            break;
        case MethodCentre::CentreIntegrall:
            centreIntegrall();
            break;
        }
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
    maxP[0] = maxX; maxP[1] = maxY; maxP[2] = max;

}

void Life::centreIntegrall()
{
    if(cam){
        double *iX = new double[height];
        double *iY = new double[width];
        for (int i = 0; i < width; i++) {
            iY[i] = 0;
        }
        for (int j = 0; j < height; j++) {
            iX[j] = 0;
        }

        double max;
        int maxX, maxY;
        max = maxX = maxY = 0;
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                iX[j] += ppFrame[i][j];
                iY[i] += ppFrame[i][j];
                if(ppFrame[i][j] > max){
                    max = ppFrame[i][j];
                    maxX = i; maxY = j;
                }
            }
        }
        maxP[0] = maxX; maxP[1] = maxY; maxP[2] = max;

        max = 0;
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
    if (cam){
        if (cam->getFrame(ppFrame)){
            //обработка кадра
            if(sBgnd && !nBackground) subtractBackground();
            lookForCenter(methodCenter);
            //        int x, y;
            //        getCentre(x,y);
            getSections();
            if(isAverage)
                average();
            else {
                averageSections.x = *pSectionX;
                averageSections.y = *pSectionY;
            }
            diameter();
            //        qDebug() << "центр" << x << y;
            //qDebug() << "frame";
            emit updateFrame();
        }
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

void Life::diameter()
{
    int max = maxP[2];
    int level = max * sliceLevel / 100.;
    for (int i = 0; i < averageSections.x.size()-1; i++) {
        if(averageSections.x[i] <= level && averageSections.x[i+1] >= level) {
            diameterMas[0] = i;
            break;
        }
    }
    for (int i = averageSections.x.size()-1; i > 0; i--) {
        if(averageSections.x[i] <= level && averageSections.x[i-1]  >= level) {
            diameterMas[1] = i;
            break;
        }
    }
    for (int i = 0; i < averageSections.y.size()-1; i++) {
        if(averageSections.y[i] <= level && averageSections.y[i+1] >= level) {
            diameterMas[2] = i;
            break;
        }
    }
    for (int i = averageSections.y.size()-1; i > 0; i--) {
        if(averageSections.y[i] <= level && averageSections.y[i-1]  >= level) {
            diameterMas[3] = i;
            break;
        }
    }
}

void Life::createAxis()
{
    if (cam){
        for (int i = 0; i < width; i++) {
            (*pAxisX)[i] = (i + 0.5) * cam->getPSize_mkm() / 1000.;
        }
        for (int i = 0; i < height; i++) {
            (*pAxisY)[i] = (i + 0.5) * cam->getPSize_mkm() / 1000.;
        }
    }

}

int Life::getAverageState() const
{
    return averageState;
}

double Life::setFPS(double fps)
{
    if (cam){
        cam->setFPS(fps);
        return fps;
    }
    else{
        return NAN;
    }
}

double Life::setExp(double exp)
{
    if(cam){
        cam->setExp(exp);
        return exp;
    }
    else {
        return NAN;
    }
}

double Life::getFPS()
{

}

void Life::setSetting(double &minFps, double &maxFps, double &fps, double &minExp, double &maxExp, double &exp)
{
    if(cam){
        cam->setFPS(fps);
        cam->getRangeFPS(minFps, maxFps);
        cam->setExp(exp);
        cam->getRangeExp(minExp, maxExp);
    }
}

void Life::getSetting(double &minFps, double &maxFps, double &fps, double &minExp, double &maxExp, double &exp)
{
    if(cam){
        cam->getFPS(fps);
        cam->getRangeFPS(minFps, maxFps);
        cam->getExp(exp);
        cam->getRangeExp(minExp, maxExp);
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

void Life::getMax(int &x, int &y, int &z) const
{
    x = maxP[0];
    y = maxP[1];
    z = maxP[2];
}

int Life::getBits() const
{
    if(cam){
        return cam->getBits();
    }
}

int Life::getDiametr(int &x1, int &x2, int &y1, int &y2) const
{
    if(diameterMas[0] >= 0 &&
            diameterMas[1] >= 0 &&
            diameterMas[2] >= 0 &&
            diameterMas[3] >= 0){
        x1 = diameterMas[0];
        x2 = diameterMas[1];
        y1 = diameterMas[2];
        y2 = diameterMas[3];
        return 1;
    }
    else {
        return 0;
    }


}

int Life::getWidth() const
{
    return width;
}

int Life::getWidth_mm() const
{
    if(cam){
        return width * cam->getPSize_mkm() / 1000.;
    }
}

int Life::getHeight() const
{
    return height;
}

int Life::getHeight_mm() const
{
    if(cam){
        return height * cam->getPSize_mkm() / 1000.;
    }

}

double Life::pixelTo_mm(int p)
{
    if(cam){
        return cam->getPSize_mkm()/1000.*p;
    }
}

bool Life::statusCam()
{
    return cam;
}

