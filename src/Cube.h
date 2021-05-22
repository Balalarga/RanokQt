#ifndef CUBE_H
#define CUBE_H

#include "Vectors.h"
#include "Color.h"
#include <GLEW/glew.h>
#include "IDrawableObject.h"

class Cube: public IDrawableObject
{
public:
    Cube(Vector3 position, Vector3 size, Color color):
        IDrawableObject(color)
    {
        verticies[0] =  {position.x()-size.x(),
                         position.y()-size.y(), position.z()+size.z()};
        verticies[1] =  {position.x()+size.x(),
                         position.y()-size.y(), position.z()+size.z()};
        verticies[2] =  {position.x()+size.x(),
                         position.y()+size.y(), position.z()+size.z()};
        verticies[3] =  {position.x()-size.x(),
                         position.y()+size.y(), position.z()+size.z()};

        verticies[4] =  {position.x()+size.x(),
                         position.y()-size.y(), position.z()-size.z()};
        verticies[5] =  {position.x()-size.x(),
                         position.y()-size.y(), position.z()-size.z()};
        verticies[6] =  {position.x()-size.x(),
                         position.y()+size.y(), position.z()-size.z()};
        verticies[7] =  {position.x()+size.x(),
                         position.y()+size.y(), position.z()-size.z()};

        verticies[8] =  {position.x()+size.x(),
                         position.y()-size.y(), position.z()+size.z()};
        verticies[9] =  {position.x()+size.x(),
                         position.y()-size.y(), position.z()-size.z()};
        verticies[10] = {position.x()+size.x(),
                         position.y()+size.y(), position.z()-size.z()};
        verticies[11] = {position.x()+size.x(),
                         position.y()+size.y(), position.z()+size.z()};

        verticies[12] = {position.x()-size.x(),
                         position.y()-size.y(), position.z()-size.z()};
        verticies[13] = {position.x()-size.x(),
                         position.y()-size.y(), position.z()+size.z()};
        verticies[14] = {position.x()-size.x(),
                         position.y()+size.y(), position.z()+size.z()};
        verticies[15] = {position.x()-size.x(),
                         position.y()+size.y(), position.z()-size.z()};

        verticies[16] = {position.x()-size.z(),
                         position.y()+size.y(), position.z()+size.z()};
        verticies[17] = {position.x()+size.z(),
                         position.y()+size.y(), position.z()+size.z()};
        verticies[18] = {position.x()+size.x(),
                         position.y()+size.y(), position.z()-size.z()};
        verticies[19] = {position.x()-size.x(),
                         position.y()+size.y(), position.z()-size.z()};

        verticies[20] = {position.x()-size.x(),
                         position.y()-size.y(), position.z()-size.z()};
        verticies[21] = {position.x()+size.x(),
                         position.y()-size.y(), position.z()-size.z()};
        verticies[22] = {position.x()+size.x(),
                         position.y()-size.y(), position.z()+size.z()};
        verticies[23] = {position.x()-size.x(),
                         position.y()-size.y(), position.z()+size.z()};
    }
    void Draw()
    {
        glBegin(GL_QUADS);
        glColor4f(color.r, color.g, color.b, color.a);
        for(int i = 0; i < 6; i++)
        {
            glNormal3f(normals[i].x(),normals[i].y(),normals[i].z());
            int verId = 4*i;
            for(int j = 0; j < 4; j++)
                glVertex3f(verticies[verId+j].x(),
                        verticies[verId+j].y(),
                        verticies[verId+j].z());
        }
        glEnd();
    }
private:
    Vector3 verticies[24];
    Vector3 normals[6]{
        {0, 0, 1},
        {0, 0, -1},
        {1, 0, 0},
        {-1, 0, 0},
        {0, 1, 0},
        {0, -1, 0},
    };
};

#endif // CUBE_H
