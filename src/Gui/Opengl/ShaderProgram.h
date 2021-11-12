#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <QString>
#include <QStringList>
#include <QOpenGLShaderProgram>

struct ShadersList
{
    ShadersList(const QString& vertexShader,
                const QString& fragmentShader,
                const QString& geometryShader = ""):
        vertexShader(vertexShader),
        fragmentShader(fragmentShader),
        geometryShader(geometryShader)
    {
    }

    QString vertexShader;
    QString fragmentShader;
    QString geometryShader;
};


class ShaderProgram: public QObject
{
public:
    ShaderProgram(const ShadersList& list, QObject* parent = nullptr);
    ~ShaderProgram();

    bool Create();

    void Bind();
    void Release();

    template<class T>
    void SetUniformValue(const char *name, const T& value)
    {
        _program->setUniformValue(name, value);
    }

    QStringList uniforms;

    QOpenGLShaderProgram* GetProgram();


private:
    QOpenGLShaderProgram* _program;
    ShadersList _shadersList;

    QList<int> _uniformIDs;
};

#endif // SHADERPROGRAM_H
