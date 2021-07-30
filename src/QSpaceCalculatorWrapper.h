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
        CommonCalculator([this](CalculatorMode m, int bs, int s, int e){ EmitCompute(m, bs, s, e); })
    {

    }
    

signals:
    void Computed(CalculatorMode mode, int batchStart, int start, int end);


protected:
    void EmitCompute(CalculatorMode mode, int batchStart, int start, int end)
    {
        emit Computed(mode, batchStart, start, end);
    }
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
        OpenclCalculator([this](CalculatorMode m, int bs, int s, int e){ EmitCompute(m, bs, s, e); })
    {

    }
    

signals:
void Computed(CalculatorMode mode, int batchStart, int start, int end);


protected:
    void EmitCompute(CalculatorMode mode, int batchStart, int start, int end)
    {
        emit Computed(mode, batchStart, start, end);
    }
    void run() override
    {
        Run();
    }

};

#endif // QSPACECALCULATORWRAPPER_H
