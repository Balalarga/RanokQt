#include "square.h"

Square::Square(Vector3 position, Vector3 size, Color color):
    IDrawableObject(color)
{
    verticies[0] = {position.x()-size.x(),
                    position.y()-size.y(), 0};
    verticies[1] =  {position.x()+size.x(),
                     position.y()-size.y(), 0};
    verticies[2] =  {position.x()+size.x(),
                     position.y()+size.y(), 0};
    verticies[3] =  {position.x()-size.x(),
                     position.y()+size.y(), 0};
}

void Square::Draw()
{
    glBegin(GL_QUADS);
    glColor4f(color.r, color.g, color.b, color.a);
    glNormal3f(0, 0, -1);
    for(int j = 0; j < 4; j++)
        glVertex3f(verticies[j].x(),
                   verticies[j].y(),
                   verticies[j].z());
    glEnd();
}
