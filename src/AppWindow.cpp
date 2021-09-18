#include "AppWindow.h"

#include <QDockWidget>
#include <QFileDialog>
#include <QKeyEvent>
#include "Gui/StyleLoader.h"


AppWindow::AppWindow(QWidget *parent):
    QMainWindow(parent),
    _screensContainer(new ScreensView(this))
{
    StyleLoader::attach("../assets/styles/ManjaroMix.qss");

    setCentralWidget(_screensContainer);

//    Qt::WindowFlags flags;
//    flags = Qt::FramelessWindowHint;
//    setWindowFlags(flags);
}

bool AppWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(QKeySequence("F6") == QKeySequence(keyEvent->key()))
        {
            QString fileName = QFileDialog::getOpenFileName(this,
                                                            tr("Stylesheet"), "../style",
                                                            tr("Stylesheet(*.qss)"));
            if(!fileName.isEmpty())
                StyleLoader::attach(fileName);
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}
