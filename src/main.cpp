#include <QApplication>

#include "Gui/Widget.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.resize(1280, 800);

    qApp->installEventFilter(&w);

    w.show();
    return a.exec();
}
