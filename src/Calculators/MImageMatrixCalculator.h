#ifndef IMAGERECURSIVECALCULATOR_H
#define IMAGERECURSIVECALCULATOR_H

#include "MImageCalculator.h"
#include <SFML/System/Vector3.hpp>

class MImageMatrixCalculator: public MImageCalculator
{
public:
    MImageMatrixCalculator(sf::Vector3i step);
    virtual const std::deque<MImageData>& Calculate(Program& program, ImageType type, std::function<void(MImageData&)> iterFunc = nullptr);

protected:
    sf::Vector3i m_step;

    const std::deque<MImageData>& matrix1(Program& program, ImageType type, std::function<void(MImageData&)> iterFunc);
    const std::deque<MImageData>& matrix2(Program& program, ImageType type, std::function<void(MImageData&)> iterFunc);
    const std::deque<MImageData>& matrix3(Program& program, ImageType type, std::function<void(MImageData&)> iterFunc);

};

#endif // IMAGERECURSIVECALCULATOR_H
