// Window.h

#pragma once

#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    void makeContextCurrent();
    bool shouldClose();
    void swapBuffers();
    void pollEvents();
    void setFramebufferSizeCallback(GLFWframebuffersizefun callback);
    GLFWwindow* getGLFWwindow() const;

private:
    GLFWwindow* window;
};
