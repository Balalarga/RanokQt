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
    WcsObject(QObject *parent = nullptr);
    void Create();

    void SetColors(const QVector<QColor>& colors);

protected:
    void UpdateVbo();

private:
    QVector<QColor> colors;
};

#endif // WCSOBJECT_H
