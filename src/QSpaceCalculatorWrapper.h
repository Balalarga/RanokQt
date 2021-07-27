#ifndef QSPACECALCULATORWRAPPER_H
#define QSPACECALCULATORWRAPPER_H

#include <QThread>
#include "SpaceCalculators.h"

class QSpaceCalculatorWrapper: public QThread
{
    Q_OBJECT
public:
    QSpaceCalculatorWrapper(ISpaceCalculator* calculator, QObject* parent):
        QThread(parent),
        _calculator(calculator)
    {

    }

    ~QSpaceCalculatorWrapper()
    {
        if(_calculator)
            delete _calculator;
    }

    ISpaceCalculator* Get()
    {
        return _calculator;
    }

protected:
    void run()
    {
        _calculator->Run();
    }

    ISpaceCalculator* _calculator;
};

#endif // QSPACECALCULATORWRAPPER_H
