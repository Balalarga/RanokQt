#ifndef MATRIXCALCULATOR_H
#define MATRIXCALCULATOR_H

#include "basecalculator.h"


class MatrixCalculator: public BaseCalculator
{
public:
    MatrixCalculator(const std::vector<int>& step);
    virtual const std::deque<VoxelData>& Calculate(Program& program, Zone zone) override;

private:
    std::vector<int> m_step;

};

#endif // MATRIXCALCULATOR_H
