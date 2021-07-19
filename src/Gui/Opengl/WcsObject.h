#ifndef WCSOBJECT_H
#define WCSOBJECT_H


#include <QColor>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class WcsObject
{
public:
    WcsObject() = default;
    ~WcsObject();
    void Create(QOpenGLShaderProgram* shader);
    void Destroy();
    void Render();

    void SetColors(const QVector<QColor>& colors);


protected:
    void UpdateVbo();


private:
    QVector<QColor> colors;
    QOpenGLShaderProgram* shader;

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    int bufferSize;
};

#endif // WCSOBJECT_H
