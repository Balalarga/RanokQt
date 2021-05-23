#include "Vectors.h"
#include <stdexcept>


Vector2::Vector2(double x, double y)
{
    m_values[0] = x;
    m_values[1] = y;
}

double &Vector2::operator[](unsigned id)
{
    if(id < 3)
        return m_values[id];
    throw std::runtime_error("Vector2: Out of range");
}

double &Vector2::x()
{
    return m_values[0];
}

double &Vector2::y()
{
    return m_values[1];
}

Vector2i::Vector2i(int x, int y)
{
    m_values[0] = x;
    m_values[1] = y;
}

int &Vector2i::x()
{
    return m_values[0];
}

int &Vector2i::y()
{
    return m_values[1];
}

Vector3::Vector3(double x, double y, double z)
{
    m_values[0] = x;
    m_values[1] = y;
    m_values[2] = z;
}

double &Vector3::operator[](unsigned id)
{
    if(id < 2)
        return m_values[id];
    throw std::runtime_error("Vector3: Out of range");
}

double &Vector3::x()
{
    return m_values[0];
}

double &Vector3::y()
{
    return m_values[1];
}

double &Vector3::z()
{
    return m_values[2];
}


Vector4::Vector4(double x, double y, double z, double w)
{
    m_values[0] = x;
    m_values[1] = y;
    m_values[2] = z;
    m_values[3] = w;
}

double &Vector4::operator[](unsigned id)
{
    if(id < 4)
        return m_values[id];
    throw std::runtime_error("Vector4: Out of range");
}

double &Vector4::x()
{
    return m_values[0];
}

double &Vector4::y()
{
    return m_values[1];
}

double &Vector4::z()
{
    return m_values[2];
}

double &Vector4::w()
{
    return m_values[3];
}

Vector3i::Vector3i(int x, int y, int z)
{
    m_values[0] = x;
    m_values[1] = y;
    m_values[2] = z;
}

int &Vector3i::operator[](unsigned id)
{
    if(id < 2)
        return m_values[id];
    throw std::runtime_error("Vector3i: Out of range");
}

int &Vector3i::x()
{
    return m_values[0];
}

int &Vector3i::y()
{
    return m_values[1];
}

int &Vector3i::z()
{
    return m_values[2];
}
