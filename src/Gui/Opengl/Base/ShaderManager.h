#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

#include <QString>
#include <QMap>

#include "ShaderProgram.h"


class ShaderManager: public QObject
{
public:
    ShaderManager(QObject* parent = nullptr);
    ~ShaderManager();

    ShaderProgram* Add(const QString& tag, const ShadersList& shaders, const QStringList& uniforms);
    ShaderProgram* Add(const QString& tag, const ShadersList& shaders);

    bool CreateAll();

    ShaderProgram* Get(const QString& tag);

private:
    QMap<QString, ShaderProgram*> _shaderStorage;
};

#endif // SHADERFACTORY_H
