#ifndef IMAGERECURSIVECALCULATOR_H
#define IMAGERECURSIVECALCULATOR_H

#include "MImageCalculator.h"

// Матричный калькулятор образов, работает по аналогии с MatrixCalculator, но для образов
class MImageMatrixCalculator: public MImageCalculator
{
public:
    MImageMatrixCalculator(Vector3i step, QObject* parent = nullptr);


public slots:
    virtual void Calculate() override;


protected:
    Vector3i m_step;


   void Calculate1d();
   void Calculate2d();
   void Calculate3d();

};

#endif // IMAGERECURSIVECALCULATOR_H
