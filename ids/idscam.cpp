#include "idscam.h"

#include <QString>

IdsCam::IdsCam()
{
    int nRet;
    isOK = 0;
    nRet = is_InitCamera(&hCam, 0); // подключение камеры hCam = 0 - первая доступная камера
    if (nRet == IS_SUCCESS) {
        isOK = 1;
        qDebug()<<"Камера подключена. ID = " << hCam;
    }
    else {
        qDebug()<<"ошибка инициализации";
    }

    nRet = is_GetSensorInfo(hCam, &sInfo); // информация о матрице
    if(nRet != IS_SUCCESS) {
        qDebug() << "информация о матрице не прочитана";
    }
    else {
        qDebug() << "Модель сенсора: " << sInfo.strSensorName;
        pSize_mkm = sInfo.wPixelSize / 100.;
        qDebug() << "Размер пикселя: " << pSize_mkm;

    }

    nRet = is_GetCameraInfo(hCam,&cInfo); // информация о камере
    if(nRet != IS_SUCCESS) {
        qDebug() << "информация о камере не прочитана";
    }
    else {
        qDebug() << "Серийный номер: " << cInfo.SerNo;

    }


    nRet = is_ResetToDefault(hCam); // сброс настроек на стандартные
    if(nRet != IS_SUCCESS) {
        qDebug() << "параметры не сброшены";
    }


    nRet = is_SetDisplayMode(hCam,IS_SET_DM_DIB); // режим сбора данных в масссив
    if(nRet != IS_SUCCESS) {
        qDebug() << "ошибка задания режима камеры";
    }


    nRet = is_AOI(hCam, IS_AOI_IMAGE_GET_AOI, (void*)&rectAOI, sizeof(rectAOI)); //размеры и положение захватываемого кадра
    if(nRet == IS_SUCCESS) {
//        int x     = rectAOI.s32X;
//        int y     = rectAOI.s32Y;
        width = rectAOI.s32Width;
        height = rectAOI.s32Height;
        qDebug () << "разрешение матрицы: " << width << " x " << height;
    }
    else {
        qDebug() <<"не удалось прочитать разрешение матрицы";
    }


    nRet = is_AllocImageMem(hCam, width, height, 16, &ppcImgMem, &memID);
    if (nRet == IS_SUCCESS){
        nRet = is_SetImageMem(hCam, ppcImgMem, memID);
        if(nRet == IS_SUCCESS){
            is_SetColorMode(hCam, IS_CM_MONO12);
        }
        else {
            qDebug() << "ошибка выделения памяти";
        }
    }
    else {
        qDebug() << "ошибка выделения памяти";
    }

    double fps = 2;
    setFPS(fps);

}


int IdsCam::initCum(Cam **cam)
{
    qDebug() << DBG(*cam);
    if(!(*cam)){
        qDebug() << "init cam";
        *cam = new IdsCam;
        (*cam)->thisCam = cam;
        if((*cam)->statusCam()){
            return 1;
        }
        else {
            delete *cam;
            *cam = 0;
            return 0;
        }
    }
    else {
        qDebug() << "delete cam";
        delete (*cam);
        *cam = 0;
        return 0;
    }
}

IdsCam::~IdsCam()
{
    qDebug() << DBG(*thisCam);
    if((*thisCam)){
        *thisCam = 0;
        stopLive();
        isOK = 0;
        is_FreeImageMem(hCam, ppcImgMem, memID);
        is_ExitCamera(hCam);
        qDebug() << "камера отключена";
    }
}

int IdsCam::setFPS(double &fps)
{
    int nRet = is_SetFrameRate(hCam,fps,&fps);
    if (nRet == IS_SUCCESS) {
        return 1;
    }
    else {
        return 0;
    }
}

int IdsCam::getRangeFPS(double &minFPS, double &maxFPS)
{
    double minT, maxT, intervallT;
    int nRet = is_GetFrameTimeRange(hCam, &minT, &maxT, &intervallT);
    if (nRet == IS_SUCCESS) {
        minFPS = 1 / maxT;
        maxFPS = 1 / minT;
        return 1;
    }
    else {
        return 0;
    }
}

int IdsCam::startLive()
{
    int nRet = is_CaptureVideo(hCam, IS_DONT_WAIT); // свободный режим
    if(nRet != IS_SUCCESS) {
        qDebug() << "ошибка захвата изображения";
        return 0;
    }
    else {
        isLife = 1;
        hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        is_InitEvent(hCam, hEvent, IS_SET_EVENT_FRAME);
        is_EnableEvent(hCam, IS_SET_EVENT_FRAME);
        is_FreezeVideo(hCam, IS_DONT_WAIT);
        return 1;
    }


}

int IdsCam::stopLive()
{
    int nRet = is_StopLiveVideo(hCam,IS_FORCE_VIDEO_STOP);
    if(nRet != IS_SUCCESS) {
        qDebug() << "ошибка остановки захата";
        return 0;
    }
    is_DisableEvent(hCam, IS_SET_EVENT_FRAME);
    is_ExitEvent(hCam, IS_SET_EVENT_FRAME);
    CloseHandle(hEvent);
    isLife = 0;
    return 1;

}

int IdsCam::getFrame(float **frame)
{
    // захват с ожиданием
    DWORD dwRet = WaitForSingleObject(hEvent, 1000);
    if (dwRet == WAIT_TIMEOUT) {
        /* wait timed out */
        qDebug() <<"время ожидание истекло";
        stopLive();
        return 0;
    }
    else if (dwRet == WAIT_OBJECT_0) {
        /* event signaled */
        short *array = (short *)ppcImgMem;

//        //отладка
//        for (int i = 0; i < width; i+=100){
//            QString str;
//            for (int j = 0; j < height; j+=100) {
//                short tmp = (array[i + j * width]);
//                str+= QString::number(tmp) + " ";
//            }
//            qDebug() << str;
//        }
//        qDebug() << "========================";


        for (int i = 0; i < width; i++){
            for (int j = 0; j < height; j++) {
                frame[i][j] = array[i + j * width];
            }
        }
    return 1;
    }
}

bool IdsCam::statusCam()
{
    return isOK;
}

bool IdsCam::statusLife()
{
    return isLife;
}
