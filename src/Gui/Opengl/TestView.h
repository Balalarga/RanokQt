#ifndef TESTVIEW_H
#define TESTVIEW_H


#include <QGLWidget>
#include <QOpenGLFunctions>
#include <QThread>
#include <QVector>
#include <QVector4D>
#include <QMap>

#include "ShaderProgram.h"
#include "VoxelObject.h"

class TestView : public QGLWidget
{
    Q_OBJECT
public:

    explicit TestView(QWidget *parent = nullptr);
    ~TestView();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void UpdateMvpMatrix();

    void CreateVao();

private:
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projMatrix;
    QMatrix4x4 mvpMatrix;

    ShaderProgram* m_voxelShader;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;

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

#endif // TESTVIEW_H
