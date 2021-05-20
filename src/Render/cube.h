#ifndef CUBE_H
#define CUBE_H

#include "glInclude.h"

class Cube
{
public:
    Cube(glm::vec3 center, glm::vec3 size, glm::vec4 color);
    void Draw();

private:
    glm::vec3 normals[6]{
        {0, 0, 1},
        {0, 0, -1},
        {1, 0, 0},
        {-1, 0, 0},
        {0, 1, 0},
        {0, -1, 0},
    };
    glm::vec3 verticies[24];
    glm::vec4 color;
};

#endif // CUBE_H
