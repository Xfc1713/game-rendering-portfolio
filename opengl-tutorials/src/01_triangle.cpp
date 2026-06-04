// LearnOpenGL 第 1 步：窗口 + 橙色三角形（VAO/VBO/EBO）

#include "gfx_window.h"
#include "gl_loader.h"
#include "shader_util.h"

#include <GLFW/glfw3.h>

namespace {
const char* kVert = R"(#version 330 core
layout (location = 0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos, 1.0);
}
)";

const char* kFrag = R"(#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)";
}  // namespace

int main() {
    GLFWwindow* window = createRenderWindow(800, 600, "01_triangle");
    if (!window) return -1;

    const unsigned int program = createShaderProgram(kVert, kFrag);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };
    unsigned int vao = 0, vbo = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    while (!windowShouldClose(window)) {
        pollWindow(window);
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
    }

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    destroyWindow(window);
    return 0;
}
