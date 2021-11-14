#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include "Base/OpenglWidget.h"
#include "GridObject.h"
#include "VoxelObject.h"
#include "LinesObject.h"
#include "WcsObject.h"


class SceneView : public OpenglWidget
{
    Q_OBJECT
public:
    explicit SceneView(QWidget *parent = nullptr);


public slots:
    void AddVoxelObject(float x, float y, float z,
                   float r, float g, float b, float a);
    void Flush();
    void ClearObjects(bool soft = false);
    void CreateVoxelObject(int count);
    void SetModelCube(const QVector3D& start, QVector3D& end);


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
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projMatrix;
    QMatrix4x4 mvpMatrix;

    VoxelObject* voxelObject;
    GridObject* gridObject;
    WcsObject* wcsObject;

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
};

#endif // SCENEVIEW_H
