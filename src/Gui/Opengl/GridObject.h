#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include <QColor>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class GridObject
{
public:
    GridObject() = default;
    ~GridObject();
    void Create(QOpenGLShaderProgram* shader);
    void Destroy();
    void Render();

    void SetMainColor(const QColor& color);

protected:
    void UpdateVbo();

private:
    QColor mainColor;
    QOpenGLShaderProgram* shader;

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    int bufferSize;

    unsigned linesCount = 1001;
    float lineWidth = 5000;
};

#endif // MESHOBJECT_H
