#ifndef SPACECALCULATOR_H
#define SPACECALCULATOR_H

#include <QObject>

#include "VoxelImageData.h"
#include "VoxelData.h"
#include "Language/Program.h"

class SpaceCalculator: public QObject
{
    Q_OBJECT
public:

    static SpaceCalculator& Get();

    bool GetModel(const Program& program, int batchSize = 0);
    bool GetMImage(const Program& program, int batchSize = 0);

    QColor GetVoxelColor();
    void SetVoxelColor(const QColor &newDefaultColor);

signals:
    void ComputedModel(int start, int count);
    void ComputedMimage(int start, int count);

private:
    SpaceCalculator(QObject* parent = 0);
    QColor _voxelColor;
};

#endif // SPACECALCULATOR_H
