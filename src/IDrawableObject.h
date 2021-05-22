#ifndef IDRAWABLEOBJECT_H
#define IDRAWABLEOBJECT_H

#include "Color.h"

class IDrawableObject
{
public:
    IDrawableObject(Color color):
        color(color)
    {}
    virtual ~IDrawableObject(){};
    virtual void Draw() = 0;

protected:
    Color color;
};

#endif // IDRAWABLEOBJECT_H
