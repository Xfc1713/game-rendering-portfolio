// 纹理采样 + uniform

#include "gfx_window.h"
#include "gl_loader.h"
#include "shader_util.h"

#include <GLFW/glfw3.h>

#include <vector>

namespace {
const char* kVert = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
out vec2 vUV;
void main() {
    gl_Position = vec4(aPos, 1.0);
    vUV = aUV;
}
)";

const char* kFrag = R"(#version 330 core
in vec2 vUV;
out vec4 FragColor;
uniform sampler2D uTexture;
void main() {
    FragColor = texture(uTexture, vUV);
}
)";

unsigned int createCheckerTexture(int size) {
    std::vector<unsigned char> pixels(static_cast<size_t>(size * size * 3));
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            const bool checker = ((x / 16) + (y / 16)) % 2 == 0;
            const unsigned char c = checker ? 220 : 40;
            const size_t i = static_cast<size_t>((y * size + x) * 3);
            pixels[i] = c;
            pixels[i + 1] = checker ? 80 : 180;
            pixels[i + 2] = checker ? 80 : 220;
        }
    }
    unsigned int tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return tex;
}
}  // namespace

int main() {
    GLFWwindow* window = createRenderWindow(800, 600, "03_textured_quad");
    if (!window) return -1;

    const unsigned int program = createShaderProgram(kVert, kFrag);
    const unsigned int texture = createCheckerTexture(256);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 2.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 2.0f, 2.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 2.0f,
    };
    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    unsigned int vao = 0, vbo = 0, ebo = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!windowShouldClose(window)) {
        pollWindow(window);
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(program, "uTexture"), 0);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glfwSwapBuffers(window);
    }

    glDeleteTextures(1, &texture);
    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    destroyWindow(window);
    return 0;
}
