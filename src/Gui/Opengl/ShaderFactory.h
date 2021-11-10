#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

#include <QString>
#include <QMap>

#include "ShaderProgram.h"


class ShaderFactory
{
public:
    static ShaderFactory& Get();
    ShaderProgram* AddAndLoad(const QString& tag, const ShadersList& shaders, const QStringList& uniforms);
    ShaderProgram* Add(const QString& tag, const ShadersList& shaders);
    ShaderProgram* Load(const QString& tag, const QStringList& uniforms);
    ShaderProgram* Get(const QString& tag);


private:
    ShaderFactory() = default;
    ~ShaderFactory();

    QMap<QString, ShaderProgram*> _shaderStorage;
};

#endif // SHADERFACTORY_H
