#include "life.h"

Life::Life()
{
    cam = 0;
    stop = 1;
    frameRun = 0;
    nBackground = 0;
    sBgnd = 0;
    methodCenter = MethodCentre::CentreIntegrall;
    frame = nullptr;
    background = nullptr;
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
        pSectionX = new QVector<double>(width);
        pSectionY = new QVector<double>(height);
        pAxisX = new QVector<double>(width);
        pAxisY = new QVector<double>(height);
        createAxis();

        frame = new float*[width];
        background = new float*[width];
        for(int i = 0; i < width; i++)
        {
            frame[i] = new float[height];
            background[i] = new float[height];
            for (int j = 0; j < height; j++){
                frame[i][j] = 0;
                background[i][j] = 0;
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
    stop = 1;
    while (frameRun) {
        Sleep(10);
    }
    if(cam){
        cam->stopLive();
    }
    delete pSectionX;
    delete pSectionY;
    delete pAxisX;
    delete pAxisY;
    if(frame)
    {
        for (int i = 0; i < width;i++) {
            delete[] frame[i];
        }
        delete[] frame;
        frame = nullptr;
    }
    if(background)
    {
        for (int i = 0; i < width;i++) {
            delete[] background[i];
        }
        delete[] background;
        background = nullptr;
    }


}

void Life::initCamera(int c, QString &model, QString &serial)
{
    if (c)
    {
        IdsCam::initCum(&cam);
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
    qDebug() << "average 0" << nAverage;
    averageSections+=tmp / nAverage;
    qDebug() << "average 1";
    averageSections-=averageQueue.dequeue() / nAverage;
<<<<<<< HEAD
    averageState = averageState <= 100 ? averageState + 100 / nAverage : 0;
}

void Life::setSliceLevel(double sliceLevel)
{
    if (cam)
        this->sliceLevel = sliceLevel;
=======
    qDebug() << "average 2";
    averageState = averageState <= 100 ? averageState + 100 / nAverage : 0;
    qDebug() << "average end" << averageState;
>>>>>>> parent of a72803e... опять удалил ids
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
            if(frame[i][j] > max){
                max = frame[i][j];
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
                iX[j] += frame[i][j];
                iY[i] += frame[i][j];
                if(frame[i][j] > max){
                    max = frame[i][j];
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
            background[i][j]=0;
        }
    }
    for (int k = 0; k < nBackground; k++)
    {
        emit stateSaveBCGR(double(k) / nBackground*100);
        qDebug() << "фон" << k;
        getFrame();
        for(int i = 0; i < width; i++){
            for (int j = 0; j < height; j++) {
                background[i][j]+=frame[i][j];
            }
        }
    }
    for(int i = 0; i < width; i++){
        for (int j = 0; j < height; j++) {
            background[i][j] = background[i][j] / nBackground;
        }
    }
    emit stateSaveBCGR(100);

    nBackground = 0;

}

void Life::getFrame()
{
    if (cam){
        if (cam->getFrame(frame)){
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
            frame[i][j] -= background[i][j];
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
        (*pSectionX)[i] = frame[i][y];
    }
    for (int j = 0; j < height; j++) {
        (*pSectionY)[j] = frame[x][j];
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

bool Life::statusLife()
{
    return !stop;
}

