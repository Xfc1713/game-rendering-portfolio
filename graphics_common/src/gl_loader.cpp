#include "gl_loader.h"

#include <GLFW/glfw3.h>

#include <iostream>

#define LOAD_GL(name) \
    name = reinterpret_cast<decltype(name)>(glfwGetProcAddress(#name)); \
    if (!name) { \
        std::cerr << "Failed to load " #name "\n"; \
        return false; \
    }

unsigned int GL_ARRAY_BUFFER = 0x8892;
unsigned int GL_ELEMENT_ARRAY_BUFFER = 0x8893;
unsigned int GL_STATIC_DRAW = 0x88E4;
unsigned int GL_FLOAT = 0x1406;
unsigned int GL_FALSE = 0;
unsigned int GL_TRIANGLES = 0x0004;
unsigned int GL_COLOR_BUFFER_BIT = 0x00004000;
unsigned int GL_DEPTH_BUFFER_BIT = 0x00000100;
unsigned int GL_DEPTH_TEST = 0x0B71;
unsigned int GL_TEXTURE0 = 0x84C0;
unsigned int GL_TEXTURE_2D = 0x0DE1;
unsigned int GL_RGB = 0x1907;
unsigned int GL_RGBA = 0x1908;
unsigned int GL_UNSIGNED_BYTE = 0x1401;
unsigned int GL_UNSIGNED_INT = 0x1405;
unsigned int GL_TEXTURE_WRAP_S = 0x2802;
unsigned int GL_TEXTURE_WRAP_T = 0x2803;
unsigned int GL_REPEAT = 0x2901;
unsigned int GL_TEXTURE_MIN_FILTER = 0x2801;
unsigned int GL_TEXTURE_MAG_FILTER = 0x2800;
unsigned int GL_LINEAR = 0x2601;
unsigned int GL_LINEAR_MIPMAP_LINEAR = 0x2703;
unsigned int GL_FRONT = 0x0404;
unsigned int GL_BACK = 0x0405;
unsigned int GL_CULL_FACE = 0x0B44;
unsigned int GL_LESS = 0x0201;
unsigned int GL_FRAMEBUFFER = 0x8D40;
unsigned int GL_DEPTH_ATTACHMENT = 0x8D00;
unsigned int GL_COLOR_ATTACHMENT0 = 0x8CE0;
unsigned int GL_FRAMEBUFFER_COMPLETE = 0x8CD5;
unsigned int GL_NONE = 0;
unsigned int GL_NEAREST = 0x2600;
unsigned int GL_CLAMP_TO_EDGE = 0x812F;
unsigned int GL_DEPTH_COMPONENT = 0x1902;
unsigned int GL_FLOAT_TYPE = 0x1406;
unsigned int GL_VERTEX_SHADER = 0x8B31;
unsigned int GL_FRAGMENT_SHADER = 0x8B30;

void (*glClear)(unsigned int) = nullptr;
void (*glDrawBuffer)(unsigned int) = nullptr;
void (*glReadBuffer)(unsigned int) = nullptr;
void (*glClearColor)(float, float, float, float) = nullptr;
void (*glViewport)(int, int, int, int) = nullptr;
void (*glEnable)(unsigned int) = nullptr;
void (*glDepthFunc)(unsigned int) = nullptr;
void (*glCullFace)(unsigned int) = nullptr;
void (*glGenVertexArrays)(int, unsigned int*) = nullptr;
void (*glBindVertexArray)(unsigned int) = nullptr;
void (*glGenBuffers)(int, unsigned int*) = nullptr;
void (*glBindBuffer)(unsigned int, unsigned int) = nullptr;
void (*glBufferData)(unsigned int, GLsizeiptr, const void*, unsigned int) = nullptr;
void (*glVertexAttribPointer)(unsigned int, int, unsigned int, unsigned char, int, const void*) = nullptr;
void (*glEnableVertexAttribArray)(unsigned int) = nullptr;
void (*glDrawArrays)(unsigned int, int, int) = nullptr;
void (*glDrawElements)(unsigned int, int, unsigned int, const void*) = nullptr;
unsigned int (*glCreateShader)(unsigned int) = nullptr;
void (*glShaderSource)(unsigned int, int, const char* const*, const int*) = nullptr;
void (*glCompileShader)(unsigned int) = nullptr;
void (*glGetShaderiv)(unsigned int, unsigned int, int*) = nullptr;
void (*glGetShaderInfoLog)(unsigned int, int, int*, char*) = nullptr;
unsigned int (*glCreateProgram)() = nullptr;
void (*glAttachShader)(unsigned int, unsigned int) = nullptr;
void (*glLinkProgram)(unsigned int) = nullptr;
void (*glGetProgramiv)(unsigned int, unsigned int, int*) = nullptr;
void (*glGetProgramInfoLog)(unsigned int, int, int*, char*) = nullptr;
void (*glUseProgram)(unsigned int) = nullptr;
void (*glDeleteShader)(unsigned int) = nullptr;
int (*glGetUniformLocation)(unsigned int, const char*) = nullptr;
void (*glUniform1i)(int, int) = nullptr;
void (*glUniform1f)(int, float) = nullptr;
void (*glUniform3f)(int, float, float, float) = nullptr;
void (*glUniformMatrix4fv)(int, int, unsigned char, const float*) = nullptr;
void (*glGenTextures)(int, unsigned int*) = nullptr;
void (*glBindTexture)(unsigned int, unsigned int) = nullptr;
void (*glTexImage2D)(unsigned int, int, int, int, int, int, unsigned int, unsigned int, const void*) = nullptr;
void (*glTexParameteri)(unsigned int, unsigned int, int) = nullptr;
void (*glActiveTexture)(unsigned int) = nullptr;
void (*glGenerateMipmap)(unsigned int) = nullptr;
void (*glDeleteVertexArrays)(int, const unsigned int*) = nullptr;
void (*glDeleteBuffers)(int, const unsigned int*) = nullptr;
void (*glDeleteProgram)(unsigned int) = nullptr;
void (*glDeleteTextures)(int, const unsigned int*) = nullptr;
void (*glGenFramebuffers)(int, unsigned int*) = nullptr;
void (*glBindFramebuffer)(unsigned int, unsigned int) = nullptr;
void (*glFramebufferTexture2D)(unsigned int, unsigned int, unsigned int, unsigned int, int) = nullptr;
unsigned int (*glCheckFramebufferStatus)(unsigned int) = nullptr;

bool loadOpenGL33Core() {
    LOAD_GL(glClear);
    LOAD_GL(glDrawBuffer);
    LOAD_GL(glReadBuffer);
    LOAD_GL(glClearColor);
    LOAD_GL(glViewport);
    LOAD_GL(glEnable);
    LOAD_GL(glDepthFunc);
    LOAD_GL(glCullFace);
    LOAD_GL(glGenVertexArrays);
    LOAD_GL(glBindVertexArray);
    LOAD_GL(glGenBuffers);
    LOAD_GL(glBindBuffer);
    LOAD_GL(glBufferData);
    LOAD_GL(glVertexAttribPointer);
    LOAD_GL(glEnableVertexAttribArray);
    LOAD_GL(glDrawArrays);
    LOAD_GL(glDrawElements);
    LOAD_GL(glCreateShader);
    LOAD_GL(glShaderSource);
    LOAD_GL(glCompileShader);
    LOAD_GL(glGetShaderiv);
    LOAD_GL(glGetShaderInfoLog);
    LOAD_GL(glCreateProgram);
    LOAD_GL(glAttachShader);
    LOAD_GL(glLinkProgram);
    LOAD_GL(glGetProgramiv);
    LOAD_GL(glGetProgramInfoLog);
    LOAD_GL(glUseProgram);
    LOAD_GL(glDeleteShader);
    LOAD_GL(glGetUniformLocation);
    LOAD_GL(glUniform1i);
    LOAD_GL(glUniform1f);
    LOAD_GL(glUniform3f);
    LOAD_GL(glUniformMatrix4fv);
    LOAD_GL(glGenTextures);
    LOAD_GL(glBindTexture);
    LOAD_GL(glTexImage2D);
    LOAD_GL(glTexParameteri);
    LOAD_GL(glActiveTexture);
    LOAD_GL(glGenerateMipmap);
    LOAD_GL(glDeleteVertexArrays);
    LOAD_GL(glDeleteBuffers);
    LOAD_GL(glDeleteProgram);
    LOAD_GL(glDeleteTextures);
    LOAD_GL(glGenFramebuffers);
    LOAD_GL(glBindFramebuffer);
    LOAD_GL(glFramebufferTexture2D);
    LOAD_GL(glCheckFramebufferStatus);
    return true;
}
