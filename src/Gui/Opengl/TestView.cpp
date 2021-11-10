#include "TestView.h"
#include <QVector2D>

#include <QMouseEvent>

TestView::TestView(QWidget *parent, QSize renderSize):
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    renderSize(renderSize)
{
    m_mouseState.pressed[Qt::RightButton] = false;
    m_mouseState.pressed[Qt::LeftButton] = false;
    m_mouseState.pressed[Qt::MiddleButton] = false;

    voxelShader = new ShaderProgram({":/shaders/Test/default.vert",
                                     ":/shaders/Test/raymarching.frag"});
    voxelShader->uniforms << "worldToView"
                          << "resolution"
                          << "grad_step"
                          << "cameraPosition"
                          << "cameraRotation";


    textureShader = new ShaderProgram({":/shaders/texture.vert",
                                       ":/shaders/texture.frag"});
}

TestView::~TestView()
{
    delete voxelShader;
    delete fbo;
    screenVao.destroy();
    screenVbo.destroy();
}

void TestView::ShaderFromSource(const QString &source)
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
    delete voxelShader;
    voxelShader = new ShaderProgram({":/shaders/Test/default.vert",
                                     tempShaderName});
    voxelShader->uniforms << "worldToView"
                          << "resolution"
                          << "grad_step"
                          << "cameraPosition"
                          << "cameraRotation";

    if(!voxelShader->Create())
    {
        qDebug()<<"voxel shader error";
        return;
    }

    screenVao.destroy();
    screenVbo.destroy();
    if(!screenVao.isCreated())
        screenVao.create();
    screenVao.bind();

    QVector<float> data{
        -1.f, -1.f,
        -1.f, 1.f,
        1.f, 1.f,
        1.f, -1.f,
    };

    screenVbo.create();
    screenVbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    screenVbo.bind();
    screenVbo.allocate(data.data(), sizeof(float)*data.size());
    voxelShader->GetProgram()->enableAttributeArray(0);
    voxelShader->GetProgram()->setAttributeBuffer(0, GL_FLOAT, 0, 2);
    screenVao.release();
    screenVbo.release();
}

void TestView::initializeGL()
{
    glEnable(GL_TEXTURE_2D);

    QVector<float> data{
        -.5f, -.5f,
        -.5f, .5f,
        .5f, .5f,
        .5f, -.5f,
    };

    QVector<float> textureData{
        -1.f, -1.f, 0.f, 0.f,
        -1.f, 1.f, 0.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, -1.f, 1.f, 0.f,
    };

    glClearColor(backColor.x(), backColor.y(), backColor.z(), backColor.w());

    if(!voxelShader->Create())
        qDebug()<<"voxel shader error";

    if(!textureShader->Create())
        qDebug()<<"Texture shader error";
    if(!textureVao.isCreated())
        textureVao.create();
    textureVao.bind();

    textureVbo.create();
    textureVbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    textureVbo.bind();
    textureVbo.allocate(textureData.data(), sizeof(float)*textureData.size());
    textureShader->GetProgram()->enableAttributeArray(0);
    textureShader->GetProgram()->setAttributeBuffer(0, GL_FLOAT, 0, 2, 4*sizeof(float));
    textureShader->GetProgram()->enableAttributeArray(1);
    textureShader->GetProgram()->setAttributeBuffer(1, GL_FLOAT, 2*sizeof(float), 2, 4*sizeof(float));
    textureVao.release();
    textureVbo.release();

    if(!screenVao.isCreated())
        screenVao.create();
    screenVao.bind();

    screenVbo.create();
    screenVbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    screenVbo.bind();
    screenVbo.allocate(data.data(), sizeof(float)*data.size());
    voxelShader->GetProgram()->enableAttributeArray(0);
    voxelShader->GetProgram()->setAttributeBuffer(0, GL_FLOAT, 0, 2);
    screenVao.release();
    screenVbo.release();

    fbo = new QOpenGLFramebufferObject(renderSize);
}

void TestView::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    projMatrix.setToIdentity();
    projMatrix.perspective(45, width/(float)height, 0.1, 10000);
    UpdateMvpMatrix();
}

void TestView::paintGL()
{
    fbo->bind();
    glClearColor(backColor.x(), backColor.y(), backColor.z(), backColor.w());
    glClear(GL_COLOR_BUFFER_BIT);

    constexpr float s = 3.141592/180.f;
    QVector2D cameraRotation(m_camera.xAngle*s, -m_camera.zAngle*s);
    QVector3D cameraPos(0, 0, -m_camera.zoom);

    voxelShader->Bind();
    voxelShader->GetProgram()->setUniformValue("worldToView", mvpMatrix);
    voxelShader->GetProgram()->setUniformValue("grad_step", 0.02f);
    voxelShader->GetProgram()->setUniformValue("resolution", size());
    voxelShader->GetProgram()->setUniformValue("cameraPosition", cameraPos);
    voxelShader->GetProgram()->setUniformValue("cameraRotation", cameraRotation);
    screenVao.bind();
    glDrawArrays(GL_QUADS, 0, 4);
    screenVao.release();
    voxelShader->Release();

    fbo->release();

    textureShader->Bind();
    textureVao.bind();
    glBindTexture(GL_TEXTURE_2D, fbo->takeTexture());
    glDrawArrays(GL_QUADS, 0, 4);
    textureVao.release();
    textureShader->Release();
}

void TestView::mousePressEvent(QMouseEvent *event)
{
    m_mouseState.pos = event->pos();
    m_mouseState.pressed[event->button()] = true;
}

void TestView::mouseReleaseEvent(QMouseEvent *event)
{
    m_mouseState.pressed[event->button()] = false;
}

void TestView::UpdateMvpMatrix()
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

void TestView::mouseMoveEvent(QMouseEvent *event)
{
    if(m_mouseState.pressed[Qt::LeftButton])
    {
        m_camera.xAngle += (m_mouseState.pos.y() - event->pos().y())*0.5;
        m_camera.zAngle += (event->pos().x() - m_mouseState.pos.x())*0.5;
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

void TestView::wheelEvent(QWheelEvent *event)
{
    double dz = 1;
    if(_shiftPressed)
        dz = 0.5;
    if(event->angleDelta().y() > 0 && m_camera.zoom < -dz)
        m_camera.zoom += dz;
    else if(event->angleDelta().y() < 0)
        m_camera.zoom -= dz;
    UpdateMvpMatrix();
}
