#include "ShaderManager.h"

ShaderManager::ShaderManager(QObject *parent):
    QObject(parent)
{
}

ShaderManager::~ShaderManager()
{
    for(auto& s: _shaderStorage)
        delete s;
}

ShaderProgram *ShaderManager::Add(const QString &tag, const ShadersList &shaders, const QStringList &uniforms)
{
    QString name = tag;
    if(auto shader = Get(tag))
    {
        qDebug()<<"Shader "<<tag<<" already created";
        return shader;
    }

    auto shaderProgram = new ShaderProgram(shaders);
    shaderProgram->uniforms = std::move(uniforms);

    _shaderStorage[name] = shaderProgram;

    return shaderProgram;
}

ShaderProgram *ShaderManager::Add(const QString &tag, const ShadersList &shaders)
{
    QString name = tag;
    if(auto shader = Get(tag))
    {
        qDebug()<<"Shader "<<tag<<" already created";
        return shader;
    }

    auto shaderProgram = new ShaderProgram(shaders);

    _shaderStorage[name] = shaderProgram;

    return shaderProgram;
}

bool ShaderManager::CreateAll()
{
    int status = 0;
    auto shaderNames = _shaderStorage.keys();
    for(auto& name: shaderNames)
    {
        if(!_shaderStorage[name]->IsCreated() && !_shaderStorage[name]->Create())
        {
            status++;
            qDebug()<<"Shader "<< name <<" creating error";
        }
    }
    return status == 0;
}

ShaderProgram *ShaderManager::Get(const QString &tag)
{
    auto it = _shaderStorage.find(tag);
    if(it != _shaderStorage.end())
        return *it;

    return nullptr;
}
