#ifndef IOPENGLDRAWABLEOBJECT_H
#define IOPENGLDRAWABLEOBJECT_H

#include <QObject>

#include "ShaderProgram.h"


class IOpenglDrawableObject : public QObject
{
    Q_OBJECT
public:
    explicit IOpenglDrawableObject(ShaderProgram* shader, QObject *parent = nullptr);


    ShaderProgram* GetShaderProgram();


signals:


private:
    ShaderProgram* _shaderProgram;
};

#endif // IOPENGLDRAWABLEOBJECT_H
