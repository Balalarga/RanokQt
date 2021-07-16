#ifndef OPENGLCUBE_H
#define OPENGLCUBE_H

#include <QObject>
#include "OpenglObject.h"
#include <CL/cl.h>

class OpenglCube : public OpenglObject
{
    Q_OBJECT
public:
    explicit OpenglCube(cl_double3 pos, cl_double3 size, QColor color, QObject *parent = nullptr);

    void Render();

private:
    cl_double3 verticies[24];
    cl_double3 normals[6]{
        {0, 0, 1},
        {0, 0, -1},
        {1, 0, 0},
        {-1, 0, 0},
        {0, 1, 0},
        {0, -1, 0},
    };
};

#endif // OPENGLCUBE_H
