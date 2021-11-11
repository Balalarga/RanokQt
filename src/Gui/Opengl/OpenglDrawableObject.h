#ifndef IOPENGLDRAWABLEOBJECT_H
#define IOPENGLDRAWABLEOBJECT_H

#include <memory>

#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include "ShaderProgram.h"
#include "VaoLayout.h"


class OpenglDrawableObject : public QObject
{
    Q_OBJECT
public:
    explicit OpenglDrawableObject(ShaderProgram *shaderProgram, const VaoLayout& vaoLayout, QObject *parent = nullptr);
    virtual ~OpenglDrawableObject();

    virtual void Create(unsigned verticesCount);
    virtual void Create(const QVector<float>& vertices);

    void Destroy();

    void AddData(const QVector<float>& vertices);
    void SetPrimitive(unsigned primitive);
    void BindShader();
    void ReleaseShader();
    void Render();

    unsigned GetLayoutSize() const;
    unsigned GetFillingSize() const;
    unsigned GetSize() const;

    ShaderProgram* GetShaderProgram();

private:
    unsigned _primitive;
    unsigned _verticesCount;
    unsigned _verticesFilling;

    const VaoLayout _vaoLayout;

    ShaderProgram* _shaderProgram;
    QOpenGLVertexArrayObject _vao;
    QOpenGLBuffer _vbo;
};

#endif // IOPENGLDRAWABLEOBJECT_H
