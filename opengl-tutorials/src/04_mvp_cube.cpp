// MVP 变换旋转立方体

#include "gfx_window.h"
#include "gl_loader.h"
#include "shader_util.h"

#include <GLFW/glfw3.h>

#include <cmath>

namespace {
const char* kVert = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform mat4 uMVP;
out vec3 vColor;
void main() {
    gl_Position = uMVP * vec4(aPos, 1.0);
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

void multiply(const float* a, const float* b, float* out) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            out[i * 4 + j] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                out[i * 4 + j] += a[i * 4 + k] * b[k * 4 + j];
            }
        }
    }
}

void perspective(float fovyDeg, float aspect, float nearZ, float farZ, float* m) {
    const float f = 1.0f / std::tan(fovyDeg * 0.5f * 3.14159265f / 180.0f);
    for (int i = 0; i < 16; ++i) m[i] = 0.0f;
    m[0] = f / aspect;
    m[5] = f;
    m[10] = (farZ + nearZ) / (nearZ - farZ);
    m[11] = -1.0f;
    m[14] = (2.0f * farZ * nearZ) / (nearZ - farZ);
}

void lookAt(float* m) {
    for (int i = 0; i < 16; ++i) m[i] = 0.0f;
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[10] = 1.0f;
    m[15] = 1.0f;
    m[14] = -3.0f;
}

void rotateY(float angle, float* m) {
    for (int i = 0; i < 16; ++i) m[i] = 0.0f;
    const float c = std::cos(angle);
    const float s = std::sin(angle);
    m[0] = c;
    m[2] = s;
    m[5] = 1.0f;
    m[8] = -s;
    m[10] = c;
    m[15] = 1.0f;
}
}  // namespace

int main() {
    GLFWwindow* window = createRenderWindow(800, 600, "04_mvp_cube");
    if (!window) return -1;

    glEnable(GL_DEPTH_TEST);

    const unsigned int program = createShaderProgram(kVert, kFrag);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 1, 0, 0,
         0.5f, -0.5f, -0.5f, 0, 1, 0,
         0.5f,  0.5f, -0.5f, 0, 0, 1,
        -0.5f,  0.5f, -0.5f, 1, 1, 0,
        -0.5f, -0.5f,  0.5f, 1, 0, 1,
         0.5f, -0.5f,  0.5f, 0, 1, 1,
         0.5f,  0.5f,  0.5f, 1, 1, 1,
        -0.5f,  0.5f,  0.5f, 0.2f, 0.8f, 0.3f,
    };
    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 4, 7, 7, 3, 0,
        1, 5, 6, 6, 2, 1,
        3, 2, 6, 6, 7, 3,
        0, 1, 5, 5, 4, 0,
    };

    unsigned int vao = 0, vbo = 0, ebo = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    float proj[16], view[16], model[16], vp[16], mvp[16];

    while (!windowShouldClose(window)) {
        pollWindow(window);
        const float t = static_cast<float>(glfwGetTime());
        rotateY(t, model);
        lookAt(view);
        perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f, proj);
        multiply(proj, view, vp);
        multiply(vp, model, mvp);

        glClearColor(0.08f, 0.09f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program);
        glUniformMatrix4fv(glGetUniformLocation(program, "uMVP"), 1, GL_FALSE, mvp);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        glfwSwapBuffers(window);
    }

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    destroyWindow(window);
    return 0;
}
