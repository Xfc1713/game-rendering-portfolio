#pragma once

struct GLFWwindow;

// 创建 3.3 Core 上下文窗口并加载 OpenGL 函数
GLFWwindow* createRenderWindow(int width, int height, const char* title);
void pollWindow(GLFWwindow* window);
bool windowShouldClose(GLFWwindow* window);
void destroyWindow(GLFWwindow* window);
