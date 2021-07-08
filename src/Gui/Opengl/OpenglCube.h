#ifndef OPENGLCUBE_H
#define OPENGLCUBE_H

#include <QObject>
#include "OpenglObject.h"

class OpenglCube : public OpenglObject
{
    Q_OBJECT
public:
    explicit OpenglCube(Vector3f pos, Vector3f size, QColor color, QObject *parent = nullptr);

    void Render();

private:
    Vector3f verticies[24];
    Vector3f normals[6]{
        {0, 0, 1},
        {0, 0, -1},
        {1, 0, 0},
        {-1, 0, 0},
        {0, 1, 0},
        {0, -1, 0},
    };
};

#endif // OPENGLCUBE_H
