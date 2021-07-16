#ifndef MESHOBJECT_H
#define MESHOBJECT_H


#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class GridObject
{
public:
    GridObject() = default;
    void Create(QOpenGLShaderProgram* shader);
    void Destroy();
    void Render();

protected:
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    int bufferSize;

    unsigned linesCount = 1001;
    float lineWidth = 5000;
};

#endif // MESHOBJECT_H
