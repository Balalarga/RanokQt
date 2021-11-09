#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <QString>
#include <QStringList>
#include <QOpenGLShaderProgram>

struct ShadersList
{
    QString vertexShader = "";
    QString fragmentShader = "";
    QString geometryShader = "";
};


class ShaderProgram: public QObject
{
public:
    ShaderProgram(const ShadersList& list);
    ~ShaderProgram();

    bool Create();
    void Destroy();
    void Bind();
    void Release();

    void AddUniform(QString name);
    QOpenGLShaderProgram* GetProgram();

    QStringList m_uniformNames;
    QList<int> m_uniformIDs;

private:
    QOpenGLShaderProgram *m_program;
    ShadersList shadersList;
};

#endif // SHADERPROGRAM_H
