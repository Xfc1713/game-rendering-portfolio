# GitHub 仓库初始化步骤（简版）

**完整图文流程见 [GITHUB-上手指南.md](GITHUB-上手指南.md)**（含截图、录屏、简历填链接、自检清单）。

在项目根目录 `game-rendering-portfolio` 执行：

```powershell
git init
git add .
git commit -m "feat: 游戏渲染校招作品集（C++/OpenGL + MiniForwardRenderer）"
```

在 GitHub 新建仓库 `game-rendering-portfolio`（Public），然后：

```powershell
git remote add origin https://github.com/你的用户名/game-rendering-portfolio.git
git branch -M main
git push -u origin main
```

将链接填入：

- [career/resume-rendering.md](../career/resume-rendering.md)
- [career/cover-letter-template.md](../career/cover-letter-template.md)

## .gitignore

已忽略 `build/` 等构建产物；首次 clone 者需自行 `cmake` 构建（会拉取 GLFW）。
