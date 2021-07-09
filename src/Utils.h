#ifndef UTILS_H
#define UTILS_H

template<class T>
class Vector2
{
public:
    Vector2():x(0), y(0)
    {}
    Vector2(T x, T y):x(x), y(y)
    {}
    template<class U>
    operator Vector2<U>()
    {
        return Vector2<U>(static_cast<U>(x),
                          static_cast<U>(y));
    }


    T x, y;
};

using Vector2i = Vector2<int>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;


template<class T>
class Vector3
{
public:
    Vector3():x(0), y(0), z(0)
    {}
    Vector3(T x, T y, T z):x(x), y(y), z(z)
    {}
    template<class U>
    operator Vector3<U>()
    {
        return Vector3<U>(static_cast<U>(x),
                          static_cast<U>(y),
                          static_cast<U>(z));
    }

    Vector3<T> operator*(double m)
    {
        return Vector3<T>(x*m, y*m, z*m);
    }
    Vector3<T> operator/(double m)
    {
        return Vector3<T>(x/m, y/m, z/m);
    }


    T x, y, z;
};

using Vector3i = Vector3<int>;
using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;



#endif // UTILS_H
