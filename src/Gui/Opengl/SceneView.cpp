#include "SceneView.h"

#include "Space/SpaceManager.h"
#include <QMouseEvent>

SceneView::SceneView(QWidget *parent):
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    voxelObject(nullptr)
{
    m_mouseState.pressed[Qt::RightButton] = false;
    m_mouseState.pressed[Qt::LeftButton] = false;
    m_mouseState.pressed[Qt::MiddleButton] = false;

    m_voxelShader = new ShaderProgram({
                                          ":/shaders/voxel.vert",
                                          ":/shaders/voxel.frag",
                                          ":/shaders/voxel.geom"
                                      });
    m_voxelShader->uniforms << "worldToView"
                            << "voxSize"
                            << "useAlpha";
    gridObject = new GridObject(this);
    wcsObject = new WcsObject(this);
    voxelObject = new VoxelObject;//(this);
}

SceneView::~SceneView()
{
    delete m_voxelShader;
    delete gridObject;
    delete wcsObject;
    delete voxelObject;
}

void SceneView::AddVoxelObject(float x, float y, float z,
                               float r, float g, float b, float a)
{
    if(voxelObject)
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
    {
        voxelObject->Destroy();
    }
    else
    {
        voxelObject->Recreate(m_voxelShader->GetProgram());
    }
}

void SceneView::CreateVoxelObject(int count)
{
    voxelObject->Create(count, m_voxelShader->GetProgram());
}

void SceneView::UseAlphaColor(bool use)
{
    if(use)
        glDisable(GL_DEPTH_TEST);
    else
        glEnable(GL_DEPTH_TEST);
    m_voxelShader->Bind();
    m_voxelShader->SetUniformValue("useAlpha", use);
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

    gridObject->Create();
    wcsObject->Create();
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

    gridObject->BindShader();
    gridObject->GetShaderProgram()->SetUniformValue("worldToView", mvpMatrix);
    gridObject->GetShaderProgram()->SetUniformValue("backColor", backColor);
    gridObject->Render();
    gridObject->ReleaseShader();

    m_voxelShader->Bind();
    m_voxelShader->SetUniformValue("worldToView", mvpMatrix);
    m_voxelShader->SetUniformValue("voxSize", QVector3D(voxSize.x,
                                                 voxSize.y, voxSize.z));
    voxelObject->Render();
    m_voxelShader->Release();

    wcsObject->BindShader();
    wcsObject->GetShaderProgram()->SetUniformValue("worldToView", mvpMatrix);
    wcsObject->GetShaderProgram()->SetUniformValue("backColor", backColor);
    wcsObject->Render();
    wcsObject->ReleaseShader();
}

void SceneView::mousePressEvent(QMouseEvent *event)
{
    m_mouseState.pos = event->pos();
    m_mouseState.pressed[event->button()] = true;
}

void SceneView::mouseReleaseEvent(QMouseEvent *event)
{
    m_mouseState.pressed[event->button()] = false;
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
