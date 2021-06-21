#include "sceneview.h"

#include <GL/glu.h>

SceneView::SceneView(QWidget *parent):
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    m_frameTimer(new QTimer(this))
{
    m_frameTimer->setInterval(20);
    connect(m_frameTimer, &QTimer::timeout, this, &QGLWidget::updateGL);
    m_frameTimer->start();
}

SceneView::~SceneView()
{
}

void SceneView::AddObject(OpenglObject *obj)
{
    m_objects.PushBack(obj);
}

void SceneView::ClearObjects()
{
    m_objects.Clear();
}

void SceneView::initializeGL()
{
    qglClearColor(qRgb(20, 20, 20));


    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glShadeModel(GL_SMOOTH);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void SceneView::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.f, (float)width / height,
                   0.1f, 100.f);
}

void SceneView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, m_camera.zoom,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);
    glRotatef(m_camera.xAngle-90, 1, 0, 0);
    glRotatef(m_camera.zAngle, 0, 0, 1);

    glBegin(GL_LINES);
    // grid
    qglColor(qRgba(40, 40, 40, 150));
    for(int i = -100; i < 100; i++)
    {
        glVertex3i(i, -100, 0);
        glVertex3i(i,  100, 0);
        glVertex3i(-100, i, 0);
        glVertex3i( 100, i, 0);
    }
    glEnd();

    for(int i = 0; i < m_objects.Size(); i++)
        m_objects.At(i)->Render();

    glBegin(GL_LINES);
    // x
    glColor3f(1.0f, 0.0f, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.5, 0.0, 0.0);

    // y
    glColor3f(0.0f, 1.0f, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.5, 0.0);

    // z
    glColor3f(0.0f, 0.0f, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.5);
    glEnd();


    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, 10, 10, 0.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_CULL_FACE);

    glClear(GL_DEPTH_BUFFER_BIT);
    glTranslated(9, 1, 0);
    glRotatef(m_camera.xAngle-90, 1, 0, 0);
    glRotatef(m_camera.zAngle, 0, 0, 1);

    glBegin(GL_LINES);
        // x
        glColor3f(1.0f, 0.0f, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(-0.5, 0.0, 0.0);

        // y
        glColor3f(0.0f, 1.0f, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, -0.5, 0.0);

        // z
        glColor3f(0.0f, 0.0f, 1.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, -0.5);
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void SceneView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_mouseState.pos = event->pos();
        m_mouseState.pressed = true;
    }
}

void SceneView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_mouseState.pressed = false;
    }
}

void SceneView::mouseMoveEvent(QMouseEvent *event)
{
    if(m_mouseState.pressed)
    {
        m_camera.xAngle += (m_mouseState.pos.y() - event->pos().y())*0.5;
        m_camera.zAngle += (event->pos().x() - m_mouseState.pos.x())*0.5;
        m_mouseState.pos = {event->pos().x(), event->pos().y()};
    }
}

void SceneView::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0 && m_camera.zoom < -0.5f)
        m_camera.zoom += 0.4f;
    else if(event->angleDelta().y() < 0)
        m_camera.zoom -= 0.4f;
}
