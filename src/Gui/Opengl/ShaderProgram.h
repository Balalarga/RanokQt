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
    ShaderProgram(const ShadersList& list, QObject* parent = nullptr);
    ~ShaderProgram();

    bool Create();

    void Bind();
    void Release();

    QStringList uniforms;

    QOpenGLShaderProgram* GetProgram();


private:
    QOpenGLShaderProgram* _program;
    ShadersList _shadersList;

    QList<int> _uniformIDs;
};

#endif // SHADERPROGRAM_H
