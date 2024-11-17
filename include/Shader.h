// Shader.h

#pragma once

#include <string>
#include <unordered_map>
#include "glad/glad.h"

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void use() const;
    GLuint getID() const;

private:
    GLuint ID;
    std::unordered_map<std::string, GLint> uniformLocations;

    std::string loadShaderSource(const std::string& filepath);
    GLuint compileShader(const std::string& source, GLenum type);
};

