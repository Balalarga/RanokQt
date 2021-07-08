#include "OpenglSquare.h"


OpenglSquare::OpenglSquare(Vector2f pos, Vector2f size, QColor color, QObject *parent):
    OpenglObject(color, parent)
{
    verticies[0] = {pos.x-size.x,
                    pos.y-size.y, 0};
    verticies[1] =  {pos.x+size.x,
                     pos.y-size.y, 0};
    verticies[2] =  {pos.x+size.x,
                     pos.y+size.y, 0};
    verticies[3] =  {pos.x-size.x,
                     pos.y+size.y, 0};
}

OpenglSquare::OpenglSquare(Vector3f pos, Vector3f size, QColor color, QObject *parent):
    OpenglObject(color, parent)
{
    verticies[0] = {pos.x-size.x,
                    pos.y-size.y, 0};
    verticies[1] =  {pos.x+size.x,
                     pos.y-size.y, 0};
    verticies[2] =  {pos.x+size.x,
                     pos.y+size.y, 0};
    verticies[3] =  {pos.x-size.x,
                     pos.y+size.y, 0};
}

void OpenglSquare::Render()
{
    glColor4f(GetColor().redF(), GetColor().greenF(),
              GetColor().blueF(), GetColor().alphaF());
    glNormal3f(0, 0, -1);
    for(int j = 0; j < 4; j++)
        glVertex3f(verticies[j].x,
                   verticies[j].y,
                   verticies[j].z);
}
