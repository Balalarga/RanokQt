#include "AppWindow.h"

#include <QDockWidget>
#include <QFileDialog>
#include <QKeyEvent>
#include <QCloseEvent>
#include "Gui/StyleLoader.h"


AppWindow::AppWindow(QWidget *parent):
    QMainWindow(parent),
    _screensContainer(new ScreensView(this))
{
    StyleLoader::attach("assets/styles/NewStyle.qss");
    setCentralWidget(_screensContainer);
}

void AppWindow::closeEvent(QCloseEvent *event)
{
    if (_screensContainer->ReadyToClose())
        event->accept();
    else
        event->ignore();
}
