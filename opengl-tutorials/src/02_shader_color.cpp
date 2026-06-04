// 顶点着色器输出颜色，片元插值 — 理解 varying

#include "gfx_window.h"
#include "gl_loader.h"
#include "shader_util.h"

#include <GLFW/glfw3.h>

namespace {
const char* kVert = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 vColor;
void main() {
    gl_Position = vec4(aPos, 1.0);
    vColor = aColor;
}
)";

const char* kFrag = R"(#version 330 core
in vec3 vColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(vColor, 1.0);
}
)";
}  // namespace

int main() {
    GLFWwindow* window = createRenderWindow(800, 600, "02_shader_color");
    if (!window) return -1;

    const unsigned int program = createShaderProgram(kVert, kFrag);

    float vertices[] = {
        -0.6f, -0.4f, 0.0f, 1.0f, 0.2f, 0.2f,
         0.6f, -0.4f, 0.0f, 0.2f, 1.0f, 0.3f,
         0.0f,  0.6f, 0.0f, 0.2f, 0.4f, 1.0f,
    };

    unsigned int vao = 0, vbo = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!windowShouldClose(window)) {
        pollWindow(window);
        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
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
