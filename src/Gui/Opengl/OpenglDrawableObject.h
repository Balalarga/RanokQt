#ifndef IOPENGLDRAWABLEOBJECT_H
#define IOPENGLDRAWABLEOBJECT_H

#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include "ShaderProgram.h"
#include "VaoLayout.h"


class OpenglDrawableObject : public QObject
{
    Q_OBJECT
public:
    explicit OpenglDrawableObject(const QVector<float>& vertices, const VaoLayout& vaoLayout, QObject *parent = nullptr);

    void SetPrimitive(unsigned primitive);
    ShaderProgram& GetShaderProgram();

    void Render();

private:
    unsigned _primitive;
    unsigned _dataSize;
    ShaderProgram* _shaderProgram;
//    VaoLayout _vaoLayout;

    QOpenGLVertexArrayObject _vao;
    QOpenGLBuffer _vbo;
};

#endif // IOPENGLDRAWABLEOBJECT_H
