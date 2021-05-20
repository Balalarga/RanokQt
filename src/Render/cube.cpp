#include "cube.h"

Cube::Cube(glm::vec3 center, glm::vec3 size, glm::vec4 color)
{
    verticies[0] =  {center.x-size.x, center.y-size.y, center.z+size.z};
    verticies[1] =  {center.x+size.x, center.y-size.y, center.z+size.z};
    verticies[2] =  {center.x+size.x, center.y+size.y, center.z+size.z};
    verticies[3] =  {center.x-size.x, center.y+size.y, center.z+size.z};

    verticies[4] =  {center.x+size.x, center.y-size.y, center.z-size.z};
    verticies[5] =  {center.x-size.x, center.y-size.y, center.z-size.z};
    verticies[6] =  {center.x-size.x, center.y+size.y, center.z-size.z};
    verticies[7] =  {center.x+size.x, center.y+size.y, center.z-size.z};

    verticies[8] =  {center.x+size.x, center.y-size.y, center.z+size.z};
    verticies[9] =  {center.x+size.x, center.y-size.y, center.z-size.z};
    verticies[10] = {center.x+size.x, center.y+size.y, center.z-size.z};
    verticies[11] = {center.x+size.x, center.y+size.y, center.z+size.z};

    verticies[12] = {center.x-size.x, center.y-size.y, center.z-size.z};
    verticies[13] = {center.x-size.x, center.y-size.y, center.z+size.z};
    verticies[14] = {center.x-size.x, center.y+size.y, center.z+size.z};
    verticies[15] = {center.x-size.x, center.y+size.y, center.z-size.z};

    verticies[16] = {center.x-size.x, center.y+size.y, center.z+size.z};
    verticies[17] = {center.x+size.x, center.y+size.y, center.z+size.z};
    verticies[18] = {center.x+size.x, center.y+size.y, center.z-size.z};
    verticies[19] = {center.x-size.x, center.y+size.y, center.z-size.z};

    verticies[20] = {center.x-size.x, center.y-size.y, center.z-size.z};
    verticies[21] = {center.x+size.x, center.y-size.y, center.z-size.z};
    verticies[22] = {center.x+size.x, center.y-size.y, center.z+size.z};
    verticies[23] = {center.x-size.x, center.y-size.y, center.z+size.z};

    this->color = color;
}

void Cube::Draw()
{
    glColor4f(color.r, color.g, color.b, color.a);
    for(int i = 0; i < 6; i++)
    {
        glNormal3f(normals[i].x,normals[i].y,normals[i].z);
        int verId = 4*i;
        glVertex3f(verticies[verId].x,verticies[verId].y,verticies[verId].z);
        glVertex3f(verticies[verId+1].x,verticies[verId+1].y,verticies[verId+1].z);
        glVertex3f(verticies[verId+2].x,verticies[verId+2].y,verticies[verId+2].z);
        glVertex3f(verticies[verId+3].x,verticies[verId+3].y,verticies[verId+3].z);
    }
}
