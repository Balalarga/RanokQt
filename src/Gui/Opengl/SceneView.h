#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <QGLWidget>
#include <QVector>
#include <QTimer>

#include "AsyncVector.h"
#include "OpenglObject.h"

#include <pthread.h>


class SceneView : public QGLWidget
{
    Q_OBJECT
public:
    explicit SceneView(QWidget *parent = nullptr);
    ~SceneView();

public slots:
    void AddObject(OpenglObject* obj);
    void ClearObjects();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QTimer* m_frameTimer;

    AsyncVector<OpenglObject*> m_objects;

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
