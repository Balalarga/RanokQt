#ifndef SCREENSVIEW_H
#define SCREENSVIEW_H

#include <QTabWidget>
#include <QVector>
#include "Screens/ClearableWidget.h"

class ScreensView : public QTabWidget
{
    Q_OBJECT
public:
    explicit ScreensView(QWidget *parent = nullptr);


    bool ReadyToClose();


private slots:
    void TabChanged(int id);


private:
    QVector<ClearableWidget*> _tabs;
};

#endif // SCREENSVIEW_H
