#ifndef RECURCIVECALCULATOR_H
#define RECURCIVECALCULATOR_H

#include "BaseCalculator.h"

// Рекурсивный калькулятор: с повышением уровня рекурсии делит пространство на все более маленькие подпространства
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

    // Рекурсивные функции для 1D, 2D, 3D моделей
    void recursionFunc1(sf::Vector3<double> coords, sf::Vector3<double> size, int step, std::function<void (VoxelData &)> iterFunc);
    void recursionFunc2(sf::Vector3<double> coords, sf::Vector3<double> size, int step, std::function<void (VoxelData &)> iterFunc);
    void recursionFunc3(sf::Vector3<double> coords, sf::Vector3<double> size, int step, std::function<void (VoxelData &)> iterFunc);

};

#endif // RECURCIVECALCULATOR_H
