// Shader.cpp
#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <GLFW/glfw3.h>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode = loadShaderSource(vertexPath);
    std::string fragmentCode = loadShaderSource(fragmentPath);

    GLuint vertexShader = compileShader(vertexCode, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentCode, GL_FRAGMENT_SHADER);

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    GLint success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::string errorMsg = "Shader program linking failed:\n" + std::string(infoLog);
        throw std::runtime_error(errorMsg);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

void Shader::use() const {
    glUseProgram(ID);
}

GLuint Shader::getID() const {
    return ID;
}

std::string Shader::loadShaderSource(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader file: " + filepath);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint Shader::compileShader(const std::string& source, GLenum type) {
    std::cout << "glCreateShader address: " << (void*)glCreateShader << std::endl;
    GLuint shader = glCreateShader(type);
    const GLchar* code = source.c_str();
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::string shaderType = (type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
        std::string errorMsg = shaderType + " shader compilation failed:\n" + std::string(infoLog);
        throw std::runtime_error(errorMsg);
    }

    return shader;
}