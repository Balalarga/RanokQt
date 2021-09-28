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
}

TestView::~TestView()
{
    delete m_voxelShader;
    vao.destroy();
    vbo.destroy();
}

void TestView::ShaderFromSource(const QString &source)
{
    const QString tempShaderName = ".tempFragShaderFile";
    QFile fragFile(tempShaderName);
    fragFile.open(QIODevice::WriteOnly);
    QTextStream stream(&fragFile);
    stream << source;
    fragFile.close();
    delete m_voxelShader;
    m_voxelShader = new ShaderProgram(":/shaders/Test/default.vert",
                                      tempShaderName);
    m_voxelShader->AddUniform("worldToView");
    m_voxelShader->AddUniform("resolution");

    if(!m_voxelShader->Create())
    {
        qDebug()<<"voxel shader error";
        return;
    }

    m_voxelShader->GetRawProgram()->enableAttributeArray(0);
    m_voxelShader->GetRawProgram()->setAttributeBuffer(0, GL_FLOAT, 0, 2);
}

void TestView::initializeGL()
{
    glClearColor(backColor.x(), backColor.y(), backColor.z(), backColor.w());

    if(!m_voxelShader->Create())
        qDebug()<<"voxel shader error";

    m_voxelShader->GetRawProgram()->enableAttributeArray(0);
    m_voxelShader->GetRawProgram()->setAttributeBuffer(0, GL_FLOAT, 0, 2);
    CreateVao();
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
    glClear(GL_COLOR_BUFFER_BIT);

    glClearColor(backColor.x(), backColor.y(), backColor.z(), backColor.w());

    QVector2D resolution(width(), height());

    m_voxelShader->Bind();
    m_voxelShader->GetRawProgram()->setUniformValue("worldToView", mvpMatrix);
    m_voxelShader->GetRawProgram()->setUniformValue("resolution", resolution);
    vao.bind();
    glDrawArrays(GL_QUADS, 0, 4);
    vao.release();
    m_voxelShader->Release();

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
