#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <QGLWidget>
#include <QVector>
#include <QTimer>

#include "AsyncVector.h"
#include "OpenglObject.h"
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
    void AddObject(double x, double y, double z);
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
    QTimer* m_frameTimer;

    ShaderProgram* m_voxelShader;
    ShaderProgram* m_gridShader;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projMatrix;
    QMatrix4x4 mvpMatrix;

    GridObject* gridObject;
    VoxelObject* voxelObject;


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
