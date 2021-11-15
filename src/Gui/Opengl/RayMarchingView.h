#ifndef RAY_MARCHING_VIEW_H
#define RAY_MARCHING_VIEW_H

#include <QOpenGLFramebufferObject>

#include "Base/OpenglDrawableObject.h"
#include "Base/OpenglWidget.h"


class RayMarchingView : public OpenglWidget
{
    Q_OBJECT
public:
    explicit RayMarchingView(QWidget *parent = nullptr, QSize renderSize = QSize(800, 600));
    ~RayMarchingView();

    void SetRenderSize(const QSize& renderSize);

    void ShaderFromSource(const QString &source);


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
    OpenglDrawableObject* _screenRect;
    OpenglDrawableObject* _textureRect;

    const QVector<float> _screenData;
    const QVector<float> _textureData;

    QSize _renderSize;

    QMatrix4x4 viewMatrix;
    QMatrix4x4 projMatrix;
    QMatrix4x4 mvpMatrix;

    QOpenGLFramebufferObject* fbo;

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

#endif // TESTVIEW_H
