#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include <QColor>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

#include "Base/OpenglDrawableObject.h"

class GridObject: public OpenglDrawableObject
{
public:
    GridObject(ShaderProgram *shaderProgram, const VaoLayout& vaoLayout, QObject *parent = nullptr);
    ~GridObject();

    void SetMainColor(const QColor& color);
    void SetLinesSpace(const QVector2D& start, const QVector2D& end);
    void SetLinesStep(float step);

    void Create();
    void Recreate();


protected:
    void UpdateVbo();


private:
    QColor _mainColor;

    QVector4D _linesSpace;
    float _linesStep;

};

#endif // MESHOBJECT_H
