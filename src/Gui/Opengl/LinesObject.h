#ifndef LINESOBJECT_H
#define LINESOBJECT_H

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QVector>


class LinesObject
{
public:
    LinesObject() = default;
    bool Create(int count, QOpenGLShaderProgram* shader);
    void AddData(float x, float y, float z,
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


#endif // LINESOBJECT_H
