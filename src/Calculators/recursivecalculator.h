#ifndef RECURCIVECALCULATOR_H
#define RECURCIVECALCULATOR_H

#include "basecalculator.h"

class RecursiveCalculator: public BaseCalculator
{
public:
    RecursiveCalculator(int depth);

    virtual std::deque<VoxelData> Calculate(Program& program, Zone zone) override;

protected:
    int m_depth;
    Program* m_program = nullptr;
    std::deque<VoxelData>* m_result = nullptr;
    Zone m_zone;

    void recursionFunc(glm::vec3 coords, glm::vec3 size, int step);
};

#endif // RECURCIVECALCULATOR_H
