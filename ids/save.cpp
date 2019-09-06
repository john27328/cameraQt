#include "widget.h"
#include "ui_widget.h"

void Widget::screenShot()
{
    //QString time = QDateTime::currentDateTime().toString("yy.MM.dd-hh.mm.ss.zzz");
    QString time = QDateTime::currentDateTime().toString("yyMMdd-hhmmsszzz/");
    QString folder = QDir::homePath() +"/camera/" + time;
    if(!QDir(folder).exists())
    {
        qDebug() << QDir().mkdir(folder);
    }
    saveScreen(folder);
    saveData(folder);

}


void Widget::saveScreen(QString name)
{
    QString fileName(name + "screen.png");
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << file.errorString();
    } else {
        ui->colorMap->savePng(fileName,0,0,2);
    }

    QString fileNameX(name + "sectionX.png");
    QFile fileX(fileNameX);

    if (!fileX.open(QIODevice::WriteOnly))
    {
        qDebug() << fileX.errorString();
    } else {
        ui->sectionX->savePng(fileNameX, 1000, 500);
    }

    QString fileNameY(name + "sectionY.png");
    QFile fileY(fileNameY);

    if (!fileY.open(QIODevice::WriteOnly))
    {
        qDebug() << fileY.errorString();
    } else {
        ui->sectionY->savePng(fileNameY, 1000, 500);
    }
}

void Widget::saveData(QString name)
{
    QString fileNameX(name + "sectionX.txt");
    QFile fileX(fileNameX);

    if (fileX.open(QFile::WriteOnly))
    {
        QTextStream strm(&fileX);
        strm << "x\tI\n";
        for (int i = 0; i < life->getWidth(); i++) {
            double x = (*life->pAxisX)[i];
            double y = life->averageSections.x[i];
            strm << x <<"\t" << y << "\n";
        }
        fileX.close();
    }

    QString fileNameY(name + "sectionY.txt");
    QFile fileY(fileNameY);

    if (fileY.open(QFile::WriteOnly))
    {
        QTextStream strm(&fileY);
        strm << "x\tI\n";
        for (int i = 0; i < life->getWidth(); i++) {
            double x = (*life->pAxisX)[i];
            double y = life->averageSections.x[i];
            strm << x <<"\t" << y << "\n";
        }
        fileY.close();
    }

}
