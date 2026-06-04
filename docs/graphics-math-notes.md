# 图形学数学笔记（Games 101 对齐 · 第 1 周）

> 配合 `cpp-foundation` 向量练习与后续 OpenGL MVP 使用。

## 1. 向量

- **点积** \( a \cdot b = \|a\|\|b\|\cos\theta \)：在光照中求 \( N \cdot L \)（Lambert）。
- **叉积** \( a \times b \)：得到垂直于 \(a,b\) 的向量，用于法线、切线空间、背面剔除判断。
- **归一化**：方向向量长度变为 1，光照与旋转计算必须先归一化。

代码对应：`cpp-foundation/src/vector_math.cpp`

## 2. 矩阵与齐次坐标

- 使用 **4×4 矩阵** 统一表示平移、旋转、缩放。
- **齐次坐标** \((x,y,z,w)\)：透视投影后做透视除法 \( x' = x/w \)。

### MVP

| 矩阵 | 含义 |
|------|------|
| **M** Model | 物体局部 → 世界 |
| **V** View | 世界 → 相机 |
| **P** Projection | 相机 → 裁剪空间 |

最终：`gl_Position = P * V * M * vec4(localPos, 1.0)`

## 3. 法线变换

- 法线是方向，不能只乘 Model 矩阵（含非均匀缩放时会错）。
- 使用 **法线矩阵** \( (M^{-1})^T \) 的 3×3 部分变换法线，再归一化。

## 4. 基础光照

### Lambert（漫反射）

\[
L_d = k_d \cdot \max(N \cdot L, 0) \cdot I
\]

### Blinn-Phong（镜面）

\[
H = \text{normalize}(V + L), \quad
L_s = k_s \cdot \max(N \cdot H, 0)^{\text{shininess}}
\]

MiniForwardRenderer 片元着色器采用 Blinn-Phong。

## 5. 深度与 Z-Fighting

- **深度缓冲**：记录每像素最近深度，近处遮挡远处。
- **Z-Fighting**：两平面极近时浮点精度不足导致闪烁 → 加大 near/far 比例、使用 polygon offset。

## 6. Shadow Map 直觉

1. 从光源视角渲染场景深度 → **阴影贴图**。
2. 主相机着色时，将片元变换到光源空间，比较深度。
3. 若当前深度 > 阴影贴图深度 → 在阴影中。

## 7. 本周自检

- [ ] 能手写点积/叉积公式并解释 N·L
- [ ] 能画出 MVP 数据流（局部 → 屏幕）
- [ ] 能说明为什么法线要用逆转置矩阵

下一周：在 `opengl-tutorials` 中用代码验证上述概念。
