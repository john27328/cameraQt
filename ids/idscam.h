#ifndef IDSCAM_H
#define IDSCAM_H

#include "ueye.h"
#include "cam.h"
#include <QInputDialog>
#include "debug.h"
#include <QMap>

class IdsCam: public Cam
{
public:
    static int initCum(Cam** cam, QString &model, QString &serial)
    {
int ID = 0;
        if(!(*cam)){
            qDebug() << "init cam";
            if (ID = getCameraList()){
                *cam = new IdsCam(ID);
                (*cam)->thisCam = cam;
                if((*cam)->statusCam()){
                    model = (*cam)->getModel();
                    serial = (*cam)->getSerial();
                    return 1;
                }
                else {
                    return 0;
                }
            }
            else {
                delete *cam;
                *cam = 0;
                model = "error";
                serial = "----";
                return 0;
            }
        }
        else {
            qDebug() << "delete cam";
            delete (*cam);
            *cam = 0;
            model = "----";
            serial = "----";
            return 0;
        }
    }

    static int getCameraList()
    {
        Debug dbg("static int getCameraList()");
        int ID = 0;
        // At least one camera must be available
        INT nNumCam;
        if( is_GetNumberOfCameras( &nNumCam ) == IS_SUCCESS) {
            if( nNumCam >= 1 ) {
                // Create new list with suitable size
                UEYE_CAMERA_LIST* pucl;
                pucl = (UEYE_CAMERA_LIST*) new BYTE [sizeof (DWORD) + nNumCam * sizeof (UEYE_CAMERA_INFO)];
                pucl->dwCount = nNumCam;

                //Retrieve camera info
                if (is_GetCameraList(pucl) == IS_SUCCESS) {
                    QStringList items;
                    QMap<QString, int> map;
                    for (int iCamera = 0; iCamera < (int)pucl->dwCount; iCamera++) {
                        qDebug() << "Camera" << iCamera << pucl->uci[iCamera].dwCameraID
                                 << pucl->uci[iCamera].Model << pucl->uci[iCamera].SerNo;
                        QString tmp = QString::number(iCamera)
                                + QString::fromUtf8(" IDS ")
                                + QString(pucl->uci[iCamera].Model)
                                + QString::fromUtf8(" №")
                                + QString(pucl->uci[iCamera].SerNo)
                                + QString::fromUtf8(" ID")
                                + QString((int)pucl->uci[iCamera].dwCameraID);
                        items << tmp;
                        map[tmp] = pucl->uci[iCamera].dwCameraID;

                    }
                    bool ok;
                    QString item = QInputDialog::getItem(0,
                                             QString::fromUtf8("Выберите камеру"),
                                             QString::fromUtf8("камера:"), items, 1, false, &ok);
                    if (ok && !item.isEmpty()){
                        ID = map[item];
                    }

                }
                delete [] pucl;
            }
        }
        return  ID;
    }
    ~IdsCam();
    QString getModel();
    QString getSerial();
    int setFPS(double &fps);
    int setExp(double &exp);
    int getFPS(double &fps);
    int getExp(double &exp);
    int getRangeFPS(double &minFPS, double &maxFPS);
    int getRangeExp(double &minExp, double &maxExp);
    int startLive();
    int stopLive();
    int getFrame(float **frame);
    bool statusCam(); // - 1 - ok
    bool statusLife(); // - 1 - ok

private:
    IdsCam(int ID);
    HIDS hCam = 0;      // 0 for the next available camera. 1-254 to access by ID
    SENSORINFO sInfo;
    CAMINFO cInfo;
    HWND hWndDisplay;
    IS_RECT rectAOI;
    char* ppcImgMem; // кадр
    int memID;
    int pitch;
    HANDLE hEvent;

};

#endif // IDSCAM_H
