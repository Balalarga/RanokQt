#include "spacebuilder.h"
#include <cmath>

using namespace std;

SpaceBuilder::SpaceBuilder()
{

}

SpaceBuilder::~SpaceBuilder()
{
    Delete3dSpace();
    Delete2dSpace();
    Delete1dSpace();
}


Linear3dSpaceData *SpaceBuilder::CreateSpace(const std::pair<double, double> &dim1,
                                         const std::pair<double, double> &dim2,
                                         const std::pair<double, double> &dim3,
                                         const Vector3i &step)
{
    Delete3dSpace();
    _data3 = new Linear3dSpaceData(step.x*step.y*step.z,
                                  {
                                      static_cast<float>((dim1.second-dim1.first)/step.x),
                                      static_cast<float>((dim2.second-dim2.first)/step.y),
                                      static_cast<float>((dim3.second-dim3.first)/step.z),
                                  });
    double x = dim1.first + _data3->pointSize.x/2.;
    while(x < dim1.second)
    {
        double y = dim2.first + _data3->pointSize.y/2.;
        while(y < dim2.second)
        {
            double z = dim3.first + _data3->pointSize.z/2.;
            while(z < dim3.second)
            {
                _data3->points.push_back({x, y, z});
                z += _data3->pointSize.z;
            }
            y += _data3->pointSize.y;
        }
        x += _data3->pointSize.x;
    }

    return _data3;
}

Linear3dSpaceData* SpaceBuilder::CreateSpace(const std::pair<double, double> &dim1,
                                         const std::pair<double, double> &dim2,
                                         const std::pair<double, double> &dim3,
                                         const int &recur)
{
    int depth = pow(2, recur);
    return CreateSpace(dim1, dim2, dim3, {depth, depth, depth});
}

Linear2dSpaceData *SpaceBuilder::CreateSpace(const std::pair<double, double> &dim1,
                                             const std::pair<double, double> &dim2,
                                             const Vector2i &step)
{
    Delete2dSpace();
    _data2 = new Linear2dSpaceData(step.x*step.y,
                                  {
                                      static_cast<float>((dim1.second-dim1.first)/step.x),
                                      static_cast<float>((dim2.second-dim2.first)/step.y)
                                  });
    double x = dim1.first + _data3->pointSize.x/2.;
    while(x < dim1.second)
    {
        double y = dim2.first + _data3->pointSize.y/2.;
        while(y < dim2.second)
        {
            _data2->points.push_back({x, y});
            y += _data2->pointSize.y;
        }
        x += _data2->pointSize.x;
    }

    return _data2;
}

Linear2dSpaceData *SpaceBuilder::CreateSpace(const std::pair<double, double> &dim1,
                                             const std::pair<double, double> &dim2,
                                             const int &recur)
{
    int depth = pow(2, recur);
    return CreateSpace(dim1, dim2, depth);
}

Linear1dSpaceData *SpaceBuilder::CreateSpace(const std::pair<double, double> &dim1,
                                             const double &step)
{
    _data1 = new Linear1dSpaceData(step,static_cast<float>((dim1.second-dim1.first)/step));

    double x = dim1.first + _data3->pointSize.x/2.;
    while(x < dim1.second)
    {
        _data1->points.push_back(x);
        x += _data1->pointSize;
    }

    return _data1;
}

Linear1dSpaceData *SpaceBuilder::CreateSpace(const std::pair<double, double> &dim1,
                                             const int &recur)
{
    int depth = pow(2, recur);
    return CreateSpace(dim1, depth);
}


Linear3dSpaceData *SpaceBuilder::Get3dSpace()
{
    return _data3;
}
Linear2dSpaceData *SpaceBuilder::Get2dSpace()
{
    return _data2;
}
Linear1dSpaceData *SpaceBuilder::Get1dSpace()
{
    return _data1;
}


void SpaceBuilder::Delete3dSpace()
{
    if(_data3)
    {
        delete _data3;
        _data3 = nullptr;
    }
}

void SpaceBuilder::Delete2dSpace()
{
    if(_data2)
    {
        delete _data2;
        _data2 = nullptr;
    }
}

void SpaceBuilder::Delete1dSpace()
{
    if(_data1)
    {
        delete _data1;
        _data1 = nullptr;
    }
}
