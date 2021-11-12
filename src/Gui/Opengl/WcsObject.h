#ifndef WCSOBJECT_H
#define WCSOBJECT_H


#include <QColor>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

#include "OpenglDrawableObject.h"

class WcsObject: public OpenglDrawableObject
{
public:
    WcsObject(ShaderProgram *shaderProgram, const VaoLayout& vaoLayout, QObject *parent = nullptr);
    ~WcsObject();

    void Create();

    void SetColors(const QVector<QColor>& colors);

protected:
    void UpdateVbo();

private:
    QVector<QColor> colors;
};

#endif // WCSOBJECT_H
