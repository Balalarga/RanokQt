#include <QApplication>

#include "Gui/widget.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.resize(1280, 600);

    qApp->installEventFilter(&w);

    w.show();
    return a.exec();
}
