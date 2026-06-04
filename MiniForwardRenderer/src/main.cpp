// 第 3 周作品：前向渲染 — Blinn-Phong + Shadow Map + 色调映射后处理 + FPS

#include "gfx_window.h"
#include "gl_loader.h"
#include "shader_util.h"

#include <GLFW/glfw3.h>

#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

namespace {

constexpr int kShadowSize = 1024;
constexpr int kScrWidth = 1280;
constexpr int kScrHeight = 720;

const char* kDepthVert = R"(#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 uLightSpace;
uniform mat4 uModel;
void main() {
    gl_Position = uLightSpace * uModel * vec4(aPos, 1.0);
}
)";

const char* kDepthFrag = R"(#version 330 core
void main() { }
)";

const char* kSceneVert = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uLightSpace;
out vec3 vWorldPos;
out vec3 vNormal;
out vec4 vLightSpace;
void main() {
    vec4 world = uModel * vec4(aPos, 1.0);
    vWorldPos = world.xyz;
    vNormal = mat3(transpose(inverse(uModel))) * aNormal;
    vLightSpace = uLightSpace * world;
    gl_Position = uProjection * uView * world;
}
)";

const char* kSceneFrag = R"(#version 330 core
in vec3 vWorldPos;
in vec3 vNormal;
in vec4 vLightSpace;
out vec4 FragColor;
uniform vec3 uLightDir;
uniform vec3 uViewPos;
uniform vec3 uAlbedo;
uniform sampler2D uShadowMap;
uniform float uShadowBias;
float shadowFactor() {
    vec3 proj = vLightSpace.xyz / vLightSpace.w * 0.5 + 0.5;
    if (proj.x < 0.0 || proj.x > 1.0 || proj.y < 0.0 || proj.y > 1.0 || proj.z > 1.0)
        return 1.0;
    float closest = texture(uShadowMap, proj.xy).r;
    float current = proj.z - uShadowBias;
    return current > closest ? 0.35 : 1.0;
}
void main() {
    vec3 N = normalize(vNormal);
    vec3 L = normalize(-uLightDir);
    vec3 V = normalize(uViewPos - vWorldPos);
    vec3 H = normalize(V + L);
    float diff = max(dot(N, L), 0.0);
    float spec = pow(max(dot(N, H), 0.0), 48.0);
    float sh = shadowFactor();
    vec3 ambient = 0.15 * uAlbedo;
    vec3 diffuse = sh * diff * uAlbedo;
    vec3 specular = sh * spec * vec3(0.85);
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
)";

const char* kPostVert = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
out vec2 vUV;
void main() {
    gl_Position = vec4(aPos, 1.0);
    vUV = aUV;
}
)";

const char* kPostFrag = R"(#version 330 core
in vec2 vUV;
out vec4 FragColor;
uniform sampler2D uScene;
void main() {
    vec3 hdr = texture(uScene, vUV).rgb;
    vec3 mapped = hdr / (hdr + vec3(1.0));
    mapped = pow(mapped, vec3(1.0 / 2.2));
    FragColor = vec4(mapped, 1.0);
}
)";

