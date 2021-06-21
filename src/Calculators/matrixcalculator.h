#ifndef MATRIXCALCULATOR_H
#define MATRIXCALCULATOR_H

#include "modelcalculator.h"



class MatrixCalculator: public ModelCalculator
{
    Q_OBJECT
public:
    MatrixCalculator(Vector3i step, QObject* parent = nullptr);


public slots:
    void Calculate() override;


protected:
    Vector3i m_step;

    void Calculate1d();
    void Calculate2d();
    void Calculate3d();
};

#endif // MATRIXCALCULATOR_H
