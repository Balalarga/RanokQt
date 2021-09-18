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
    void AddData(float x, float y, float z,
                 float r, float g, float b, float a);
    void ChangeColor(float x, float y, float z,
                     float r, float g, float b, float a);
    void Flush();
    void Destroy();
    void Render();
    bool IsCreated();
    void Recreate(QOpenGLShaderProgram* shader);

private:
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    int bufferSize;
    int bufferFill = 0;
    QVector<float> buffer;
    int flushCount = 4096;
    bool isCreated = false;

    int vertexCount;
};

#endif // VOXELOBJECT_H
