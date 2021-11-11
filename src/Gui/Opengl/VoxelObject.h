#ifndef VOXELOBJECT_H
#define VOXELOBJECT_H

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QVector>

#include "OpenglDrawableObject.h"

class VoxelObject: public OpenglDrawableObject
{
public:
    VoxelObject(QObject* parent = nullptr);

    void FullCreate(unsigned count);
    void Destroy() override;

    void AddVoxel(float x, float y, float z,
                  float r, float g, float b, float a);
    void Flush();

private:
    QVector<float> buffer;
    int bufferFill = 0;
    int flushCount = 4096;
};

#endif // VOXELOBJECT_H
