#ifndef SPACEBUILDER_H
#define SPACEBUILDER_H


#include <vector>
#include "Utils.h"

struct Linear3dSpaceData
{
    Linear3dSpaceData(int spaceSize, Vector3d pointSize):
        pointSize(pointSize)
    {
        points.reserve(spaceSize);
    }
    Vector3d pointSize;
    std::vector<Vector3d> points;
};
struct Linear2dSpaceData
{
    Linear2dSpaceData(int spaceSize, Vector2d pointSize):
        pointSize(pointSize)
    {
        points.reserve(spaceSize);
    }
    Vector2d pointSize;
    std::vector<Vector2d> points;
};
struct Linear1dSpaceData
{
    Linear1dSpaceData(int spaceSize, double pointSize):
        pointSize(pointSize)
    {
        points.reserve(spaceSize);
    }
    double pointSize;
    std::vector<double> points;
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
