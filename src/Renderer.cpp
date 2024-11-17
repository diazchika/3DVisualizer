// Renderer.cpp

#include "Renderer.h"
#include <stdexcept>

Renderer::Renderer(Window &window)
    : window(window)
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    shader = std::make_unique<Shader>("assets/vertex_shader.glsl", "assets/fragment_shader.glsl");

    initialize();
}

Renderer::~Renderer() {
    cleanup();
}

void Renderer::initialize() {
    // Set viewport
    int width, height;
    glfwGetFramebufferSize(window.getGLFWwindow(), &width, &height);
    glViewport(0, 0, width, height);

    // Enable depth testing if needed
    // glEnable(GL_DEPTH_TEST);

    // Setup the triangle data
    setupTri();
}

void Renderer::setupTri() {
    // Vertex data
    float vertices[] = {
        // positions
         0.0f,  0.5f, 0.0f, // top
        -0.5f, -0.5f, 0.0f, // bottom left
         0.5f, -0.5f, 0.0f  // bottom right
    };

    // Generate buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind and set data
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::render() {
    // Clear the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the triangle
    shader->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::cleanup() {
    // Delete buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}