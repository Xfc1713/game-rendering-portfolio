#include "raii_demo.h"
#include "vector_math.h"

#include <iostream>
#include <vector>

int main() {
    std::cout << "=== C++ 工程化基础 Demo（第 1 周）===\n\n";

    // 向量运算 — 对应光照中的 N·L、法线叉积等
    const Vec3 lightDir = normalize({0.3f, 1.0f, 0.2f});
    const Vec3 normal = normalize({0.0f, 1.0f, 0.0f});
    const float ndotl = std::max(0.0f, dot(normal, lightDir));
    std::cout << "[Math] Lambert N·L = " << ndotl << "\n";

    const Vec3 tangent = cross(normal, {1.0f, 0.0f, 0.0f});
    std::cout << "[Math] 切线方向长度 = " << length(tangent) << "\n\n";

    // STL 与 const 正确性
    std::vector<int> indices = {0, 1, 2, 0, 2, 3};
    const auto& cref = indices;
    std::cout << "[STL] 索引数量: " << cref.size() << "\n\n";

    run_raii_demo();

    std::cout << "\n完成。请继续阅读 docs/graphics-math-notes.md 与 opengl-tutorials。\n";
    return 0;
}
