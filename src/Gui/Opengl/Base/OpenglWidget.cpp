#include "OpenglWidget.h"

OpenglWidget::OpenglWidget(QWidget *parent):
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    _shaderManager(new ShaderManager(this)),
    _clearColor(0.1f, 0.15f, 0.3f, 0.5f)
{

}


QVector4D OpenglWidget::GetClearColor() const
{
    return _clearColor;
}

void OpenglWidget::SetClearColor(const QVector4D &color)
{
    _clearColor = color;

    glClearColor(_clearColor.x(), _clearColor.y(), _clearColor.z(), _clearColor.w());
}

void OpenglWidget::initializeGL()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(_clearColor.x(), _clearColor.y(), _clearColor.z(), _clearColor.w());
}

void OpenglWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void OpenglWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
