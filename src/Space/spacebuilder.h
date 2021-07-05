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

class SpaceBuilder
{
public:
    static SpaceBuilder& Instance(){ static SpaceBuilder builder; return builder; }

    ~SpaceBuilder();

    Linear3dSpaceData* Create3dSpace(const std::pair<double, double> &dim1,
                                     const std::pair<double, double> &dim2,
                                     const std::pair<double, double> &dim3,
                                     const Vector3i &step);
    Linear3dSpaceData* Create3dSpace(const std::pair<double, double> &dim1,
                                     const std::pair<double, double> &dim2,
                                     const std::pair<double, double> &dim3,
                                     const int &recur);
    Linear3dSpaceData* GetSpace();
    void DeleteSpace();

private:
    SpaceBuilder();

    Linear3dSpaceData* _data;
};

#endif // SPACEBUILDER_H
