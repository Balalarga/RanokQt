#ifndef IDRAWABLEOBJECT_H
#define IDRAWABLEOBJECT_H

#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <GLEW/glew.h>

class IDrawableObject
{
public:
    IDrawableObject(sf::Color color):
        color(color)
    {}
    virtual ~IDrawableObject(){};
    virtual void Draw() = 0;

protected:
    sf::Color color;
};

#endif // IDRAWABLEOBJECT_H
