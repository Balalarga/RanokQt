#ifndef VOXELDATA_H
#define VOXELDATA_H

#include "Utils.h"
#include <QColor>

enum class Zone
{
    Zero,
    Positive,
    Negative
};

struct VoxelData
{
    VoxelData(const Vector3f& position, const Vector3f& size,
              Zone zone, const QColor& color):
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

struct Voxel3dData
{
    Voxel3dData(const Vector3f& position, const Vector3f& size,
              Zone zone, const QColor& color):
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
              Zone zone, const QColor& color):
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
              Zone zone, const QColor& color):
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
