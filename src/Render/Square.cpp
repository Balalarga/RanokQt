#include "Square.h"

Square::Square(sf::Vector3<double> position, sf::Vector3<double> size, sf::Color color):
    IDrawableObject(color)
{
    verticies[0] = {position.x-size.x,
                    position.y-size.y, 0};
    verticies[1] =  {position.x+size.x,
                     position.y-size.y, 0};
    verticies[2] =  {position.x+size.x,
                     position.y+size.y, 0};
    verticies[3] =  {position.x-size.x,
                     position.y+size.y, 0};
}

void Square::Draw()
{
    glBegin(GL_QUADS);
    glColor4d(color.r/255., color.g/255., color.b/255., color.a/255.);
    glNormal3f(0, 0, -1);
    for(int j = 0; j < 4; j++)
        glVertex3f(verticies[j].x,
                   verticies[j].y,
                   verticies[j].z);
    glEnd();
}
