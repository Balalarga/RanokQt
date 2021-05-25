#include "OpenglSystem.h"

OpenglSystem* OpenglSystem::s_instance = nullptr;

void OpenglSystem::Init()
{
    GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
    GLfloat LightAmbient[] = {0.25f, 0.25f, 0.25f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
    GLfloat LightDiffuse[] = {0.75f, 0.75f, 0.75f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
    GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);
    GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
    float LightPosition[] = {0.0f, 0.0f, 10.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenglSystem::SetProjection(sf::Vector2i windowSize, float fov, float zNear, float zFar, const float* matrix)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, (float)windowSize.x / (windowSize.y),
                   zNear, zFar);
}

void OpenglSystem::SetViewport(const sf::IntRect &viewport)
{
    glViewport(viewport.left, viewport.top, viewport.width, viewport.height);
    SetProjection({viewport.width, viewport.height});
}

void OpenglSystem::Destroy()
{
    if(s_instance)
    {
        delete s_instance;
        s_instance = nullptr;
    }
}

OpenglSystem &OpenglSystem::Instance()
{
    if(!s_instance)
        s_instance = new OpenglSystem;
    return *s_instance;
}
