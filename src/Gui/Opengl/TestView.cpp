#include "TestView.h"
#include <QVector2D>

#include <QMouseEvent>

TestView::TestView(QWidget *parent):
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    m_mouseState.pressed[Qt::RightButton] = false;
    m_mouseState.pressed[Qt::LeftButton] = false;
    m_mouseState.pressed[Qt::MiddleButton] = false;

    m_voxelShader = new ShaderProgram(":/shaders/Test/default.vert",
                                      ":/shaders/Test/raymarching.frag");
    m_voxelShader->AddUniform("worldToView");
    m_voxelShader->AddUniform("resolution");
    m_voxelShader->AddUniform("grad_step");
    m_voxelShader->AddUniform("cameraPosition");
    m_voxelShader->AddUniform("cameraRotation");


    textureShader = new ShaderProgram(":/shaders/texture.vert",
                                      ":/shaders/texture.frag");
}

TestView::~TestView()
{
    delete m_voxelShader;
    delete fbo;
    vao.destroy();
    vbo.destroy();
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
    delete m_voxelShader;
    m_voxelShader = new ShaderProgram(":/shaders/Test/default.vert",
                                      tempShaderName);
    m_voxelShader->AddUniform("worldToView");
    m_voxelShader->AddUniform("resolution");
    m_voxelShader->AddUniform("grad_step");
    m_voxelShader->AddUniform("cameraPosition");
    m_voxelShader->AddUniform("cameraRotation");

    if(!m_voxelShader->Create())
    {
        qDebug()<<"voxel shader error";
        return;
    }

    vao.destroy();
    vbo.destroy();
    if(!vao.isCreated())
        vao.create();
    vao.bind();

    QVector<float> data{
        -1.f, -1.f,
        -1.f, 1.f,
        1.f, 1.f,
        1.f, -1.f,
    };

    vbo.create();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.bind();
    vbo.allocate(data.data(), sizeof(float)*data.size());
    m_voxelShader->GetRawProgram()->enableAttributeArray(0);
    m_voxelShader->GetRawProgram()->setAttributeBuffer(0, GL_FLOAT, 0, 2);
    vao.release();
    vbo.release();
}

void TestView::initializeGL()
{
    QVector<float> data{
        -1.f, -1.f,
        -1.f, 1.f,
        1.f, 1.f,
        1.f, -1.f,
    };

    QVector<float> textureData{
        -1.f, -1.f, 0.f, 0.f,
        -1.f, 1.f, 0.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, -1.f, 1.f, 0.f,
    };

    glClearColor(backColor.x(), backColor.y(), backColor.z(), backColor.w());

    if(!m_voxelShader->Create())
        qDebug()<<"voxel shader error";

    if(!textureShader->Create())
        qDebug()<<"Texture shader error";
    textureVao.create();
    textureVao.bind();

    textureVbo.create();
    textureVbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    textureVbo.bind();
    textureVbo.allocate(textureData.data(), sizeof(float)*textureData.size());
    textureShader->GetRawProgram()->enableAttributeArray(0);
    textureShader->GetRawProgram()->setAttributeBuffer(0, GL_FLOAT, 0, 2, 4*sizeof(float));
    textureShader->GetRawProgram()->enableAttributeArray(1);
    textureShader->GetRawProgram()->setAttributeBuffer(1, GL_FLOAT, 2*sizeof(float), 2, 4*sizeof(float));
    textureVao.release();
    textureVbo.release();

    if(!vao.isCreated())
        vao.create();
    vao.bind();

    vbo.create();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.bind();
    vbo.allocate(data.data(), sizeof(float)*data.size());
    m_voxelShader->GetRawProgram()->enableAttributeArray(0);
    m_voxelShader->GetRawProgram()->setAttributeBuffer(0, GL_FLOAT, 0, 2);
    vao.release();
    vbo.release();

    fbo = new QOpenGLFramebufferObject(QSize(1280, 720));

    glEnable(GL_TEXTURE_2D);
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

    m_voxelShader->Bind();
    m_voxelShader->GetRawProgram()->setUniformValue("worldToView", mvpMatrix);
    m_voxelShader->GetRawProgram()->setUniformValue("grad_step", 0.02f);
    m_voxelShader->GetRawProgram()->setUniformValue("resolution", size());
    m_voxelShader->GetRawProgram()->setUniformValue("cameraPosition", cameraPos);
    m_voxelShader->GetRawProgram()->setUniformValue("cameraRotation", cameraRotation);
    vao.bind();
    glDrawArrays(GL_QUADS, 0, 4);
    vao.release();    
    m_voxelShader->Release();

    fbo->release();

    textureShader->Bind();
    glBindTexture(GL_TEXTURE_2D, fbo->texture());
    textureVao.bind();
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

void TestView::CreateVao()
{
    if(!vao.isCreated())
        vao.create();
    vao.bind();
    QVector<float> data{
        -1.f, -1.f,
        -1.f, 1.f,
        1.f, 1.f,
        1.f, -1.f,
    };

    vbo.create();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.bind();
    vbo.allocate(data.data(), sizeof(float)*data.size());

    vao.release();
    vbo.release();

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
    double dz = 3;
    if(_shiftPressed)
        dz = 0.5;
    if(event->angleDelta().y() > 0 && m_camera.zoom < -dz)
        m_camera.zoom += dz;
    else if(event->angleDelta().y() < 0)
        m_camera.zoom -= dz;
    UpdateMvpMatrix();
}
