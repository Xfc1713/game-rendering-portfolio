#pragma once

// 图形学常用二维/三维向量（第 1 周数学与 C++ 结合练习）

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;
};

struct Vec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

float dot(const Vec3& a, const Vec3& b);
Vec3 cross(const Vec3& a, const Vec3& b);
Vec3 normalize(const Vec3& v);
float length(const Vec3& v);
