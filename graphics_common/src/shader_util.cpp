#include "shader_util.h"

#include "gl_loader.h"

#include <iostream>
#include <vector>

namespace {
constexpr unsigned int kCompileStatus = 0x8B81;
constexpr unsigned int kLinkStatus = 0x8B82;
constexpr unsigned int kVertexShader = 0x8B31;
constexpr unsigned int kFragmentShader = 0x8B30;
constexpr unsigned int kInfoLogLength = 0x8B84;
}  // namespace

unsigned int compileShaderFromSource(unsigned int type, const char* source) {
    const unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success = 0;
    glGetShaderiv(shader, kCompileStatus, &success);
    if (!success) {
        int logLen = 0;
        glGetShaderiv(shader, kInfoLogLength, &logLen);
        std::vector<char> log(static_cast<size_t>(logLen + 1));
        glGetShaderInfoLog(shader, logLen, nullptr, log.data());
        std::cerr << "Shader compile error:\n" << log.data() << "\n";
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

unsigned int linkProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    const unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success = 0;
    glGetProgramiv(program, kLinkStatus, &success);
    if (!success) {
        int logLen = 0;
        glGetProgramiv(program, kInfoLogLength, &logLen);
        std::vector<char> log(static_cast<size_t>(logLen + 1));
        glGetProgramInfoLog(program, logLen, nullptr, log.data());
        std::cerr << "Program link error:\n" << log.data() << "\n";
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

unsigned int createShaderProgram(const char* vertexSrc, const char* fragmentSrc) {
    const unsigned int vs = compileShaderFromSource(kVertexShader, vertexSrc);
    const unsigned int fs = compileShaderFromSource(kFragmentShader, fragmentSrc);
    if (!vs || !fs) {
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
        return 0;
    }
    const unsigned int program = linkProgram(vs, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}
