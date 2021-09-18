#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QMainWindow>
#include "ScreensView.h"

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow(QWidget *parent = nullptr);


protected:
    bool eventFilter(QObject *obj, QEvent *event) override;


private slots:


private:
    ScreensView* _screensContainer;
};
#endif // WIDGET_H
