#ifndef CLEARABLEWIDGET_H
#define CLEARABLEWIDGET_H

#include <QWidget>

class ClearableWidget: public QWidget
{
    Q_OBJECT
public:
    ClearableWidget(QWidget* parent = nullptr):
        QWidget(parent)
    {
    }

public slots:
    virtual void Cleanup() = 0;
};

#endif // CLEARABLEWIDGET_H
