#include "SceneView.h"

#include "Space/SpaceBuilder.h"
#include <QMouseEvent>

SceneView::SceneView(QWidget *parent):
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    m_frameTimer(new QTimer(this))
{
    m_frameTimer->setInterval(20);
    connect(m_frameTimer, &QTimer::timeout, this, &QGLWidget::updateGL);
    m_frameTimer->start();

    m_gridShader = new ShaderProgram(":/shaders/grid.vert",":/shaders/grid.frag");
    m_gridShader->AddUniform("worldToView");
    m_gridShader->AddUniform("gridColor");
    m_gridShader->AddUniform("backColor");
    m_voxelShader = new ShaderProgram(":/shaders/voxel.vert",":/shaders/voxel.frag");
    m_voxelShader->AddUniform("worldToView");
    gridObject = new GridObject;
    voxelObject = new VoxelObject;
}

SceneView::~SceneView()
{
    ClearObjects();
    m_voxelShader->Destroy();
    m_gridShader->Destroy();
    gridObject->Destroy();
    voxelObject->Destroy();
    delete m_voxelShader;
    delete m_gridShader;
    delete gridObject;
    delete voxelObject;
}

void SceneView::AddObject(double x, double y, double z)
{
    voxelObject->AddData(x, y, z);
}

void SceneView::Flush()
{
    voxelObject->Flush();
}

void SceneView::ClearObjects()
{
    voxelObject->Destroy();
}

void SceneView::CreateVoxelObject(int count)
{
    voxelObject->Create(count, m_voxelShader->GetRawProgram());
}

void SceneView::initializeGL()
{
    qglClearColor(qRgb(20, 20, 20));

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if(!m_voxelShader->Create())
        qDebug()<<"voxel shader error";
    if(!m_gridShader->Create())
        qDebug()<<"grid shader error";

    gridObject->Create(m_gridShader->GetRawProgram());
}

void SceneView::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    projMatrix.setToIdentity();
    projMatrix.perspective(45, width/(float)height, 0.1, 10000);
    UpdateMvpMatrix();
}

void SceneView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QVector4D backColor(0.1f, 0.15f, 0.3f, 0.5f);
    QVector4D minorGridColor(0.5f, 0.5f, 0.7f, 0.5f);
    glClearColor(backColor.x(), backColor.y(), backColor.z(), backColor.w());

    m_gridShader->Bind();
    m_gridShader->GetRawProgram()->setUniformValue("worldToView", mvpMatrix);
    m_gridShader->GetRawProgram()->setUniformValue("gridColor", minorGridColor);
    m_gridShader->GetRawProgram()->setUniformValue("backColor", backColor);
    gridObject->Render();
    m_gridShader->Release();

    if(voxelObject->IsCreated())
    {
        m_voxelShader->Bind();
        m_voxelShader->GetRawProgram()->setUniformValue("worldToView", mvpMatrix);
        voxelObject->Render();
    }
}

void SceneView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_mouseState.pos = event->pos();
        m_mouseState.pressed = true;
    }
}

void SceneView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_mouseState.pressed = false;
    }
}

void SceneView::UpdateMvpMatrix()
{
    viewMatrix.setToIdentity();
    viewMatrix.lookAt({0.0f, 0.0f, m_camera.zoom},
                      {0.0f, 0.0f, 0.0f},
                      {0.0f, 1.0f, 0.0f});
    viewMatrix.rotate(m_camera.xAngle-90, 1, 0, 0);
    viewMatrix.rotate(m_camera.zAngle, 0, 0, 1);
    mvpMatrix = projMatrix * viewMatrix;
//    updateGL();
}

void SceneView::mouseMoveEvent(QMouseEvent *event)
{
    if(m_mouseState.pressed)
    {
        m_camera.xAngle += (m_mouseState.pos.y() - event->pos().y())*0.5;
        m_camera.zAngle += (event->pos().x() - m_mouseState.pos.x())*0.5;
        m_mouseState.pos = {event->pos().x(), event->pos().y()};
        UpdateMvpMatrix();
    }
}

void SceneView::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0 && m_camera.zoom < -0.5f)
        m_camera.zoom += 0.4f;
    else if(event->angleDelta().y() < 0)
        m_camera.zoom -= 0.4f;
    UpdateMvpMatrix();
}
