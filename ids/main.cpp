#include "widget.h"
#include <QApplication>
int Debug::n = 0;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
