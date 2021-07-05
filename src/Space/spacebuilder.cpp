#include "spacebuilder.h"
#include <cmath>

using namespace std;

SpaceBuilder::SpaceBuilder()
{

}

SpaceBuilder::~SpaceBuilder()
{
    DeleteSpace();
}

Linear3dSpaceData *SpaceBuilder::Create3dSpace(const std::pair<double, double> &dim1,
                                         const std::pair<double, double> &dim2,
                                         const std::pair<double, double> &dim3,
                                         const Vector3i &step)
{
    DeleteSpace();
    _data = new Linear3dSpaceData(step.x*step.y*step.z,
                                  {
                                      static_cast<float>((dim1.second-dim1.first)/step.x),
                                      static_cast<float>((dim2.second-dim2.first)/step.y),
                                      static_cast<float>((dim3.second-dim3.first)/step.z),
                                  });
    double x = dim1.first + _data->pointSize.x/2.;
    while(x < dim1.second)
    {
        double y = dim2.first + _data->pointSize.y/2.;
        while(y < dim2.second)
        {
            double z = dim3.first + _data->pointSize.z/2.;
            while(z < dim3.second)
            {
                _data->points.push_back({x, y, z});
                z += _data->pointSize.z;
            }
            y += _data->pointSize.y;
        }
        x += _data->pointSize.x;
    }

    return _data;
}

Linear3dSpaceData* SpaceBuilder::Create3dSpace(const std::pair<double, double> &dim1,
                                         const std::pair<double, double> &dim2,
                                         const std::pair<double, double> &dim3,
                                         const int &recur)
{
    int depth = pow(2, recur);
    return Create3dSpace(dim1, dim2, dim3, {depth, depth, depth});
}

Linear3dSpaceData *SpaceBuilder::GetSpace()
{
    return _data;
}

void SpaceBuilder::DeleteSpace()
{
    if(_data)
    {
        delete _data;
        _data = nullptr;
    }
}

