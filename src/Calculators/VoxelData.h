#ifndef VOXELDATA_H
#define VOXELDATA_H

#include "Utils.h"
#include <QColor>

struct VoxelData
{
    VoxelData(){};
    VoxelData(const Vector3f& position, const Vector3f& size,
              const QColor& color):
        position(position),
        size(size),
        color(color)
    {}
    Vector3f position;
    Vector3f size;
    QColor color;
};

#endif // VOXELDATA_H
