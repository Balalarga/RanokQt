#include <QApplication>
#include <clocale>
#include "Space/Calculators/OpenclCalculator.h"
#include "AppWindow.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    setlocale(LC_NUMERIC, "C");

    static_assert (sizeof(cl_double) == sizeof(double), "Type size error cl_double != double");

    AppWindow w;
    w.resize(1280, 720);

    qApp->installEventFilter(&w);

    w.show();
    return a.exec();
    return a.exec();
}
