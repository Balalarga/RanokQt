#ifndef MATRIXCALCULATOR_H
#define MATRIXCALCULATOR_H

#include "basecalculator.h"


class MatrixCalculator: public BaseCalculator
{
public:
    MatrixCalculator(glm::vec3 step);
    virtual const std::deque<VoxelData>& Calculate(Program& program, Zone zone) override;

private:
    glm::vec3 m_step;

};

#endif // MATRIXCALCULATOR_H
