#include <QApplication>

#include "AppWindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AppWindow w;
    w.resize(1280, 720);

    qApp->installEventFilter(&w);

    w.show();
    return a.exec();
}
