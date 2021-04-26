#include "window.h"

using namespace std;

Window::Window(WindowConfig config)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(config.name.c_str(),
                              config.x, config.y,
                              config.width, config.height,
                              SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext(window);

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

    glEnable(GL_DEPTH_TEST);

    glDisable(GL_CULL_FACE);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)800 / (600> 0 ? (float)600: 1.0f), 0.125f, 512.0f);

    float LightPosition[] = {0.0f, 2.5f, 2.5f};
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

Window::~Window()
{
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::AddCube(glm::vec3 pos, float size, glm::vec4 color)
{
    mtx.lock();
    cubes.push_back(Cube(pos, size, color));
    mtx.unlock();
}

void Window::AddCube(glm::vec3 pos, glm::vec3 size, glm::vec4 color)
{
    mtx.lock();
    cubes.push_back(Cube(pos, size, color));
    mtx.unlock();
}

void Window::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, zoom, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glRotatef(yAngle, 0.0f, 1.0f, 0.0f);

    glBegin(GL_LINES);
        glColor3d(0., 0., 1.);
        glVertex3d(0, 0, 0);
        glVertex3d(1, 0, 0);

        glColor3d(1., 0., 0.);
        glVertex3d(0, 0, 0);
        glVertex3d(0, 1, 0);

        glColor3d(0., 1., 0.);
        glVertex3d(0, 0, 0);
        glVertex3d(0, 0, 1);

    glEnd();

    mtx.lock();
    glBegin(GL_QUADS);

    for(auto& i: cubes)
        i.Draw();

    glEnd();
    mtx.unlock();

    SDL_GL_SwapWindow(window);
}

void Window::HandleEvent()
{
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT)
            isOpen = false;
        else if(e.type == SDL_KEYDOWN)
        {
            if(e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                isOpen = false;
            else if(e.key.keysym.scancode == SDL_SCANCODE_S)
            {
                zoom += 0.2;
            }
            else if(e.key.keysym.scancode == SDL_SCANCODE_W)
            {
                zoom -= 0.2;
            }
            else if(e.key.keysym.scancode == SDL_SCANCODE_A)
            {
                yAngle-=2;
            }
            else if(e.key.keysym.scancode == SDL_SCANCODE_D)
            {
                yAngle+=2;
            }
        }
    }
}

void Window::SetZoom(float z)
{
    zoom = z;
}
