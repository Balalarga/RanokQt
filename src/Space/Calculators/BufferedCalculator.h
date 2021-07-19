#ifndef BUFFEREDCALCULATOR_H
#define BUFFEREDCALCULATOR_H

#include "ISpaceCalculator.h"

class BufferedCalculator : public ISpaceCalculator
{
public:
    explicit BufferedCalculator(QObject *parent = nullptr);

    void CalcModel();
    void CalcMImage();


protected:


};

#endif // BUFFEREDCALCULATOR_H
