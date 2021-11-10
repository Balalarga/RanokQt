#include "ShaderFactory.h"


ShaderFactory &ShaderFactory::Get()
{
    static ShaderFactory self;
    return self;
}

ShaderProgram *ShaderFactory::AddAndLoad(const QString &tag, const ShadersList &shaders, const QStringList &uniforms)
{
    if(auto shader = Get(tag))
        return shader;

    auto shaderProgram = new ShaderProgram(shaders);
    shaderProgram->Create();

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

ShaderProgram *ShaderFactory::Load(const QString &tag, const QStringList &uniforms)
{
    if(auto shader = Get(tag))
    {
        shader->uniforms = std::move(uniforms);
        shader->Create();
        return shader;
    }

    return nullptr;
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
