#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics/RenderWindow.hpp>
#include "imgui-SFML.h"
#include "imgui.h"
#include <functional>

class Menu
{
public:
    Menu(sf::RenderWindow &window, std::function<void()>& menuFunction);
    ~Menu();

    void Render(sf::Time deltaTime);
    void HandleEvent(sf::Event& event);
    void SetFunction(std::function<void()>& menuFunction);

private:
    sf::RenderWindow& m_window;
    std::function<void()>& m_menuFunction;
};

#endif // MENU_H
