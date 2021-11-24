#include "SceneView.h"

#include "Space/SpaceManager.h"
#include <QMouseEvent>

SceneView::SceneView(QWidget *parent):
    OpenglWidget(parent),
    voxelObject(nullptr)
{
    m_mouseState.pressed[Qt::RightButton] = false;
    m_mouseState.pressed[Qt::LeftButton] = false;
    m_mouseState.pressed[Qt::MiddleButton] = false;

    ShadersList gridShadersList(":/shaders/grid.vert", ":/shaders/grid.frag");
    QStringList gridUniforms({"worldToView", "gridColor", "backColor"});
    ShaderProgram* gridShader = GetShaderManager().Add("gridShader",
                                                       gridShadersList,
                                                       gridUniforms);
    VaoLayout gridLayout({VaoLayoutItem(3, GL_FLOAT),
                          VaoLayoutItem(4, GL_FLOAT)});
    gridObject = new GridObject(gridShader, gridLayout, this);
    wcsObject = new WcsObject(gridShader, gridLayout, this);


    ShadersList voxelShadersList(":/shaders/point.vert",
                                 ":/shaders/point.frag");
    VaoLayout voxelLayout = VaoLayout({VaoLayoutItem(3, GL_FLOAT),
                                       VaoLayoutItem(4, GL_FLOAT)});
    QStringList voxelUniforms({"worldToView", "voxSize", "useAlpha"});
    ShaderProgram* voxelShader = GetShaderManager().Add("voxelShader",
                                                        voxelShadersList,
                                                        voxelUniforms);
    voxelObject = new VoxelObject(voxelShader, voxelLayout, this);
}

void SceneView::AddVoxelObject(float x, float y, float z,
                               float r, float g, float b, float a)
{
    voxelObject->AddVoxel(x, y, z, r, g, b, a);
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
        voxelObject->Clear();
    }
}

void SceneView::CreateVoxelObject(int count)
{
    voxelObject->Create(count);
}

void SceneView::SetModelCube(const QVector3D &start, QVector3D &end)
{
    gridObject->SetLinesSpace(start.toVector2D(), end.toVector2D());
}

void SceneView::initializeGL()
{
    OpenglWidget::initializeGL();

    GetShaderManager().CreateAll();

    gridObject->Create();
    wcsObject->Create();
}

void SceneView::resizeGL(int width, int height)
{
    OpenglWidget::resizeGL(width, height);

    projMatrix.setToIdentity();
    projMatrix.perspective(45, width/(float)height, 0.1, 10000);
    UpdateMvpMatrix();
}

void SceneView::paintGL()
{
    OpenglWidget::paintGL();

    Vector3f voxSize = {0.2, 0.2, 0.2};
    if(SpaceManager::Self().WasInited())
        voxSize = SpaceManager::Self().GetPointSize();

    gridObject->BindShader();
    gridObject->GetShaderProgram()->SetUniformValue("worldToView", mvpMatrix);
    gridObject->GetShaderProgram()->SetUniformValue("backColor", GetClearColor());
    gridObject->Render();
    gridObject->ReleaseShader();

    if(voxelObject->IsCreated())
    {
        voxelObject->BindShader();
        voxelObject->GetShaderProgram()->SetUniformValue("worldToView", mvpMatrix);
        voxelObject->GetShaderProgram()->SetUniformValue("voxSize", QVector3D(voxSize.x,
                                                                              voxSize.y,
                                                                              voxSize.z));
        voxelObject->Render();
        voxelObject->ReleaseShader();
    }

    wcsObject->BindShader();
    wcsObject->GetShaderProgram()->SetUniformValue("worldToView", mvpMatrix);
    wcsObject->GetShaderProgram()->SetUniformValue("backColor", GetClearColor());
    wcsObject->Render();
    wcsObject->ReleaseShader();
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

void SceneView::mousePressEvent(QMouseEvent *event)
{
    m_mouseState.pos = event->pos();
    m_mouseState.pressed[event->button()] = true;
}

void SceneView::mouseReleaseEvent(QMouseEvent *event)
{
    m_mouseState.pressed[event->button()] = false;
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
    constexpr double dz = 3;

    if(event->angleDelta().y() > 0 && m_camera.zoom < -dz)
        m_camera.zoom += dz;
    else if(event->angleDelta().y() < 0)
        m_camera.zoom -= dz;

    UpdateMvpMatrix();
}
