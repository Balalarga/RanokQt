#include "ShaderFactory.h"


ShaderFactory &ShaderFactory::Get()
{
    static ShaderFactory self;
    return self;
}

ShaderProgram *ShaderFactory::Add(const QString &tag, const ShadersList &shaders, const QStringList &uniforms)
{
    if(auto shader = Get(tag))
        return shader;

    auto shaderProgram = new ShaderProgram(shaders);
    shaderProgram->uniforms = std::move(uniforms);

    _shaderStorage[tag] = shaderProgram;

    return shaderProgram;
}

ShaderProgram *ShaderFactory::Add(const QString &tag, const ShadersList &shaders)
{
    if(auto shader = Get(tag))
        return shader;

    auto shaderProgram = new ShaderProgram(shaders);

    _shaderStorage[tag] = shaderProgram;

    return shaderProgram;
}

bool ShaderFactory::CreateAll()
{
    int status = 0;
    auto shaderNames = _shaderStorage.keys();
    for(auto& name: shaderNames)
    {
        if(!_shaderStorage[name]->Create())
        {
            status++;
            qDebug()<<"Shader "<< name <<" creating error";
        }
    }
    return status == 0;
}

ShaderProgram *ShaderFactory::Get(const QString &tag)
{
    auto it = _shaderStorage.find(tag);
    if(it != _shaderStorage.end())
        return *it;

    return nullptr;
}

ShaderFactory::~ShaderFactory()
{
    for(auto& s: _shaderStorage)
        delete s;
}
