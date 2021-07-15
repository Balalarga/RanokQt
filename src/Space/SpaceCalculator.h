#ifndef SPACECALCULATOR_H
#define SPACECALCULATOR_H


#include "VoxelImageData.h"
#include "VoxelData.h"
#include "Language/Program.h"


class SpaceCalculator
{
public:
    SpaceCalculator() = delete;

    static bool GetModel(const Program& program);
    static bool GetMImage(const Program& program);

    static QColor GetVoxelColor();
    static void SetVoxelColor(const QColor &newDefaultColor);

private:
    static QColor _voxelColor;
};

#endif // SPACECALCULATOR_H
