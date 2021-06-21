#ifndef OPENGLSQUARE_H
#define OPENGLSQUARE_H

#include "openglobject.h"

class OpenglSquare: public OpenglObject
{
public:
    explicit OpenglSquare(Vector2f pos, Vector2f size, QColor color, QObject *parent = nullptr);
    explicit OpenglSquare(Vector3f pos, Vector3f size, QColor color, QObject *parent = nullptr);

    void Render();

private:
    Vector3f verticies[4];
};

#endif // OPENGLSQUARE_H
