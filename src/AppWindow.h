#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QMainWindow>
#include "ScreensView.h"

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow(QWidget *parent = nullptr);


private:
    ScreensView* _screensContainer;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // WIDGET_H
