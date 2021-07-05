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

#endif // VOXELDATA_H
