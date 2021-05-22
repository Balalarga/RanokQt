#ifndef VECTORS_H
#define VECTORS_H


class Vector2
{
public:
    Vector2(double x = 0, double y = 0);
    double& operator[](unsigned id);
    double& x();
    double& y();

private:
    double m_values[2];
};

class Vector2i
{
public:
    Vector2i(int x = 0, int y = 0);
    int& operator[](unsigned id);
    int& x();
    int& y();

private:
    int m_values[2];
};

class Vector3
{
public:
    Vector3(double x = 0, double y = 0, double z = 0);
    double& operator[](unsigned id);
    double& x();
    double& y();
    double& z();

private:
    double m_values[3];
};

class Vector4
{
public:
    Vector4(double x = 0, double y = 0, double z = 0, double w = 0);
    double& operator[](unsigned id);
    double& x();
    double& y();
    double& z();
    double& w();

private:
    double m_values[4];
};

#endif // VECTORS_H
