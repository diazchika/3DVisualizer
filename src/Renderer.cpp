#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <GL/glew.h>
#include <SDL2/SDL.h>

Renderer::Renderer(int width, int height)
: m_width(width), m_height(height), m_program(0), m_vao(0), m_vbo(0), m_fftTexture(0)
{}

Renderer::~Renderer() {
    if (m_fftTexture) glDeleteTextures(1, &m_fftTexture);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_program) glDeleteProgram(m_program);
    SDL_Quit();
}

bool Renderer::init() {
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_Window* window = SDL_CreateWindow("3DVisualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "SDL_CreateWindow failed\n";
        return false;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW init failed\n";
        return false;
    }

    // Load shaders
    GLuint vs = loadShader("assets/audio_vert.glsl", GL_VERTEX_SHADER);
    GLuint fs = loadShader("assets/audio_frag.glsl", GL_FRAGMENT_SHADER);
    GLint success;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        std::cout << "OK" << std::endl;
    }

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        std::cout << "OK" << std::endl;
    }

    m_program = createProgram(vs, fs);
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(m_program, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    // Setup a full-screen quad
    float vertices[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f
    };

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLint posLoc = glGetAttribLocation(m_program, "aPos");
    glEnableVertexAttribArray(posLoc);
    glVertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Create texture for FFT data
    glGenTextures(1, &m_fftTexture);
    glBindTexture(GL_TEXTURE_2D, m_fftTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 512, 1, 0, GL_RED, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void Renderer::updateFFTTexture(const std::vector<float>& fftData) {
    // Assume fftData.size() <= 512
    glBindTexture(GL_TEXTURE_2D, m_fftTexture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei)fftData.size(), 1, GL_RED, GL_FLOAT, fftData.data());
}

void Renderer::render(float time) {
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_program);
    
    // Set uniforms if needed
    GLint timeLoc = glGetUniformLocation(m_program, "iTime");
    glUniform1f(timeLoc, time);

    GLint resolutionLoc = glGetUniformLocation(m_program, "iResolution");
    glUniform3f(resolutionLoc, (float)m_width, (float)m_height, 1.0f);

    GLint texLoc = glGetUniformLocation(m_program, "iChannel0");
    glUniform1i(texLoc, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_fftTexture);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    SDL_GL_SwapWindow(SDL_GL_GetCurrentWindow());
}

GLuint Renderer::loadShader(const std::string& path, GLenum type) {
    std::ifstream file(path);
    std::stringstream ss;
    ss << file.rdbuf();
    std::string src = ss.str();

    const char* c_str = src.c_str();
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &c_str, NULL);
    glCompileShader(shader);

    // Check compile status
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        std::cerr << "Error compiling shader: " << path << "\n" << log << "\n";
    }

    return shader;
}

GLuint Renderer::createProgram(GLuint vertShader, GLuint fragShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    // Check link status
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(!status) {
        char log[512];
        glGetProgramInfoLog(program, 512, NULL, log);
        std::cerr << "Program link error:\n" << log << "\n";
    }

    return program;
}
