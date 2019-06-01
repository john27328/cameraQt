#include "life.h"

Life::Life(): frame(nullptr), background(nullptr), pSectionX(nullptr),
    pSectionY(nullptr), pAxisX(nullptr), pAxisY(nullptr), cam(nullptr), isAverage(0)
{
    cam = nullptr;
    stop = 1;
    nBackground = 0;
    sBgnd = 0;
    methodCenter = MethodCentre::CentreIntegrall;
    frame = nullptr;
    background = nullptr;
}

Life::~Life()
{
    stop = 1;

    delete  cam;
    Sleep(100);
    qDebug() << "delete Life";
}

void Life::startLife()
{
    DBF("void Life::startLife()");
    if(cam && stop == 1){
        cam->startLive();
        stop = 0;
        start();
        emit lifeStartOk();
    }
    else{
        stop = 1;
    }
}

void Life::initCamera(int c, QString &model, QString &serial)
{
    DBF("void Life::initCamera(int c, QString &model, QString &serial)");
    if (c)
    {
        //IdsCam::initCum(&cam, model, serial);
    }
    else {
        TestCam::initCum(&cam, model, serial);
    }

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

    }
    else{
        stop = 1;
        Sleep(100);
        delete pSectionX;
        pSectionX = nullptr;
        delete pSectionY;
        pSectionY = nullptr;
        delete pAxisX;
        pAxisX = nullptr;
        delete pAxisY;
        pAxisY = nullptr;
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
}


void Life::saveBackground(int n)
{
    nBackground = n;

}

void Life::run()
{
    DBF("void Life::run()");
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
    if(cam){
        cam->stopLive();
    }
    qDebug() << "stop Life";
    exit();
}




void Life::setSubtractBackground(bool value)
{
    sBgnd = value;
}

void Life::startStopAverage(bool start, int n)
{
    DBF("void Life::startStopAverage(bool start, int n)");
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
        //        averageQueue.clear();
        averageState = 100;
    }
    averageSections.x = *pSectionX;
    averageSections.y = *pSectionY;
    //qDebug()<<"startStopAverage end";
}


void Life::average()
{
    DBF("void Life::average()");
    Sections tmp = {*pSectionX, *pSectionY};
    //    qDebug() << DBG(tmp.y.size());
    averageQueue.enqueue(tmp);
    //qDebug() << "average 0" << nAverage;
    averageSections+=tmp / nAverage;
    //qDebug() << "average 1";
    averageSections-=averageQueue.dequeue() / nAverage;
    averageState = averageState <= 100 ? averageState + 100 / nAverage : 0;
}

void Life::setSliceLevel(double sliceLevel)
{
    DBF("void Life::setSliceLevel(double sliceLevel)");
    if (cam)
        this->sliceLevel = sliceLevel;
    //qDebug() << "average end" << averageState;
}


void Life::lookForCenter(MethodCentre method)
{
    DBF("void Life::lookForCenter(MethodCentre method)");
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
    DBF("void Life::centreMax()");
    double max;
    int maxX, maxY;
    max = maxX = maxY = 0;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if((double)frame[i][j] > max){
                max = (double)frame[i][j];
                maxX = i; maxY = j;
            }
        }
    }
    centre[0] = maxX; centre[1] = maxY;
    maxP[0] = maxX; maxP[1] = maxY; maxP[2] = (int)(max + 0.5);

}

void Life::centreIntegrall()
{
    DBF("void Life::centreIntegrall()");

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
        maxP[0] = maxX; maxP[1] = maxY; maxP[2] = (int)(max + 0.5);

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
    DBF("void Life::setBackground()");
    for(int i = 0; i < width; i++){
        for (int j = 0; j < height; j++) {
            background[i][j]=0;
        }
    }
    for (int k = 0; k < nBackground; k++)
    {
        emit stateSaveBCGR(double(k) / nBackground*100);
        //        qDebug() << "фон" << k;
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
    DBF("void Life::getFrame()");
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
    DBF("void Life::subtractBackground()");
    for(int i = 0; i < width; i++){
        for (int j = 0; j < height; j++) {
            frame[i][j] -= background[i][j];
        }
    }
}

void Life::diameter()
{
    DBF("void Life::diameter()");

    int maxX = 0;
    for (int i = 0; i < averageSections.x.size(); i++){
        if(averageSections.x[i] > maxX){
            maxX = averageSections.x[i];
        }
    }
    int levelX = maxX * sliceLevel / 100.;

    for (int i = 0; i < averageSections.x.size()-1; i++) {
        if(averageSections.x[i] <= levelX && averageSections.x[i+1] >= levelX) {
            diameterMas[0] = i;
            break;
        }
    }
    for (int i = averageSections.x.size()-1; i > 0; i--) {
        if(averageSections.x[i] <= levelX && averageSections.x[i-1]  >= levelX) {
            diameterMas[1] = i;
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
            diameterMas[2] = i;
            break;
        }
    }
    for (int i = averageSections.y.size()-1; i > 0; i--) {
        if(averageSections.y[i] <= levelY && averageSections.y[i-1]  >= levelY) {
            diameterMas[3] = i;
            break;
        }
    }
    diameterMas[4] = levelX;
    diameterMas[5] = levelY;
}

void Life::createAxis()
{
    DBF("void Life::createAxis()");
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
    DBF("void Life::getSections()");
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
    return  0;
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

int Life::getLevel(int &levelX, int &levelY) const
{
    levelX = diameterMas[4];
    levelY = diameterMas[5];
    return 1;
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
    return  1;
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
    return  1;
}

double Life::pixelTo_mm(int p)
{
    if(cam){
        return cam->getPSize_mkm()/1000.*p;
    }
    return  1;

}

bool Life::statusCam()
{
    return cam;
}

bool Life::statusLife()
{
    return !stop;
}