void identity(float* m) {
    for (int i = 0; i < 16; ++i) m[i] = 0.0f;
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

void translate(float x, float y, float z, float* m) {
    identity(m);
    m[12] = x;
    m[13] = y;
    m[14] = z;
}

void scale(float s, float* m) {
    identity(m);
    m[0] = m[5] = m[10] = s;
}

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

void lookAt(float ex, float ey, float ez, float cx, float cy, float cz, float* m) {
    float fx = cx - ex, fy = cy - ey, fz = cz - ez;
    const float fl = std::sqrt(fx * fx + fy * fy + fz * fz);
    fx /= fl;
    fy /= fl;
    fz /= fl;
    float sx = fy * 0.0f - fz * 1.0f;
    float sy = fz * 0.0f - fx * 0.0f;
    float sz = fx * 1.0f - fy * 0.0f;
    const float sl = std::sqrt(sx * sx + sy * sy + sz * sz);
    sx /= sl;
    sy /= sl;
    sz /= sl;
    float ux = sy * fz - sz * fy;
    float uy = sz * fx - sx * fz;
    float uz = sx * fy - sy * fx;
    identity(m);
    m[0] = sx;
    m[1] = ux;
    m[2] = -fx;
    m[4] = sy;
    m[5] = uy;
    m[6] = -fy;
    m[8] = sz;
    m[9] = uz;
    m[10] = -fz;
    m[12] = -(sx * ex + sy * ey + sz * ez);
    m[13] = -(ux * ex + uy * ey + uz * ez);
    m[14] = fx * ex + fy * ey + fz * ez;
}

void ortho(float left, float right, float bottom, float top, float nearZ, float farZ, float* m) {
    identity(m);
    m[0] = 2.0f / (right - left);
    m[5] = 2.0f / (top - bottom);
    m[10] = -2.0f / (farZ - nearZ);
    m[12] = -(right + left) / (right - left);
    m[13] = -(top + bottom) / (top - bottom);
    m[14] = -(farZ + nearZ) / (farZ - nearZ);
}

struct Mesh {
    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int ebo = 0;
    int indexCount = 0;
};

Mesh createCubeMesh() {
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
        0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4,
        0, 4, 7, 7, 3, 0, 1, 5, 6, 6, 2, 1,
        3, 2, 6, 6, 7, 3, 0, 1, 5, 5, 4, 0,
    };
    Mesh mesh;
    mesh.indexCount = 36;
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    return mesh;
}

Mesh createPlaneMesh() {
    float vertices[] = {
        -5.0f, 0.0f, -5.0f, 0, 1, 0,
         5.0f, 0.0f, -5.0f, 0, 1, 0,
         5.0f, 0.0f,  5.0f, 0, 1, 0,
        -5.0f, 0.0f,  5.0f, 0, 1, 0,
    };
    unsigned int indices[] = {0, 1, 2, 2, 3, 0};
    Mesh mesh;
    mesh.indexCount = 6;
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    return mesh;
}

Mesh createScreenQuad() {
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    };
    unsigned int indices[] = {0, 1, 2, 2, 3, 0};
    Mesh mesh;
    mesh.indexCount = 6;
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    return mesh;
}

void drawMesh(const Mesh& mesh) {
    glBindVertexArray(mesh.vao);
    glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, nullptr);
}

unsigned int createDepthTexture(int size) {
    unsigned int tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT_TYPE,
                 nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return tex;
}

unsigned int createColorTexture(int w, int h) {
    unsigned int tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return tex;
}

unsigned int createFboColorDepth(unsigned int colorTex, unsigned int depthTex) {
    unsigned int fbo = 0;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);
    return fbo;
}

unsigned int createFboDepthOnly(unsigned int depthTex) {
    unsigned int fbo = 0;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    return fbo;
}

}  // namespace

