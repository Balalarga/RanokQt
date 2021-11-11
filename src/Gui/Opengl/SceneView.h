#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <QGLWidget>
#include <QOpenGLFunctions>
#include <QThread>
#include <QVector>
#include <QVector4D>
#include <QMap>

#include "ShaderProgram.h"
#include "GridObject.h"
#include "VoxelObject.h"
#include "LinesObject.h"
#include "WcsObject.h"



class SceneView : public QGLWidget
{
    Q_OBJECT
public:
    explicit SceneView(QWidget *parent = nullptr);
    ~SceneView();

public slots:
    void AddVoxelObject(float x, float y, float z,
                   float r, float g, float b, float a);
    void Flush();
    void ClearObjects(bool soft = false);
    void CreateVoxelObject(int count);
    void UseAlphaColor(bool use);

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;


    void UpdateMvpMatrix();

private:
    ShaderProgram* m_voxelShader;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projMatrix;
    QMatrix4x4 mvpMatrix;

    VoxelObject* voxelObject;
    GridObject* gridObject;
    WcsObject* wcsObject;

    QVector4D backColor{0.1f, 0.15f, 0.3f, 0.5f};

    struct Camera
    {
        float zoom = -5;
        float xAngle = -30;
        float zAngle = 30;
        QVector3D pos{0, 0, 0};
    } m_camera;
    struct MouseState
    {
        QPoint pos;
        QMap<Qt::MouseButton, bool> pressed;
    } m_mouseState;
    bool _shiftPressed;
};

#endif // SCENEVIEW_H
