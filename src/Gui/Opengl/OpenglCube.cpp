#include "OpenglCube.h"

OpenglCube::OpenglCube(cl_double3 pos, cl_double3 size, QColor color, QObject *parent):
    OpenglObject(color, parent)
{
    verticies[0] =  {pos.x-size.x,
                     pos.y-size.y, pos.z+size.z};
    verticies[1] =  {pos.x+size.x,
                     pos.y-size.y, pos.z+size.z};
    verticies[2] =  {pos.x+size.x,
                     pos.y+size.y, pos.z+size.z};
    verticies[3] =  {pos.x-size.x,
                     pos.y+size.y, pos.z+size.z};

    verticies[4] =  {pos.x+size.x,
                     pos.y-size.y, pos.z-size.z};
    verticies[5] =  {pos.x-size.x,
                     pos.y-size.y, pos.z-size.z};
    verticies[6] =  {pos.x-size.x,
                     pos.y+size.y, pos.z-size.z};
    verticies[7] =  {pos.x+size.x,
                     pos.y+size.y, pos.z-size.z};

    verticies[8] =  {pos.x+size.x,
                     pos.y-size.y, pos.z+size.z};
    verticies[9] =  {pos.x+size.x,
                     pos.y-size.y, pos.z-size.z};
    verticies[10] = {pos.x+size.x,
                     pos.y+size.y, pos.z-size.z};
    verticies[11] = {pos.x+size.x,
                     pos.y+size.y, pos.z+size.z};

    verticies[12] = {pos.x-size.x,
                     pos.y-size.y, pos.z-size.z};
    verticies[13] = {pos.x-size.x,
                     pos.y-size.y, pos.z+size.z};
    verticies[14] = {pos.x-size.x,
                     pos.y+size.y, pos.z+size.z};
    verticies[15] = {pos.x-size.x,
                     pos.y+size.y, pos.z-size.z};

    verticies[16] = {pos.x-size.z,
                     pos.y+size.y, pos.z+size.z};
    verticies[17] = {pos.x+size.z,
                     pos.y+size.y, pos.z+size.z};
    verticies[18] = {pos.x+size.x,
                     pos.y+size.y, pos.z-size.z};
    verticies[19] = {pos.x-size.x,
                     pos.y+size.y, pos.z-size.z};

    verticies[20] = {pos.x-size.x,
                     pos.y-size.y, pos.z-size.z};
    verticies[21] = {pos.x+size.x,
                     pos.y-size.y, pos.z-size.z};
    verticies[22] = {pos.x+size.x,
                     pos.y-size.y, pos.z+size.z};
    verticies[23] = {pos.x-size.x,
                     pos.y-size.y, pos.z+size.z};
}

void OpenglCube::Render()
{
    glColor4f(GetColor().redF(), GetColor().greenF(),
              GetColor().blueF(), GetColor().alphaF());
    for(int i = 0; i < 6; i++)
    {
        glNormal3f(normals[i].x,normals[i].y,normals[i].z);
        int verId = 4*i;
        for(int j = 0; j < 4; j++)
            glVertex3f(verticies[verId+j].x,
                    verticies[verId+j].y,
                    verticies[verId+j].z);
    }
}
