#include "ShaderProgram.h"


ShaderProgram::ShaderProgram(const ShadersList &list):
    shadersList(list),
    m_program(nullptr)
{

}

ShaderProgram::~ShaderProgram()
{
    Destroy();
}

bool ShaderProgram::Create() {
    m_program = new QOpenGLShaderProgram();

    // read the shader programs from the resource
    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, shadersList.vertexShader))
        return false;

    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, shadersList.fragmentShader))
        return false;

    if(!shadersList.geometryShader.isEmpty() &&
            !m_program->addShaderFromSourceFile(QOpenGLShader::Geometry, shadersList.geometryShader))
        return false;

    if (!m_program->link())
        return false;

    m_uniformIDs.clear();
    for (const QString& uniformName : qAsConst(m_uniformNames))
        m_uniformIDs.append( m_program->uniformLocation(uniformName));

    return true;
}

void ShaderProgram::Destroy()
{
    if(m_program)
        delete m_program;
    m_program = nullptr;
}

void ShaderProgram::Bind()
{
    if(m_program)
        m_program->bind();
}

void ShaderProgram::Release()
{
    if(m_program)
        m_program->release();
}

void ShaderProgram::AddUniform(QString name)
{
    m_uniformNames.push_back(name);
}

QOpenGLShaderProgram *ShaderProgram::GetProgram()
{
    return m_program;
}
