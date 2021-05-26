#ifndef MATRIXCALCULATOR_H
#define MATRIXCALCULATOR_H

#include "BaseCalculator.h"

// Матричный калькулятор: делит пространство исследования на части
class MatrixCalculator: public BaseCalculator
{
public:
    MatrixCalculator(sf::Vector3i step);
    const std::deque<VoxelData>& Calculate(Program& program, Zone zone, std::function<void(VoxelData&)> iterFunc = nullptr) override;

protected:
    sf::Vector3i m_step;

    // Функции для 1D, 2D, 3D
    const std::deque<VoxelData>& matrix1(Program& program, Zone zone, std::function<void(VoxelData&)> iterFunc);
    const std::deque<VoxelData>& matrix2(Program& program, Zone zone, std::function<void(VoxelData&)> iterFunc);
    const std::deque<VoxelData>& matrix3(Program& program, Zone zone, std::function<void(VoxelData&)> iterFunc);

};

#endif // MATRIXCALCULATOR_H
