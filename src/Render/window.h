#ifndef WINDOW_H
#define WINDOW_H

#include "glInclude.h"
#include "cube.h"
#include <string>
#include <vector>
#include <mutex>

struct WindowConfig
{
    std::string name = "Window";
    int width = 800, height = 600;
    int x = SDL_WINDOWPOS_CENTERED;
    int y = SDL_WINDOWPOS_CENTERED;
};

class Window
{
public:
    Window(WindowConfig config = {});
    ~Window();
    void AddCube(glm::vec3 pos, float size, glm::vec4 color);
    void AddCube(glm::vec3 pos, glm::vec3 size, glm::vec4 color);
    void Render();
    void HandleEvent();
    void SetZoom(float z);

    bool isOpen = true;

private:
    std::mutex mtx;
    float zoom = 20;
    float yAngle = 135.f;
    SDL_Window* window;
    SDL_GLContext context;
    std::vector<Cube> cubes;

};

#endif // WINDOW_H
