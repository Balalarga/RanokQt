#ifndef VOXELIMAGEDATA_H
#define VOXELIMAGEDATA_H

#include "Utils.h"

#include <map>

enum class MImageType
{
    Cx, Cy, Cz, Cw, Ct
};

struct VoxelImageData
{
    VoxelImageData(const Vector3f& position, const Vector3f& size,
                   const std::map<MImageType, double> &normals):
        position(position),
        size(size),
        images(normals)
    {}
    Vector3f position;
    Vector3f size;
    std::map<MImageType, double> images;
};

#endif // VOXELIMAGEDATA_H
