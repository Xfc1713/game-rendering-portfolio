# 第 2 周：OpenGL 入门教程

对应 [LearnOpenGL 中文](https://learnopengl-cn.github.io/) 核心章节，每个 Demo 可独立运行。

| Demo | 对应章节 | 说明 |
|------|----------|------|
| `01_triangle` | 入门 | VAO/VBO、基础着色器 |
| `02_shader_color` | 着色器 | 顶点颜色插值 |
| `03_textured_quad` | 纹理 | sampler2D、程序生成棋盘格纹理 |
| `04_mvp_cube` | 坐标系统 | MVP、旋转立方体、深度测试 |
| `05_lighting` | 光照 | Blinn-Phong、法线矩阵 |

## 构建

```powershell
cmake -B build -S .
cmake --build build --config Release
```

## 运行

```powershell
.\build\01_triangle\Release\01_triangle.exe
.\build\05_lighting\Release\05_lighting.exe
```

按 `Esc` 关闭窗口。

## 截图说明（投递前请自行截图放入 `screenshots/`）

- `01_triangle.png` — 橙色三角形
- `05_lighting.png` — 光照立方体

## 学习要点自检

- [ ] 能解释 VAO/VBO 与 `glVertexAttribPointer` stride
- [ ] 能说明 `uniform mat4 uMVP` 在 CPU 侧如何每帧更新
- [ ] 能口述 Blinn-Phong 中 N、L、H 的含义
