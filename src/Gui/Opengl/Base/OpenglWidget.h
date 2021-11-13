#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QGLWidget>

#include "ShaderManager.h"


class OpenglWidget : public QGLWidget
{
public:
    OpenglWidget(QWidget *parent = nullptr);

    QVector4D GetClearColor() const;
    void SetClearColor(const QVector4D& color);

    inline ShaderManager& GetShaderManager(){ return *_shaderManager; };


protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;


private:
    ShaderManager* _shaderManager;

    QVector4D _clearColor;
};

#endif // OPENGLWIDGET_H
