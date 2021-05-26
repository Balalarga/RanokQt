#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "OpenglSystem.h"
#include "IDrawableObject.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "AsyncVector.h"
#include "Menu.h"

// Главное окно программы
// Содержит в себе opengl пространство, объекты сцены, меню, камеру и обработку ввода
class MainWindow
{
public:
    MainWindow(sf::Vector2i windowSize = {1280, 720});
    ~MainWindow();

    void Show();

    // Добавление меню
    void CreateMenu(std::function<void()> menuFunction);

    // Добавление объекта на сцену
    void AddObject(IDrawableObject *object);
    void ClearObjects();

private:
    void HandleEvent();
    void Render();

    sf::RenderWindow* m_window = nullptr;
    Menu* m_menu = nullptr;

    // Массив объектов сцены
    AsyncVector<IDrawableObject*> m_objects;

    struct Camera
    {
        float zoom = 5;
        float xAngle = 0;
        float yAngle = 0;
    } m_camera;

    struct Mouse
    {
        sf::Vector2i pos;
        bool pressed = false;
    } m_mouse;
};

#endif // MAINWINDOW_H
