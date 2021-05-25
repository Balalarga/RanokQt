#include "Menu.h"

Menu::Menu(sf::RenderWindow &window, std::function<void()>& menuFunction):
    m_window(window),
    m_menuFunction(menuFunction)
{
    ImGui::SFML::Init(window);
}

Menu::~Menu()
{
    ImGui::SFML::Shutdown();
}

void Menu::Render(sf::Time deltaTime)
{
    m_window.pushGLStates();
    ImGui::SFML::Update(m_window, deltaTime);
    m_menuFunction();
    ImGui::SFML::Render(m_window);
    m_window.popGLStates();
}

void Menu::HandleEvent(sf::Event &event)
{
    ImGui::SFML::ProcessEvent(event);
}

void Menu::SetFunction(std::function<void ()> &menuFunction)
{
    m_menuFunction = menuFunction;
}
