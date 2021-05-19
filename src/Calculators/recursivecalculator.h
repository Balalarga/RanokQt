#ifndef RECURCIVECALCULATOR_H
#define RECURCIVECALCULATOR_H

#include "basecalculator.h"

class RecursiveCalculator: public BaseCalculator
{
public:
    RecursiveCalculator(int depth);

    virtual const std::deque<VoxelData>& Calculate(Program& program, Zone zone) override;

protected:
    int m_depth;
    Zone m_zone;
    Program* m_program = nullptr;

    void recursionFunc(glm::vec3 coords, double size, int step);

};

#endif // RECURCIVECALCULATOR_H
