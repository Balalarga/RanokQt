#ifndef QSPACECALCULATORWRAPPER_H
#define QSPACECALCULATORWRAPPER_H

#include <QThread>
#include "SpaceCalculators.h"


class CommonCalculatorThread: public QThread, public CommonCalculator
{
    Q_OBJECT
public:
    CommonCalculatorThread(QObject* parent):
        QThread(parent),
        CommonCalculator([this](CalculatorMode m, int s, int e){ emit Computed(m, s, e); })
    {

    }
    

signals:
    void Computed(CalculatorMode mode, int start, int end);


protected:
    void run() override
    {
        Run();
    }
};



class OpenclCalculatorThread: public QThread, public OpenclCalculator
{
    Q_OBJECT
public:
    OpenclCalculatorThread(QObject* parent):
        QThread(parent),
        OpenclCalculator([this](CalculatorMode m, int s, int e){ emit Computed(m, s, e); })
    {

    }
    

signals:
    void Computed(CalculatorMode mode, int start, int end);


protected:
    void run() override
    {
        Run();
    }

};

#endif // QSPACECALCULATORWRAPPER_H
