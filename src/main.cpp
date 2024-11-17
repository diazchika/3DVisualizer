// main.cpp

#include "Window.h"
#include "Renderer.h"
#include <iostream>

// Callback function for framebuffer size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Process input
void processInput(Window& window) {
    GLFWwindow* glfwWindow = window.getGLFWwindow();
    if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(glfwWindow, true);
}

int main() {
    try {
        Window window(800, 600, "3DVisualizer");
        window.makeContextCurrent();
        window.setFramebufferSizeCallback(framebuffer_size_callback);

        Renderer renderer(window);

        // Render loop
        while (!window.shouldClose()) {
            // Input
            processInput(window);

            // Render
            renderer.render();

            // Swap buffers and poll events
            window.swapBuffers();
            window.pollEvents();
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
