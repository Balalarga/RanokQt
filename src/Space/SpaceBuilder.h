#ifndef SPACEBUILDER_H
#define SPACEBUILDER_H


#include <vector>
#include "Utils.h"
#include <CL/cl.h>

struct Linear3dSpaceData
{
    Linear3dSpaceData(int spaceSize, cl_double3 pointSize):
        pointSize(pointSize)
    {
        points.reserve(spaceSize);
    }
    cl_double3 pointSize;
    std::vector<cl_double3> points;
};
struct Linear2dSpaceData
{
    Linear2dSpaceData(int spaceSize, cl_double2 pointSize):
        pointSize(pointSize)
    {
        points.reserve(spaceSize);
    }
    cl_double2 pointSize;
    std::vector<cl_double2> points;
};
struct Linear1dSpaceData
{
    Linear1dSpaceData(int spaceSize, cl_double pointSize):
        pointSize(pointSize)
    {
        points.reserve(spaceSize);
    }
    cl_double pointSize;
    std::vector<cl_double> points;
};

class SpaceBuilder
{
public:
    static SpaceBuilder& Instance(){ static SpaceBuilder builder; return builder; }

    ~SpaceBuilder();

    Linear3dSpaceData* CreateSpace(const std::pair<double, double> &dim1,
                                   const std::pair<double, double> &dim2,
                                   const std::pair<double, double> &dim3,
                                   const Vector3i &step);
    Linear3dSpaceData* CreateSpace(const std::pair<double, double> &dim1,
                                   const std::pair<double, double> &dim2,
                                   const std::pair<double, double> &dim3,
                                   const int &recur);
    Linear2dSpaceData* CreateSpace(const std::pair<double, double> &dim1,
                                   const std::pair<double, double> &dim2,
                                   const Vector2i &step);
    Linear2dSpaceData* CreateSpace(const std::pair<double, double> &dim1,
                                   const std::pair<double, double> &dim2,
                                   const int &recur);
    Linear1dSpaceData* CreateSpace(const std::pair<double, double> &dim1,
                                   const double &step);
    Linear1dSpaceData* CreateSpace(const std::pair<double, double> &dim1,
                                   const int &recur);
    Linear3dSpaceData* Get3dSpace();
    Linear2dSpaceData* Get2dSpace();
    Linear1dSpaceData* Get1dSpace();
    void Delete3dSpace();
    void Delete2dSpace();
    void Delete1dSpace();

private:
    SpaceBuilder();

    Linear3dSpaceData* _data3;
    Linear2dSpaceData* _data2;
    Linear1dSpaceData* _data1;
};

#endif // SPACEBUILDER_H
