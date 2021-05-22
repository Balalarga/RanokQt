#ifndef RECURCIVECALCULATOR_H
#define RECURCIVECALCULATOR_H

#include "basecalculator.h"

class RecursiveCalculator: public BaseCalculator
{
public:
    RecursiveCalculator(int depth);

    virtual const std::deque<VoxelData>& Calculate(Program& program, Zone zone, std::function<void(VoxelData&)> = nullptr) override;

protected:
    int m_depth;
    int stepsAmount;
    int stepsCounter = 0;
    Zone m_zone;
    Program* m_program = nullptr;

    void recursionFunc1(Vector3 coords, Vector3 size, int step, std::function<void (VoxelData &)> iterFunc);
    void recursionFunc2(Vector3 coords, Vector3 size, int step, std::function<void (VoxelData &)> iterFunc);
    void recursionFunc3(Vector3 coords, Vector3 size, int step, std::function<void (VoxelData &)> iterFunc);

};

#endif // RECURCIVECALCULATOR_H
