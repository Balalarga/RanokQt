#ifndef OPENGLSYSTEM_H
#define OPENGLSYSTEM_H

#include "GL/glew.h"
#include "GL/gl.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

// Обертка над Opengl для удобства использования
class OpenglSystem
{
public:
    void Init();
    void SetProjection(sf::Vector2i windowSize, float fov = 45.f, float zNear = 0.1, float zFar = 100, const float *matrix = nullptr);
    void SetViewport(const sf::IntRect& viewport);

    static void Destroy();
    static OpenglSystem& Instance();

private:
    static OpenglSystem* s_instance;

    OpenglSystem() = default;
    OpenglSystem(const OpenglSystem&) = delete;
    OpenglSystem& operator=(const OpenglSystem&) = delete;
};

#endif // OPENGLSYSTEM_H
