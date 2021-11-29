#include "RayMarchingView.h"
#include <QVector2D>

#include <QKeyEvent>
#include <QMouseEvent>

RayMarchingView::RayMarchingView(QWidget *parent, QSize renderSize):
    OpenglWidget(parent),
    _renderSize(renderSize),
    fbo(nullptr),
    _screenData({ 1.f,  1.f,
                  1.f, -1.f,
                 -1.f, -1.f,
                 -1.f,  1.f}),
    _textureData({ 1.f,  1.f, 1.f, 1.f,
                   1.f, -1.f, 1.f, 0.f,
                  -1.f, -1.f, 0.f, 0.f,
                  -1.f,  1.f, 0.f, 1.f})
{
    m_mouseState.pressed[Qt::RightButton] = false;
    m_mouseState.pressed[Qt::LeftButton] = false;
    m_mouseState.pressed[Qt::MiddleButton] = false;

    ShadersList screenShadersList(":/shaders/Test/default.vert",
                                ":/shaders/Test/raymarching.frag");
    QStringList screenUniforms({"worldToView", "resolution", "grad_step",
                              "cameraPosition", "cameraRotation"});
    ShaderProgram* screenShader = GetShaderManager().Add("voxelShader",
                                                         screenShadersList,
                                                         screenUniforms);
    VaoLayout screenLayout({VaoLayoutItem(2, GL_FLOAT)});
    _screenRect = new OpenglDrawableObject(screenShader, screenLayout);
    _screenRect->SetPrimitive(GL_QUADS);


    ShadersList textureShaderList(":/shaders/texture.vert",
                                 ":/shaders/texture.frag");
    ShaderProgram* textureShader = GetShaderManager().Add("textureShader",
                                                          textureShaderList);
    VaoLayout textureLayout({VaoLayoutItem(2, GL_FLOAT),
                             VaoLayoutItem(2, GL_FLOAT)});
    _textureRect = new OpenglDrawableObject(textureShader, textureLayout);
    _textureRect->SetPrimitive(GL_QUADS);
}

RayMarchingView::~RayMarchingView()
{
    if(fbo)
        delete fbo;
}

void RayMarchingView::SetRenderSize(const QSize &renderSize)
{
    _renderSize = renderSize;
    if(fbo)
    {
        delete fbo;
        fbo = new QOpenGLFramebufferObject(_renderSize);
    }
    updateGL();
}

void RayMarchingView::ShaderFromSource(const QString &source)
{
    const QString tempShaderName = ".tempFragShaderFile";
    QFile fragFile(tempShaderName);
    if(!fragFile.open(QIODevice::WriteOnly))
    {
        qDebug()<<"Shader write error";
        return;
    }
    QTextStream stream(&fragFile);
    stream << source;
    fragFile.close();

    if(!_screenRect->GetShaderProgram()->Recreate(ShadersList("", tempShaderName)))
        qDebug()<<"Screen shader error";
}

void RayMarchingView::SetShiftState(bool pressed)
{
    _shiftButtonPressed = pressed;
}

void RayMarchingView::initializeGL()
{
    OpenglWidget::initializeGL();

    _screenRect->Create(_screenData);
    _textureRect->Create(_textureData);

    fbo = new QOpenGLFramebufferObject(_renderSize);
}

void RayMarchingView::resizeGL(int width, int height)
{
    OpenglWidget::resizeGL(width, height);

    projMatrix.setToIdentity();
    projMatrix.perspective(45, width/(float)height, 0.1, 10000);
    UpdateMvpMatrix();
}

void RayMarchingView::paintGL()
{
    OpenglWidget::paintGL();

    fbo->bind();
    glClear(GL_COLOR_BUFFER_BIT);

    constexpr float s = 3.141592/180.f;
    QVector2D cameraRotation(m_camera.xAngle*s, -m_camera.zAngle*s);
    QVector3D cameraPos(0, 0, -m_camera.zoom);

    _screenRect->BindShader();
//    _screenRect->GetShaderProgram()->SetUniformValue("worldToView", mvpMatrix);
    _screenRect->GetShaderProgram()->SetUniformValue("grad_step", 0.02f);
    _screenRect->GetShaderProgram()->SetUniformValue("resolution", _renderSize);
    _screenRect->GetShaderProgram()->SetUniformValue("cameraPosition", cameraPos);
    _screenRect->GetShaderProgram()->SetUniformValue("cameraRotation", cameraRotation);
    _screenRect->Render();
    _screenRect->ReleaseShader();

    fbo->release();


    glBindTexture(GL_TEXTURE_2D, fbo->takeTexture());

    _textureRect->BindShader();
    _textureRect->Render();
    _textureRect->ReleaseShader();
}

void RayMarchingView::mousePressEvent(QMouseEvent *event)
{
    m_mouseState.pos = event->pos();
    m_mouseState.pressed[event->button()] = true;
}

void RayMarchingView::mouseReleaseEvent(QMouseEvent *event)
{
    m_mouseState.pressed[event->button()] = false;
}

void RayMarchingView::UpdateMvpMatrix()
{
    viewMatrix.setToIdentity();
    viewMatrix.lookAt({0.0f, 0.0f, m_camera.zoom},
                      {0.0f, 0.0f, 0.0f},
                      {0.0f, 1.0f, 0.0f});
    viewMatrix.rotate(m_camera.xAngle-90, 1, 0, 0);
    viewMatrix.rotate(m_camera.zAngle, 0, 0, 1);

    mvpMatrix = projMatrix * viewMatrix;

    updateGL();
}

void RayMarchingView::mouseMoveEvent(QMouseEvent *event)
{
    if(m_mouseState.pressed[Qt::LeftButton])
    {
        m_camera.xAngle += (m_mouseState.pos.y() - event->pos().y())*0.15;
        m_camera.zAngle += (event->pos().x() - m_mouseState.pos.x())*0.15;
        m_mouseState.pos = {event->pos().x(), event->pos().y()};

        UpdateMvpMatrix();
    }
    else if(m_mouseState.pressed[Qt::RightButton])
    {
        //        m_camera.pos.setY(m_camera.pos.y() + (m_mouseState.pos.y() - event->pos().y())*0.5);
        //        m_camera.pos.setX(m_camera.pos.x() + (event->pos().x() - m_mouseState.pos.x())*0.5);
        //        UpdateMvpMatrix();
    }
}

void RayMarchingView::wheelEvent(QWheelEvent *event)
{
    float dz = 1;

    if(_shiftButtonPressed)
        dz /= 10;

    if(event->angleDelta().y() > 0 && m_camera.zoom < -dz)
        m_camera.zoom += dz;
    else if(event->angleDelta().y() < 0)
        m_camera.zoom -= dz;

    UpdateMvpMatrix();
}
