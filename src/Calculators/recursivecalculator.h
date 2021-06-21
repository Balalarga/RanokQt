#ifndef RECURCIVECALCULATOR_H
#define RECURCIVECALCULATOR_H

#include "modelcalculator.h"


class RecursiveCalculator: public ModelCalculator
{
public:
    RecursiveCalculator(int depth, QObject *parent = nullptr);


public slots:
    virtual void Calculate() override;


protected:
    int m_depth;


    void Calculate1d(Vector3f coords, Vector3f size, int step);
    void Calculate2d(Vector3f coords, Vector3f size, int step);
    void Calculate3d(Vector3f coords, Vector3f size, int step);
};

#endif // RECURCIVECALCULATOR_H
