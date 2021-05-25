#ifndef MImageCalculator_H
#define MImageCalculator_H

#include <deque>
#include <vector>
#include <functional>

#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>
#include "Language/Program.h"


enum class ImageType
{
    Cx, Cy, Cz, Cw, Ct
};

struct MImageData
{
    MImageData(){};
    MImageData(const sf::Vector3<double>& center, const sf::Vector3<double>& size,
              const sf::Color& color, double value,
              int dementions = 3):
        center(center), size(size), color(color), value(value),
        dementions(dementions)
    {}
    sf::Vector3<double> center;
    sf::Vector3<double> size;
    sf::Color color;
    double value;
    int dementions;
};

class MImageCalculator
{
public:
    MImageCalculator(){m_results = new std::deque<MImageData>();}
    virtual ~MImageCalculator(){delete m_results;}
    virtual const std::deque<MImageData>& Calculate(Program& program, ImageType type, std::function<void(MImageData&)> iterFunc = nullptr) = 0;

protected:
    std::deque<MImageData>* m_results;
};

#endif // MImageCalculator_H
