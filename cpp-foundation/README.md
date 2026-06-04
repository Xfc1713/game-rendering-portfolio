# 第 1 周：C++ 工程化基础

## 目标

- 能用 **CMake** 组织多文件 C++ 工程
- 掌握指针/引用、RAII、`unique_ptr` / `shared_ptr`、移动语义
- 将向量点积/叉积/归一化与图形学直觉联系起来

## 构建与运行

```powershell
cmake -B build -S .
cmake --build build --config Release
.\build\Release\cpp_foundation_demo.exe
```

## 代码导读

| 文件 | 内容 |
|------|------|
| `vector_math.*` | 点积、叉积、归一化 |
| `raii_demo.*` | GPU 缓冲伪资源、移动构造、智能指针 |
| `main.cpp` | Lambert N·L 数值示例 |

## 配套学习

- [learncpp.com](https://www.learncpp.com/) 第 1～12 章
- 本仓库 [docs/graphics-math-notes.md](../docs/graphics-math-notes.md)
