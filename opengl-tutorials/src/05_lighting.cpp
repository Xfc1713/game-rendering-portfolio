// Blinn-Phong 光照（世界空间法线 + 方向光）

#include "gfx_window.h"
#include "gl_loader.h"
#include "shader_util.h"

#include <GLFW/glfw3.h>

#include <cmath>

namespace {
const char* kVert = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
out vec3 vWorldPos;
out vec3 vNormal;
void main() {
    vec4 world = uModel * vec4(aPos, 1.0);
    vWorldPos = world.xyz;
    vNormal = mat3(transpose(inverse(uModel))) * aNormal;
    gl_Position = uProjection * uView * world;
}
)";

const char* kFrag = R"(#version 330 core
in vec3 vWorldPos;
in vec3 vNormal;
out vec4 FragColor;
uniform vec3 uLightDir;
uniform vec3 uViewPos;
uniform vec3 uObjectColor;
void main() {
    vec3 N = normalize(vNormal);
    vec3 L = normalize(-uLightDir);
    vec3 V = normalize(uViewPos - vWorldPos);
    vec3 H = normalize(V + L);
    float diff = max(dot(N, L), 0.0);
    float spec = pow(max(dot(N, H), 0.0), 32.0);
    vec3 ambient = 0.12 * uObjectColor;
    vec3 diffuse = diff * uObjectColor;
    vec3 specular = spec * vec3(0.9);
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
)";

void identity(float* m) {
    for (int i = 0; i < 16; ++i) m[i] = 0.0f;
    m[0] = m[5] = m[10] = m[15] = 1.0f;
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

void lookAt(float eyeZ, float* m) {
    identity(m);
    m[14] = -eyeZ;
}
}  // namespace

int main() {
    GLFWwindow* window = createRenderWindow(900, 600, "05_lighting");
    if (!window) return -1;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    const unsigned int program = createShaderProgram(kVert, kFrag);

    // 单位立方体（位置 + 法线）
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0, 0, -1,
         0.5f, -0.5f, -0.5f, 0, 0, -1,
         0.5f,  0.5f, -0.5f, 0, 0, -1,
        -0.5f,  0.5f, -0.5f, 0, 0, -1,
        -0.5f, -0.5f,  0.5f, 0, 0, 1,
         0.5f, -0.5f,  0.5f, 0, 0, 1,
         0.5f,  0.5f,  0.5f, 0, 0, 1,
        -0.5f,  0.5f,  0.5f, 0, 0, 1,
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

    float model[16], view[16], proj[16];
    identity(model);
    lookAt(3.0f, view);
    perspective(45.0f, 900.0f / 600.0f, 0.1f, 100.0f, proj);

    while (!windowShouldClose(window)) {
        pollWindow(window);
        glClearColor(0.05f, 0.06f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program);
        glUniformMatrix4fv(glGetUniformLocation(program, "uModel"), 1, GL_FALSE, model);
        glUniformMatrix4fv(glGetUniformLocation(program, "uView"), 1, GL_FALSE, view);
        glUniformMatrix4fv(glGetUniformLocation(program, "uProjection"), 1, GL_FALSE, proj);
        glUniform3f(glGetUniformLocation(program, "uLightDir"), 0.3f, 1.0f, 0.2f);
        glUniform3f(glGetUniformLocation(program, "uViewPos"), 0.0f, 0.0f, 3.0f);
        glUniform3f(glGetUniformLocation(program, "uObjectColor"), 0.75f, 0.55f, 0.35f);
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
