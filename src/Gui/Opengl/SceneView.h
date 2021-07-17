#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <QGLWidget>
#include <QVector>
#include <QVector4D>

#include "ShaderProgram.h"
#include "GridObject.h"
#include "VoxelObject.h"

class SceneView : public QGLWidget
{
    Q_OBJECT
public:
    explicit SceneView(QWidget *parent = nullptr);
    ~SceneView();

public slots:
    void AddObject(float x, float y, float z,
                   float r, float g, float b, float a);
    void Flush();
    void ClearObjects();
    void CreateVoxelObject(int count);

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void UpdateMvpMatrix();

private:
    ShaderProgram* m_voxelShader;
    ShaderProgram* m_gridShader;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projMatrix;
    QMatrix4x4 mvpMatrix;

    GridObject* gridObject;
    VoxelObject* voxelObject;

    QVector4D backColor{0.1f, 0.15f, 0.3f, 0.5f};

    struct Camera
    {
        float zoom = -5;
        float xAngle = -30;
        float zAngle = 30;
    } m_camera;
    struct MouseState
    {
        QPoint pos;
        bool pressed = false;
    } m_mouseState;

};

#endif // SCENEVIEW_H
