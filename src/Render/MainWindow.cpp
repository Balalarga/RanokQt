#include "MainWindow.h"

MainWindow::MainWindow(sf::Vector2i windowSize)
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;
    m_window = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y),
                                    "OpenGL", sf::Style::Default, settings);
    m_window->setFramerateLimit(60);
    OpenglSystem::Instance().SetProjection(windowSize);
}

MainWindow::~MainWindow()
{
    ClearObjects();
    delete m_window;
}

void MainWindow::Show()
{
    sf::Clock deltaClock;
    while (m_window->isOpen())
    {
        Render();

        if(m_menu)
            m_menu->Render(deltaClock.restart());
        m_window->display();

        HandleEvent();
    }
}

void MainWindow::CreateMenu(std::function<void ()> menuFunction)
{
    if(!m_menu)
        m_menu = new Menu(*m_window, menuFunction);
    else
        m_menu->SetFunction(menuFunction);
}

void MainWindow::AddObject(IDrawableObject* object)
{
    m_objects.PushBack(object);
}

void MainWindow::ClearObjects()
{
    for(unsigned i = 0; i < m_objects.Size(); i++)
        delete m_objects.At(i);
    m_objects.Clear();
}

void MainWindow::HandleEvent()
{
    sf::Event event;
    while (m_window->pollEvent(event))
    {
        if(m_menu)
            m_menu->HandleEvent(event);
        if (event.type == sf::Event::Closed)
        {
            m_window->close();
        }
        else if (event.type == sf::Event::Resized)
        {
            OpenglSystem::Instance().SetViewport({0, 0,
                                                  static_cast<int>(event.size.width),
                                                  static_cast<int>(event.size.height)});
        }
        else if(event.type == sf::Event::MouseButtonPressed)
        {
            m_mouse.pressed = true;
            m_mouse.pos = {event.mouseButton.x, event.mouseButton.y};
        }
        else if(event.type == sf::Event::MouseButtonReleased)
        {
            m_mouse.pressed = false;
        }
        else if(m_mouse.pressed && event.type == sf::Event::MouseMoved)
        {
            m_camera.xAngle += (m_mouse.pos.y - event.mouseMove.y)*0.5;
            m_camera.yAngle += (event.mouseMove.x - m_mouse.pos.x)*0.5;
            m_mouse.pos = {event.mouseMove.x, event.mouseMove.y};
        }
        else if(event.type == sf::Event::MouseWheelScrolled)
        {
            m_camera.zoom -= event.mouseWheelScroll.delta*0.2;
        }
    }
}

void MainWindow::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, -m_camera.zoom,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);
    glRotatef(m_camera.xAngle, 1, 0, 0);
    glRotatef(m_camera.yAngle, 0, 1, 0);
    for(unsigned i = 0; i < m_objects.Size(); i++)
    {
        m_objects.Get(i)->Draw();
    }
}
