#ifndef VOXELDATA_H
#define VOXELDATA_H

#include "Utils.h"
#include <QColor>
#include <CL/cl.h>

enum class Zone
{
    Negative = -1,
    Zero = 0,
    Positive = 1
};

struct VoxelData
{
    VoxelData(const Vector3f& position, const Vector3f& size,
              const Zone& zone, const QColor& color):
        position(position),
        size(size),
        color(color),
        zone(zone)
    {}
    VoxelData(const cl_double3& position, const cl_double3& size,
              const Zone& zone, const QColor& color):
        position(position.x, position.y, position.z),
        size(size.x, size.y, size.z),
        color(color),
        zone(zone)
    {}

    Vector3f position;
    Vector3f size;
    QColor color;
    Zone zone;
};

struct Voxel3dData
{
    Voxel3dData(const Vector3f& position, const Vector3f& size,
              const Zone& zone, const QColor& color):
        position(position),
        size(size),
        color(color),
        zone(zone)
    {}
    Vector3f position;
    Vector3f size;
    QColor color;
    Zone zone;
};
struct Voxel2dData
{
    Voxel2dData(const Vector2f& position, const Vector2f& size,
              const Zone& zone, const QColor& color):
        position(position),
        size(size),
        color(color),
        zone(zone)
    {}
    Vector2f position;
    Vector2f size;
    QColor color;
    Zone zone;
};
struct Voxel1dData
{
    Voxel1dData(const float& position, const float& size,
              const Zone& zone, const QColor& color):
        position(position),
        size(size),
        color(color),
        zone(zone)
    {}
    float position;
    float size;
    QColor color;
    Zone zone;
};
#endif // VOXELDATA_H
