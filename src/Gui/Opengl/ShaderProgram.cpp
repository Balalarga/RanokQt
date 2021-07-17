#include "ShaderProgram.h"


ShaderProgram::ShaderProgram(const QString &vertexShaderFilePath, const QString & fragmentShaderFilePath, const QString &geometryShaderFilePath):
    m_vertexShaderFilePath(vertexShaderFilePath),
    m_fragmentShaderFilePath(fragmentShaderFilePath),
    m_geometryShaderFilePath(geometryShaderFilePath),
    m_program(nullptr)
{

}


bool ShaderProgram::Create() {
    m_program = new QOpenGLShaderProgram();

    // read the shader programs from the resource
    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, m_vertexShaderFilePath))
        return false;

    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, m_fragmentShaderFilePath))
        return false;

    if(!m_geometryShaderFilePath.isEmpty() &&
            !m_program->addShaderFromSourceFile(QOpenGLShader::Geometry, m_geometryShaderFilePath))
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

QOpenGLShaderProgram *ShaderProgram::GetRawProgram()
{
    return m_program;
}
