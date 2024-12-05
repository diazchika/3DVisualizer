#pragma once
#include <string>
#include <GL/glew.h>
#include <vector>

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();
    bool init();
    void updateFFTTexture(const std::vector<float>& fftData);
    void render(float time);

private:
    int m_width;
    int m_height;
    GLuint m_program;
    GLuint m_vao, m_vbo;
    GLuint m_fftTexture;

    GLuint loadShader(const std::string& path, GLenum type);
    GLuint createProgram(GLuint vertShader, GLuint fragShader);
};
