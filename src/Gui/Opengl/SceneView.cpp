#include "SceneView.h"

#include "Space/SpaceManager.h"
#include <QMouseEvent>

SceneView::SceneView(ShaderMode shaderMode, QWidget *parent):
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    _shaderMode(shaderMode),
    linesObject(nullptr),
    m_pointShader(nullptr),
    voxelObject(nullptr)
{
    m_mouseState.pressed[Qt::RightButton] = false;
    m_mouseState.pressed[Qt::LeftButton] = false;
    m_mouseState.pressed[Qt::MiddleButton] = false;

    m_gridShader = new ShaderProgram(":/shaders/grid.vert", ":/shaders/grid.frag");
    m_gridShader->AddUniform("worldToView");
    m_gridShader->AddUniform("gridColor");
    m_gridShader->AddUniform("backColor");
    if(_shaderMode == ShaderMode::Voxels)
        m_voxelShader = new ShaderProgram(":/shaders/voxel.vert",
                                          ":/shaders/voxel.frag",
                                          ":/shaders/voxel.geom");
    else if(_shaderMode == ShaderMode::Points)
        m_voxelShader = new ShaderProgram(":/shaders/point.vert",
                                          ":/shaders/point.frag");
    else
    {
        m_voxelShader = new ShaderProgram(":/shaders/line.vert",
                                          ":/shaders/line.frag",
                                          ":/shaders/line.geom");
        m_pointShader = new ShaderProgram(":/shaders/linesPoint.vert",
                                          ":/shaders/linesPoint.frag");
        m_pointShader->AddUniform("worldToView");
        m_pointShader->AddUniform("voxSize");
        m_pointShader->AddUniform("useAlpha");
    }
    m_voxelShader->AddUniform("worldToView");
    m_voxelShader->AddUniform("voxSize");
    m_voxelShader->AddUniform("useAlpha");
    gridObject = new GridObject;
    wcsObject = new WcsObject;
    if(_shaderMode != ShaderMode::Lines)
        voxelObject = new VoxelObject;
    else
        linesObject  = new LinesObject;
}

SceneView::~SceneView()
{
    ClearObjects();

    delete m_voxelShader;
    if(m_pointShader)
        delete m_pointShader;
    delete m_gridShader;
    delete gridObject;
    delete wcsObject;
    if(voxelObject)
        delete voxelObject;
    else if(linesObject)
        delete linesObject;
}

void SceneView::AddVoxelObject(float x, float y, float z,
                               float r, float g, float b, float a)
{
    if(voxelObject)
        voxelObject->AddData(x, y, z, r, g, b, a);
}

void SceneView::AddLineObject(float x, float y, float z,
                              float x1, float y1, float z1,
                              float r, float g, float b, float a)
{
    if(linesObject)
        linesObject->AddData(x, y, z, x1, y1, z1, r, g, b, a);
}

void SceneView::Flush()
{
    if(voxelObject)
        voxelObject->Flush();
    else if(linesObject)
        linesObject->Flush();

    updateGL();
}

void SceneView::ClearObjects(bool soft)
{
    if(!soft)
    {
        if(voxelObject)
            voxelObject->Destroy();
        else if(linesObject)
            linesObject->Destroy();
    }
    else
    {
        if(voxelObject)
            voxelObject->Recreate(m_voxelShader->GetRawProgram());
        else if(linesObject)
            linesObject->Recreate(m_voxelShader->GetRawProgram(), m_pointShader->GetRawProgram());
    }
}

void SceneView::CreateVoxelObject(int count)
{
    if(voxelObject)
        voxelObject->Create(count, m_voxelShader->GetRawProgram());
    else if(linesObject)
        linesObject->Create(count, m_voxelShader->GetRawProgram(), m_pointShader->GetRawProgram());
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
    if(m_pointShader)
        if(!m_pointShader->Create())
            qDebug()<<"point shader error";
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

    if(voxelObject && voxelObject->IsCreated())
    {
        m_voxelShader->Bind();
        m_voxelShader->GetRawProgram()->setUniformValue("worldToView", mvpMatrix);
        m_voxelShader->GetRawProgram()->setUniformValue("voxSize", voxSize.x,
                                                        voxSize.y, voxSize.z);
        voxelObject->Render();
        m_voxelShader->Release();
    }
    else if(linesObject && linesObject->IsCreated())
    {
        m_voxelShader->Bind();
        m_voxelShader->GetRawProgram()->setUniformValue("worldToView", mvpMatrix);
        m_voxelShader->GetRawProgram()->setUniformValue("voxSize", voxSize.x,
                                                        voxSize.y, voxSize.z);
        linesObject->RenderLines();
        m_voxelShader->Release();
        if(m_pointShader)
        {
            m_pointShader->Bind();
            m_pointShader->GetRawProgram()->setUniformValue("worldToView", mvpMatrix);
            m_pointShader->GetRawProgram()->setUniformValue("voxSize", voxSize.x,
                                                            voxSize.y, voxSize.z);
            linesObject->RenderPoints();
            m_pointShader->Release();
        }
    }

    m_gridShader->Bind();
    wcsObject->Render();
    m_gridShader->Release();

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
