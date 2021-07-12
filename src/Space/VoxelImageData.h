#ifndef VOXELIMAGEDATA_H
#define VOXELIMAGEDATA_H

#include "Utils.h"
#include <CL/cl.h>
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
    VoxelImageData(const cl_double3& position, const cl_double3& size,
                   const std::map<MImageType, double> &normals):
        position(position.x, position.y, position.z),
        size(size.x, size.y, size.z),
        images(normals)
    {}
    Vector3f position;
    Vector3f size;
    std::map<MImageType, double> images;
};

struct VoxelImage3dData
{
    VoxelImage3dData(const Vector3f& position, const Vector3f& size,
                   const std::map<MImageType, double> &normals):
        position(position),
        size(size),
        images(normals)
    {}
    Vector3f position;
    Vector3f size;
    std::map<MImageType, double> images;
};
struct VoxelImage2dData
{
    VoxelImage2dData(const Vector2f& position, const Vector2f& size,
                   const std::map<MImageType, double> &normals):
        position(position),
        size(size),
        images(normals)
    {}
    Vector2f position;
    Vector2f size;
    std::map<MImageType, double> images;
};
struct VoxelImage1dData
{
    VoxelImage1dData(const float& position, const float& size,
                   const std::map<MImageType, double> &normals):
        position(position),
        size(size),
        images(normals)
    {}
    float position;
    float size;
    std::map<MImageType, double> images;
};
#endif // VOXELIMAGEDATA_H
