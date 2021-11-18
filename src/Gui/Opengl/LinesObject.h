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
    bool Create(int count, QOpenGLShaderProgram* lineShader, QOpenGLShaderProgram *pointShader);
    void AddData(float x, float y, float z,
                 float x1, float y1, float z1,
                 float r, float g, float b, float a);
    void Flush();
    void Destroy();
    void RenderLines();
    void RenderPoints();
    bool IsCreated();
    void Recreate(QOpenGLShaderProgram *lineShader, QOpenGLShaderProgram *pointShader);

private:
    QOpenGLVertexArrayObject pointVao;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    int bufferSize;
    int bufferFill = 0;
    QVector<float> buffer;
    int flushCount = 10000;
    bool isCreated = false;

    int vertexCount;
};


#endif // LINESOBJECT_H
