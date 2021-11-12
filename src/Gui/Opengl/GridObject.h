#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include <QColor>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

#include "OpenglDrawableObject.h"

class GridObject: public OpenglDrawableObject
{
public:
    GridObject(ShaderProgram *shaderProgram, const VaoLayout& vaoLayout, QObject *parent = nullptr);
    ~GridObject();

    void SetMainColor(const QColor& color);
    void Create();

protected:
    void UpdateVbo();

private:
    unsigned linesCount = 1001;
    float lineWidth = 5000;

    QColor mainColor;
};

#endif // MESHOBJECT_H