int main() {
    GLFWwindow* window = createRenderWindow(kScrWidth, kScrHeight, "MiniForwardRenderer");
    if (!window) return -1;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    const unsigned int depthProgram = createShaderProgram(kDepthVert, kDepthFrag);
    const unsigned int sceneProgram = createShaderProgram(kSceneVert, kSceneFrag);
    const unsigned int postProgram = createShaderProgram(kPostVert, kPostFrag);

    Mesh cube = createCubeMesh();
    Mesh plane = createPlaneMesh();
    Mesh quad = createScreenQuad();

    const unsigned int shadowDepthTex = createDepthTexture(kShadowSize);
    const unsigned int shadowFbo = createFboDepthOnly(shadowDepthTex);

    const unsigned int sceneColorTex = createColorTexture(kScrWidth, kScrHeight);
    const unsigned int sceneDepthTex = createDepthTexture(kScrWidth);
    const unsigned int sceneFbo = createFboColorDepth(sceneColorTex, sceneDepthTex);

    float proj[16], view[16], modelCube[16], modelPlane[16], lightView[16], lightProj[16], lightSpace[16];
    perspective(45.0f, static_cast<float>(kScrWidth) / kScrHeight, 0.1f, 50.0f, proj);
    lookAt(2.5f, 2.0f, 3.5f, 0.0f, 0.0f, 0.0f, view);

    translate(0.0f, 0.5f, 0.0f, modelCube);
    float scaleM[16];
    scale(1.0f, scaleM);
    multiply(modelCube, scaleM, modelCube);

    identity(modelPlane);

    lookAt(-2.0f, 4.0f, 2.0f, 0.0f, 0.0f, 0.0f, lightView);
    ortho(-6.0f, 6.0f, -6.0f, 6.0f, 1.0f, 10.0f, lightProj);
    multiply(lightProj, lightView, lightSpace);

    const float lightDir[3] = {0.3f, 1.0f, 0.2f};
    double lastTime = glfwGetTime();
    int frameCount = 0;
    double fpsAccum = 0.0;

    while (!windowShouldClose(window)) {
        pollWindow(window);
        const double now = glfwGetTime();
        const double dt = now - lastTime;
        lastTime = now;
        fpsAccum += dt;
        ++frameCount;

        if (frameCount >= 30) {
            const double fps = frameCount / fpsAccum;
            char title[128];
            std::snprintf(title, sizeof(title), "MiniForwardRenderer | FPS: %.1f | Pass: Shadow->Scene->Post", fps);
            glfwSetWindowTitle(window, title);
            frameCount = 0;
            fpsAccum = 0.0;
        }

        // Pass 1: Shadow Map
        glViewport(0, 0, kShadowSize, kShadowSize);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFbo);
        glClear(GL_DEPTH_BUFFER_BIT);
        glUseProgram(depthProgram);
        glUniformMatrix4fv(glGetUniformLocation(depthProgram, "uLightSpace"), 1, GL_FALSE, lightSpace);
        glUniformMatrix4fv(glGetUniformLocation(depthProgram, "uModel"), 1, GL_FALSE, modelPlane);
        drawMesh(plane);
        glUniformMatrix4fv(glGetUniformLocation(depthProgram, "uModel"), 1, GL_FALSE, modelCube);
        drawMesh(cube);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        glReadBuffer(GL_COLOR_ATTACHMENT0);

        // Pass 2: Scene to HDR FBO
        glViewport(0, 0, kScrWidth, kScrHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, sceneFbo);
        glClearColor(0.02f, 0.02f, 0.03f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(sceneProgram);
        glUniformMatrix4fv(glGetUniformLocation(sceneProgram, "uView"), 1, GL_FALSE, view);
        glUniformMatrix4fv(glGetUniformLocation(sceneProgram, "uProjection"), 1, GL_FALSE, proj);
        glUniformMatrix4fv(glGetUniformLocation(sceneProgram, "uLightSpace"), 1, GL_FALSE, lightSpace);
        glUniform3f(glGetUniformLocation(sceneProgram, "uLightDir"), lightDir[0], lightDir[1], lightDir[2]);
        glUniform3f(glGetUniformLocation(sceneProgram, "uViewPos"), 2.5f, 2.0f, 3.5f);
        glUniform1f(glGetUniformLocation(sceneProgram, "uShadowBias"), 0.002f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadowDepthTex);
        glUniform1i(glGetUniformLocation(sceneProgram, "uShadowMap"), 0);

        glUniformMatrix4fv(glGetUniformLocation(sceneProgram, "uModel"), 1, GL_FALSE, modelPlane);
        glUniform3f(glGetUniformLocation(sceneProgram, "uAlbedo"), 0.35f, 0.38f, 0.42f);
        drawMesh(plane);

        glUniformMatrix4fv(glGetUniformLocation(sceneProgram, "uModel"), 1, GL_FALSE, modelCube);
        glUniform3f(glGetUniformLocation(sceneProgram, "uAlbedo"), 0.78f, 0.52f, 0.32f);
        drawMesh(cube);

        // Pass 3: Tone mapping to screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glUseProgram(postProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sceneColorTex);
        glUniform1i(glGetUniformLocation(postProgram, "uScene"), 0);
        drawMesh(quad);
        glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(window);
    }

    glDeleteProgram(depthProgram);
    glDeleteProgram(sceneProgram);
    glDeleteProgram(postProgram);
    destroyWindow(window);
    return 0;
}
