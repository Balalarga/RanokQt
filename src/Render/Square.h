#ifndef SQUARE_H
#define SQUARE_H

#include "IDrawableObject.h"

// 2D квадрат
class Square: public IDrawableObject
{
public:
    Square(sf::Vector3<double> position, sf::Vector3<double> size, sf::Color color);
    void Draw() override;

private:
    sf::Vector3<double> verticies[4];
};

#endif // SQUARE_H
