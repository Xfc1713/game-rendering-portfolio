#pragma once

// 通过 GLFW 加载 OpenGL 3.3 Core 常用函数（避免提交完整 GLAD 生成文件）

using GLProc = void (*)();

bool loadOpenGL33Core();

// 类型与常量（精简集）
using GLsizeiptr = ptrdiff_t;
using GLintptr = ptrdiff_t;

extern unsigned int GL_ARRAY_BUFFER;
extern unsigned int GL_ELEMENT_ARRAY_BUFFER;
extern unsigned int GL_STATIC_DRAW;
extern unsigned int GL_FLOAT;
extern unsigned int GL_FALSE;
extern unsigned int GL_TRIANGLES;
extern unsigned int GL_COLOR_BUFFER_BIT;
extern unsigned int GL_DEPTH_BUFFER_BIT;
extern unsigned int GL_DEPTH_TEST;
extern unsigned int GL_TEXTURE0;
extern unsigned int GL_TEXTURE_2D;
extern unsigned int GL_RGB;
extern unsigned int GL_RGBA;
extern unsigned int GL_UNSIGNED_BYTE;
extern unsigned int GL_UNSIGNED_INT;
extern unsigned int GL_TEXTURE_WRAP_S;
extern unsigned int GL_TEXTURE_WRAP_T;
extern unsigned int GL_REPEAT;
extern unsigned int GL_TEXTURE_MIN_FILTER;
extern unsigned int GL_TEXTURE_MAG_FILTER;
extern unsigned int GL_LINEAR;
extern unsigned int GL_LINEAR_MIPMAP_LINEAR;
extern unsigned int GL_FRONT;
extern unsigned int GL_BACK;
extern unsigned int GL_CULL_FACE;
extern unsigned int GL_LESS;
extern unsigned int GL_FRAMEBUFFER;
extern unsigned int GL_DEPTH_ATTACHMENT;
extern unsigned int GL_COLOR_ATTACHMENT0;
extern unsigned int GL_FRAMEBUFFER_COMPLETE;
extern unsigned int GL_NONE;
extern unsigned int GL_TEXTURE_MIN_FILTER;
extern unsigned int GL_TEXTURE_MAG_FILTER;
extern unsigned int GL_NEAREST;
extern unsigned int GL_CLAMP_TO_EDGE;
extern unsigned int GL_DEPTH_COMPONENT;
extern unsigned int GL_FLOAT_TYPE;
extern unsigned int GL_VERTEX_SHADER;
extern unsigned int GL_FRAGMENT_SHADER;

extern void (*glClear)(unsigned int mask);
extern void (*glDrawBuffer)(unsigned int mode);
extern void (*glReadBuffer)(unsigned int mode);
extern void (*glClearColor)(float r, float g, float b, float a);
extern void (*glViewport)(int x, int y, int width, int height);
extern void (*glEnable)(unsigned int cap);
extern void (*glDepthFunc)(unsigned int func);
extern void (*glCullFace)(unsigned int mode);
extern void (*glGenVertexArrays)(int n, unsigned int* arrays);
extern void (*glBindVertexArray)(unsigned int array);
extern void (*glGenBuffers)(int n, unsigned int* buffers);
extern void (*glBindBuffer)(unsigned int target, unsigned int buffer);
extern void (*glBufferData)(unsigned int target, GLsizeiptr size, const void* data, unsigned int usage);
extern void (*glVertexAttribPointer)(unsigned int index, int size, unsigned int type, unsigned char normalized,
                                     int stride, const void* pointer);
extern void (*glEnableVertexAttribArray)(unsigned int index);
extern void (*glDrawArrays)(unsigned int mode, int first, int count);
extern void (*glDrawElements)(unsigned int mode, int count, unsigned int type, const void* indices);
extern unsigned int (*glCreateShader)(unsigned int type);
extern void (*glShaderSource)(unsigned int shader, int count, const char* const* string, const int* length);
extern void (*glCompileShader)(unsigned int shader);
extern void (*glGetShaderiv)(unsigned int shader, unsigned int pname, int* params);
extern void (*glGetShaderInfoLog)(unsigned int shader, int bufSize, int* length, char* infoLog);
extern unsigned int (*glCreateProgram)();
extern void (*glAttachShader)(unsigned int program, unsigned int shader);
extern void (*glLinkProgram)(unsigned int program);
extern void (*glGetProgramiv)(unsigned int program, unsigned int pname, int* params);
extern void (*glGetProgramInfoLog)(unsigned int program, int bufSize, int* length, char* infoLog);
extern void (*glUseProgram)(unsigned int program);
extern void (*glDeleteShader)(unsigned int shader);
extern int (*glGetUniformLocation)(unsigned int program, const char* name);
extern void (*glUniform1i)(int location, int v0);
extern void (*glUniform1f)(int location, float v0);
extern void (*glUniform3f)(int location, float v0, float v1, float v2);
extern void (*glUniformMatrix4fv)(int location, int count, unsigned char transpose, const float* value);
extern void (*glGenTextures)(int n, unsigned int* textures);
extern void (*glBindTexture)(unsigned int target, unsigned int texture);
extern void (*glTexImage2D)(unsigned int target, int level, int internalformat, int width, int height, int border,
                            unsigned int format, unsigned int type, const void* pixels);
extern void (*glTexParameteri)(unsigned int target, unsigned int pname, int param);
extern void (*glActiveTexture)(unsigned int texture);
extern void (*glGenerateMipmap)(unsigned int target);
extern void (*glDeleteVertexArrays)(int n, const unsigned int* arrays);
extern void (*glDeleteBuffers)(int n, const unsigned int* buffers);
extern void (*glDeleteProgram)(unsigned int program);
extern void (*glDeleteTextures)(int n, const unsigned int* textures);
extern void (*glGenFramebuffers)(int n, unsigned int* framebuffers);
extern void (*glBindFramebuffer)(unsigned int target, unsigned int framebuffer);
extern void (*glFramebufferTexture2D)(unsigned int target, unsigned int attachment, unsigned int textarget,
                                      unsigned int texture, int level);
extern unsigned int (*glCheckFramebufferStatus)(unsigned int target);
