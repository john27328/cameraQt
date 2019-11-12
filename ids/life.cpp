#include "life.h"
#include <unistd.h>

Life::Life(): frame(nullptr), frameFinal(nullptr) ,background(nullptr), pSectionX(nullptr),
    pSectionY(nullptr), pAxisX(nullptr), pAxisY(nullptr), cam(nullptr), isAverage(0)
{
    cam = nullptr;
    stop = 1;
    nBackground = 0;
    sBgnd = 0;
    methodCenter = MethodCenter::CentreIntegrall;
    methodDiameter = MethodDiameter::DiameterSlice;
    frame = nullptr;
    background = nullptr;
}

Life::~Life()
{
    stop = 1;

    delete  cam;
    sleep(0.100);
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

int Life::setMerhodDiameter(int i)
{
    switch (i) {
    case 0:
        methodDiameter = MethodDiameter::DiameterSlice;
        return 1;
        break;
    case 1:
        methodDiameter = MethodDiameter::diameterSecondMoments;
        return 1;
        break;
    default:
        break;
    }
}

void Life::initCamera(int c, QString &model, QString &serial)
{
    DBF("void Life::initCamera(int c, QString &model, QString &serial)");

        TestCam::initCum(&cam, model, serial);


    if(cam){
        range.width = cam->getWidth();
        range.height = cam->getHeight();
        pSectionX = new QVector<double>(range.width);
        pSectionY = new QVector<double>(range.height);
        pAxisX = new QVector<double>(range.width);
        pAxisY = new QVector<double>(range.height);
        createAxis();

        frame = new double*[range.width];
        frameFinal = new double*[range.width];
        background = new double*[range.width];
        averageFrame = new double*[range.width];
        for(int i = 0; i < range.width; i++)
        {
            frame[i] = new double[range.height];
            frameFinal[i] = new double[range.height];
            background[i] = new double[range.height];
            averageFrame[i] = new double[range.height];
            for (int j = 0; j < range.height; j++){
                frame[i][j] = 0;
                frameFinal[i][j] = 0;
                background[i][j] = 0;
            }
        }

    }
    else{
        stop = 1;
        sleep(0.100);
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
            for (int i = 0; i < range.width;i++) {
                delete[] frame[i];
            }
            delete[] frame;
            frame = nullptr;
        }
        if(frameFinal)
        {
            for (int i = 0; i < range.width;i++) {
                delete[] frameFinal[i];
            }
            delete[] frameFinal;
            frameFinal = nullptr;
        }
        if(background)
        {
            for (int i = 0; i < range.width;i++) {
                delete[] background[i];
            }
            delete[] background;
            background = nullptr;
        }
        if(averageFrame)
        {
            for (int i = 0; i < range.width;i++) {
                delete[] averageFrame[i];
            }
            delete[] averageFrame;
            averageFrame = nullptr;
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
        else {
            emit disconnectCam();
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
    //qDebug() << "averageState" << averageState;

}

void Life::averageFrameFunction()
{

}

void Life::summFrame(const double **frame1, const double **frame2, double **frameOut)
{
    for (int i = 0; i < range.width; i++)
        for(int j = 0; j < range.height; j++)
            frameOut[i][j] = frame1[i][j] + frame2[i][j];
}

void Life::minusFrame(const double **frame1, const double **frame2, double **frameOut)
{
    for (int i = 0; i < range.width; i++)
        for(int j = 0; j < range.height; j++)
            frameOut[i][j] = frame1[i][j] - frame2[i][j];
}

void Life::multFrame(const double **frame, double d, double **frameOut)
{
    for (int i = 0; i < range.width; i++)
        for(int j = 0; j < range.height; j++)
            frameOut[i][j] = frame[i][j] * d;
}

void Life::frameCopy(double **frame1, double **frameOut)
{
    for (int i = 0; i < range.width; i++) {
        for (int j = 0; j < range.height; j++) {
            frameOut[i][j] = frame1[i][j];
        }
    }
}

void Life::setCutLevel(int value)
{
    cutLevel = value;
}

int Life::getCutLevel() const
{
    return cutLevel;
}

void Life::setSliceLevel(double sliceLevel)
{
    DBF("void Life::setSliceLevel(double sliceLevel)");
    if (cam)
        this->sliceLevel = sliceLevel;
}


void Life::setBackground()
{
    DBF("void Life::setBackground()");
    for(int i = 0; i < range.width; i++){
        for (int j = 0; j < range.height; j++) {
            background[i][j]=0;
        }
    }
    for (int k = 0; k < nBackground; k++)
    {
        emit stateSaveBCGR(double(k) / nBackground*100);
        //        qDebug() << "фон" << k;
        getFrame();
        for(int i = 0; i < range.width; i++){
            for (int j = 0; j < range.height; j++) {
                background[i][j]+=frame[i][j];
            }
        }
    }
    for(int i = 0; i < range.width; i++){
        for (int j = 0; j < range.height; j++) {
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
            centerMax();
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
            lookForDiamter(methodDiameter);
            //        qDebug() << "центр" << x << y;
            //qDebug() << "frame";
            frameCopy(frame, frameFinal);
            emit updateFrame();
        }
    }

}

void Life::subtractBackground()
{
    DBF("void Life::subtractBackground()");
    for(int i = 0; i < range.width; i++){
        for (int j = 0; j < range.height; j++) {
            frame[i][j] -= background[i][j];
        }
    }
}

void Life::createAxis()
{
    DBF("void Life::createAxis()");
    if (cam){
        for (int i = 0; i < range.width; i++) {
            (*pAxisX)[i] = (i + 0.5) * cam->getPSize_mkm() / 1000.;
        }
        for (int i = 0; i < range.height; i++) {
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
    x = center.x;
    y = center.y;
}

void Life::getSections()
{
    DBF("void Life::getSections()");
    int x, y;
    getCentre(x,y);
    for (int i = 0; i < range.width; i++) {
        (*pSectionX)[i] = frame[i][y];
    }
    for (int j = 0; j < range.height; j++) {
        (*pSectionY)[j] = frame[x][j];
    }
}

void Life::getMax(int &x, int &y, int &z) const
{
    x = maxPower.x;
    y = maxPower.y;
    z = maxPower.power;
}

int Life::getBits() const
{
    if(cam){
        return cam->getBits();
    }
    return  0;
}

int Life::getEdge(int &x1, int &x2, int &y1, int &y2) const
{
    if(edge.x1 >= 0 &&
            edge.x2 >= 0 &&
            edge.y1 >= 0 &&
            edge.y2 >= 0){
        x1 = edge.x1;
        x2 = edge.x2;
        y1 = edge.y1;
        y2 = edge.y2;
        return 1;
    }
    else {
        return 0;
    }


}

int Life::getLevel(int &levelX, int &levelY) const
{
    levelX = edge.levelX;
    levelY = edge.levelY;
    return 1;
}

int Life::getDiameter(int &dx, int &dy) const
{
    dx = diameter.width;
    dy = diameter.height;
    return 1;
}

int Life::getDiameter_mm(double &dx, double &dy) const
{
    dx = pixelTo_mm(diameter.width);
    dy = pixelTo_mm(diameter.height);
    return 1;
}

int Life::getDiameterSigma(double &d, double &dx, double &dy, double &dBig, double &dSmall, double phi)
{
    d = diameterSigma.d;
    dx = diameterSigma.dx;
    dy = diameterSigma.dy;
    dBig = diameterSigma.dSigmaBig;
    dSmall = diameterSigma.dSigmaSmall;
    phi = diameterSigma.phi;
    return 1;
}

int Life::getDiameterSigma_mm(double &d, double &dx, double &dy, double &dBig, double &dSmall, double phi)
{
    d = pixelTo_mm(diameterSigma.d);
    dx = pixelTo_mm(diameterSigma.dx);
    dy = pixelTo_mm(diameterSigma.dy);
    dBig = pixelTo_mm(diameterSigma.dSigmaBig);
    dSmall = pixelTo_mm(diameterSigma.dSigmaSmall);
    phi = pixelTo_mm(diameterSigma.phi);
    return 1;
}

int Life::getWidth() const
{
    return range.width;
}

int Life::getWidth_mm() const
{
    if(cam){
        return range.width * cam->getPSize_mkm() / 1000.;
    }
    return  1;
}

int Life::getHeight() const
{
    return range.height;
}

int Life::getHeight_mm() const
{
    if(cam){
        return range.height * cam->getPSize_mkm() / 1000.;
    }
    return  1;
}

double Life::pixelTo_mm(double p) const
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

