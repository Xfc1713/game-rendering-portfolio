#pragma once

#include <string>

unsigned int compileShaderFromSource(unsigned int type, const char* source);
unsigned int linkProgram(unsigned int vertexShader, unsigned int fragmentShader);
unsigned int createShaderProgram(const char* vertexSrc, const char* fragmentSrc);
