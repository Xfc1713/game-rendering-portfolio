# 游戏渲染岗面试题速查（第 4 周）

## C++

1. **虚函数表**：多态如何实现？析构函数为何常设为 virtual？
2. **智能指针**：`unique_ptr` vs `shared_ptr` 适用场景；循环引用如何用 `weak_ptr`？
3. **移动语义**：左值/右值引用；移动构造减少何种开销？
4. **const 正确性**：const 成员函数、const 引用参数的意义。

## 图形学

1. **渲染管线**：应用阶段 → 几何 → 光栅 → 片元 → 输出；各阶段输入输出是什么？
2. **MVP**：Model / View / Projection 各自把坐标变到哪里？
3. **法线矩阵**：为何是 Model 矩阵逆转置的 3×3？
4. **深度缓冲**：Z-Fighting 原因与缓解（near/far、polygon offset）。
5. **Shadow Map**：两步流程；Shadow Acne 与 **Bias**；PCF 软阴影思路。
6. **Blinn-Phong**：N、L、V、H 含义；与 Phong 区别。
7. **后处理**：为何需要 FBO；全屏 Quad 如何画。

## 工程与性能

1. **CPU vs GPU 瓶颈**：Draw Call 过多、Overdraw、带宽占用的概念。
2. **Profile 思路**：先看 FPS；区分 CPU 提交 vs GPU 填充（本 Demo 标题 FPS 即入口）。
3. **平台优化**：移动端 tile-based、带宽与精度（了解即可）。

## 算法（发挥 CS 优势）

重点刷 **LeetCode 热题 100** 中：

- 数组：两数之和、最大子数组
- 链表：反转、环检测
- 栈/队列：有效括号
- 二叉树：遍历、最大深度、最近公共祖先
- DFS/BFS：岛屿数量、课程表（拓扑）

详见 [leetcode-plan.md](leetcode-plan.md)。

## 行为面

1. **为何从 Java 后端转向渲染？**  
   模板：对实时图形与性能优化更感兴趣；已通过 OpenGL 项目验证学习路径；希望长期深耕引擎渲染。

2. **最近解决的一个图形 Bug？**  
   模板：Shadow Acne → 调 `uShadowBias`；或 FBO incomplete → 检查 attachment 与 `glCheckFramebufferStatus`。

3. **项目中最大难点？**  
   结合 MiniForwardRenderer：光源空间矩阵、深度 FBO 无颜色附件时 `glDrawBuffer(GL_NONE)`。

## 白板练习（每天 1 题）

- 手写 `dot` / `cross` / `normalize`
- 手写 4×4 矩阵乘法
- 画 Shadow Map 数据流图

## 投递前自检

- [ ] 能不看代码讲解 MiniForwardRenderer 三个 Pass
- [ ] GitHub README 含构建步骤与截图
- [ ] 简历技能与项目 **一一对应**，无空泛关键词
