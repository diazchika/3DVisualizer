// Renderer.h

#pragma once

#include "Window.h"
#include "Shader.h"
#include "glad/glad.h"
#include <memory>

class Renderer {
public:
    Renderer(Window &window);
    ~Renderer();

    void initialize();
    void render();
    void cleanup();

private:
    Window &window;
    std::unique_ptr<Shader> shader;

    GLuint VAO, VBO;
    void setupTri();
};

