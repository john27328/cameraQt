#ifndef DEBUG_H
#define DEBUG_H
#include <QDebug>
#include <QString>
#define DBG(x) qDebug()<< (x);
#define DV(x) #x << (x)
#define DBF(x) //Debug dbg(x);


class Debug
{
public:
    Debug(QString name)
    {
        nameD = name;
        n++;
        QString tmp;
        for (int i = 0; i < n; i++) tmp += "-";
        qDebug()<< tmp << nameD << "start";
    }
    ~Debug()
    {
        n--;
        QString tmp;
        for (int i = 0; i < n; i++) tmp += "-";
        qDebug()<< tmp << nameD << "end";
    }private:
    static int n;
    QString nameD;
};


#endif // DEBUG_H

