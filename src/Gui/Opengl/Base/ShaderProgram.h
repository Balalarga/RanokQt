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
    QStringList uniforms;

    ShaderProgram(const ShadersList& list, QObject* parent = nullptr);
    ~ShaderProgram();

    bool Create();
    bool Recreate(const ShadersList& list);
    bool IsCreated();

    void Bind();
    void Release();

    QOpenGLShaderProgram* GetProgram();

    template<class T>
    void SetUniformValue(const char *name, const T& value)
    {
        _program->setUniformValue(name, value);
    }


private:
    QOpenGLShaderProgram* _program;
    ShadersList _shadersList;

    QList<int> _uniformIDs;
};

#endif // SHADERPROGRAM_H
