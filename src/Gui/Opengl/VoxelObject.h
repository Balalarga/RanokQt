#ifndef VOXELOBJECT_H
#define VOXELOBJECT_H

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QVector>


class VoxelObject
{
public:
    VoxelObject() = default;
    bool Create(int count, QOpenGLShaderProgram* shader);
    void AddData(float x, float y, float z);
    void Flush();
    void Destroy();
    void Render();
    bool IsCreated();

private:
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    int bufferSize;
    int bufferFill = 0;
    QVector<float> buffer;
    int flushCount = 300;
    bool isCreated = false;
};

#endif // VOXELOBJECT_H
