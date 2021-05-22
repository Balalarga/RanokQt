#ifndef SQUARE_H
#define SQUARE_H

#include "Vectors.h"
#include "Color.h"
#include <GLEW/glew.h>
#include "IDrawableObject.h"

class Square: public IDrawableObject
{
public:
    Square(Vector3 position, Vector3 size, Color color);
    void Draw() override;

private:
    Vector3 verticies[4];
};

#endif // SQUARE_H
