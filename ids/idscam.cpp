#include "idscam.h"

#include <QString>

IdsCam::IdsCam(int ID)
{
    int nRet;
    hCam = ID;
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

    bits = 12;

}

IdsCam::~IdsCam()
{
    qDebug() << thisCam;
    qDebug()<<"~IdsCam";

    //qDebug() << DBG(*thisCam);
    if((*thisCam)){
        qDebug() << "!";
        (*thisCam) = 0;
        qDebug() << "остановка потока";
        stopLive();
        isOK = 0;
        qDebug() << "освобождение камеры";
        is_FreeImageMem(hCam, ppcImgMem, memID);
        is_ExitCamera(hCam);
        qDebug() << "камера отключена";
    }
}

QString IdsCam::getModel()
{
    return sInfo.strSensorName;
}

QString IdsCam::getSerial()
{
    return cInfo.SerNo;
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

int IdsCam::setExp(double &exp)
{
    //E, (void*) &parameter, sizeof(parameter));
    int nRet = is_Exposure(hCam,
                           IS_EXPOSURE_CMD_SET_EXPOSURE,
                           (void *)&exp,
                           sizeof(exp));
    if (nRet == IS_SUCCESS) {
        return 1;
    }
    else {
        return 0;
    }
}

int IdsCam::getFPS(double &fps)
{
    int nRet = is_GetFramesPerSecond(hCam, &fps);
    qDebug()<<"getFPS" << nRet << fps;
    if (nRet == IS_SUCCESS) {
        return 1;
    }
    else {
        return 0;
    }
}

int IdsCam::getExp(double &exp)
{
    int nRet = is_Exposure(hCam,
                           IS_EXPOSURE_CMD_GET_EXPOSURE,
                           (void *)&exp,
                           sizeof(exp));
    qDebug()<<"getExp" << nRet << exp<< sizeof(exp);
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
    qDebug()<<"getRangeFPS" << nRet << 1 / maxT << 1 / minT;

    if (nRet == IS_SUCCESS) {
        minFPS = 1 / maxT;
        maxFPS = 1 / minT;
        return 1;
    }
    else {
        return 0;
    }
}

int IdsCam::getRangeExp(double &minExp, double &maxExp)
{
    double range[3];
    int nRet = is_Exposure(hCam,
                           IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE,
                           (void *)range,
                           sizeof(range));
    qDebug()<<"getRangeExp" << nRet <<sizeof(range) << range[0] << range[1];

    if (nRet == IS_SUCCESS) {
        minExp = range[0];
        maxExp = range[1];
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
    if(isLife){
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
    return 0;
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
        for (int i = 0; i < width; i++){
            for (int j = 0; j < height; j++) {
                frame[i][j] = array[i + j * width];
            }
        }
    return 1;
    }
    return 0;
}

bool IdsCam::statusCam()
{
    return isOK;
}

bool IdsCam::statusLife()
{
    return isLife;
}
