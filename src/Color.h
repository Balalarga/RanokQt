#ifndef COLOR_H
#define COLOR_H


struct Color
{
    Color():
        r(0), g(0), b(0), a(0)
    {}
    Color(float r, float g, float b, float a = 1):
        r(r), g(g), b(b), a(a)
    {}

    float r, g, b, a;
};

#endif // COLOR_H
