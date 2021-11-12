#include "ShaderProgram.h"


ShaderProgram::ShaderProgram(const ShadersList &list, QObject *parent):
    QObject(parent),
    _shadersList(list),
    _program(nullptr)
{

}

ShaderProgram::~ShaderProgram()
{
    if(_program)
        delete _program;
    _program = nullptr;
}

bool ShaderProgram::Create() {
    _program = new QOpenGLShaderProgram();

    // read the shader programs from the resource
    if (!_program->addShaderFromSourceFile(QOpenGLShader::Vertex, _shadersList.vertexShader))
        return false;

    if (!_program->addShaderFromSourceFile(QOpenGLShader::Fragment, _shadersList.fragmentShader))
        return false;

    if(!_shadersList.geometryShader.isEmpty() &&
            !_program->addShaderFromSourceFile(QOpenGLShader::Geometry, _shadersList.geometryShader))
        return false;

    if (!_program->link())
        return false;

    _uniformIDs.clear();
    for (const QString& uniformName : qAsConst(uniforms))
        _uniformIDs.append(_program->uniformLocation(uniformName));

    return true;
}

void ShaderProgram::Bind()
{
    if(_program)
        _program->bind();
}

void ShaderProgram::Release()
{
    if(_program)
        _program->release();
}

QOpenGLShaderProgram *ShaderProgram::GetProgram()
{
    return _program;
}
