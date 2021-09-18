#include "SceneView.h"

#include "Space/SpaceManager.h"
#include <QMouseEvent>

SceneView::SceneView(QWidget *parent):
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    m_gridShader = new ShaderProgram(":/shaders/grid.vert", ":/shaders/grid.frag");
    m_gridShader->AddUniform("worldToView");
    m_gridShader->AddUniform("gridColor");
    m_gridShader->AddUniform("backColor");
    m_voxelShader = new ShaderProgram(":/shaders/voxel.vert",
                                      ":/shaders/voxel.frag",
                                      ":/shaders/voxel.geom");
    m_voxelShader->AddUniform("worldToView");
    m_voxelShader->AddUniform("voxSize");
    m_voxelShader->AddUniform("useAlpha");
    gridObject = new GridObject;
    wcsObject = new WcsObject;
    voxelObject = new VoxelObject;
}

SceneView::~SceneView()
{
    ClearObjects();

    delete m_voxelShader;
    delete m_gridShader;
    delete gridObject;
    delete wcsObject;
    delete voxelObject;
}

void SceneView::AddObject(float x, float y, float z, float r, float g, float b, float a)
{
    voxelObject->AddData(x, y, z, r, g, b, a);
}

void SceneView::Flush()
{
    voxelObject->Flush();
    updateGL();
}

void SceneView::ClearObjects(bool soft)
{
    if(!soft)
        voxelObject->Destroy();
    else
        voxelObject->Recreate(m_voxelShader->GetRawProgram());
}

void SceneView::CreateVoxelObject(int count)
{
    voxelObject->Create(count, m_voxelShader->GetRawProgram());
}

void SceneView::UseAlphaColor(bool use)
{
    if(use)
        glDisable(GL_DEPTH_TEST);
    else
        glEnable(GL_DEPTH_TEST);
    m_voxelShader->Bind();
    m_voxelShader->GetRawProgram()->setUniformValue("useAlpha", use);
    m_voxelShader->Release();
}

void SceneView::initializeGL()
{
    glClearColor(backColor.x(), backColor.y(), backColor.z(), backColor.w());

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if(!m_voxelShader->Create())
        qDebug()<<"voxel shader error";
    if(!m_gridShader->Create())
        qDebug()<<"grid shader error";

    gridObject->Create(m_gridShader->GetRawProgram());
    wcsObject->Create(m_gridShader->GetRawProgram());
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
    cl_float3 voxSize = {0.2, 0.2, 0.2};
    if(SpaceManager::Self().WasInited())
        voxSize = SpaceManager::Self().GetPointSize();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(backColor.x(), backColor.y(), backColor.z(), backColor.w());

    m_gridShader->Bind();
    m_gridShader->GetRawProgram()->setUniformValue("worldToView", mvpMatrix);
    m_gridShader->GetRawProgram()->setUniformValue("backColor", backColor);
    gridObject->Render();
    m_gridShader->Release();

    if(voxelObject->IsCreated())
    {
        m_voxelShader->Bind();
        m_voxelShader->GetRawProgram()->setUniformValue("worldToView", mvpMatrix);
        m_voxelShader->GetRawProgram()->setUniformValue("voxSize", voxSize.x, voxSize.y, voxSize.z);
        voxelObject->Render();
        m_voxelShader->Release();
    }

    m_gridShader->Bind();
    wcsObject->Render();
    m_gridShader->Release();

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

void SceneView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Shift)
        _shiftPressed = true;
}

void SceneView::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Shift)
        _shiftPressed = false;
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
    updateGL();
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
    double dz = 3;
    if(_shiftPressed)
        dz = 0.5;
    if(event->angleDelta().y() > 0 && m_camera.zoom < -dz)
        m_camera.zoom += dz;
    else if(event->angleDelta().y() < 0)
        m_camera.zoom -= dz;
    UpdateMvpMatrix();
}
