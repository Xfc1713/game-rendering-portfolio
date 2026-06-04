# GitHub 作品集上架指南（从零到写进简历）

按顺序做，大约 **半天～1 天** 可完成首次上架。

---

## 第 0 步：你要达到的标准（HR/面试官打开仓库时）

- [ ] 仓库 **Public**，README 一眼能看懂「做了什么」
- [ ] 有 **可构建说明**（复制命令能编过最好，你本地至少成功一次）
- [ ] 有 **演示**：截图 2～3 张 **或** B 站/网盘录屏链接（写在 README 顶部）
- [ ] 简历里的链接 **能打开**，不要 404

---

## 第 1 步：本地先跑通（必做，否则别 push）

在 PowerShell 中：

```powershell
cd "e:\许付成专用\AI漫剧\小说脚本\game-rendering-portfolio"

# 建议先测最小项目（不拉 GLFW，最快）
cd cpp-foundation
cmake -B build -S .
cmake --build build --config Release
.\build\Release\cpp_foundation_demo.exe
```

再测主项目（**需要联网**，首次 CMake 会下载 GLFW）：

```powershell
cd "e:\许付成专用\AI漫剧\小说脚本\game-rendering-portfolio\MiniForwardRenderer"
cmake -B build -S .
cmake --build build --config Release
.\build\MiniForwardRenderer\Release\MiniForwardRenderer.exe
```

- 能弹出窗口、有画面 → 继续  
- 报错 → 先解决再上传 GitHub（把报错信息留给排查）

### 截图（投递前）

运行 `MiniForwardRenderer` 后：

1. 截一张 **带阴影的场景**  
2. 截一张 **窗口标题含 FPS** 的画面  

保存到：

```
MiniForwardRenderer/screenshots/mini-renderer-1.png
MiniForwardRenderer/screenshots/mini-renderer-2.png
```

然后在 [MiniForwardRenderer/README.md](../MiniForwardRenderer/README.md) 里用 Markdown 引用（可选，上传前做更专业）。

---

## 第 2 步：录 2 分钟演示视频（强烈建议）

1. 用 **Win + G** 或 OBS 录屏，运行 `MiniForwardRenderer.exe` 约 30～60 秒  
2. 上传到 **B 站**（可不公开发布，仅链接可见）或 **网盘**  
3. 把链接写在根目录 [README.md](../README.md) 的「演示」一节（见下方模板）

---

## 第 0 步：安装 Git（若提示「git 不是内部或外部命令」）

### 方式 A：官网安装（推荐）

1. 打开 https://git-scm.com/download/win  
2. 下载 **64-bit Git for Windows Setup**  
3. 安装时一路 Next，建议勾选 **Git from the command line and also from 3rd-party software**（加入 PATH）  
4. **关闭并重新打开** cmd，输入：

```cmd
git --version
```

应显示 `git version 2.x.x`。

### 方式 B：winget（管理员 cmd）

```powershell
winget install --id Git.Git -e --source winget
```

安装后 **新开** cmd 再试 `git --version`。

### 仍找不到 git

重启电脑，或手动把 `C:\Program Files\Git\cmd` 加入系统环境变量 Path。

---

## 第 3 步：注册 / 登录 GitHub

1. 打开 https://github.com 注册（已有账号则登录）  
2. 建议用户名：**英文**、好记，例如 `xufucheng`（将用于简历链接）  
3. 开启 **两步验证**（可选，防盗号）

---

## 第 4 步：在 GitHub 网页新建空仓库

1. 右上角 **+** → **New repository**  
2. 填写：
   - **Repository name**：`game-rendering-portfolio`（与本地文件夹同名即可）
   - **Public**
   - **不要**勾选 “Add a README”（本地已有，避免冲突）
3. 点 **Create repository**  
4. 记下页面上的地址，形如：  
   `https://github.com/你的用户名/game-rendering-portfolio.git`

---

## 第 5 步：本地 Git 首次提交并推送

在 PowerShell（项目根目录）：

```powershell
cd "e:\许付成专用\AI漫剧\小说脚本\game-rendering-portfolio"

git init
git add .
git status
git commit -m "feat: 游戏渲染校招作品集（C++/OpenGL + MiniForwardRenderer）"

git branch -M main
git remote add origin https://github.com/你的用户名/game-rendering-portfolio.git
git push -u origin main
```

### 常见问题

| 问题 | 处理 |
|------|------|
| 提示要登录 | 安装 [GitHub CLI](https://cli.github.com/) 后执行 `gh auth login`，或用 HTTPS + Personal Access Token |
| `remote origin already exists` | `git remote set-url origin https://github.com/...` |
| push 被拒绝（文件过大） | 不要提交 `build/`；确认 [.gitignore](../.gitignore) 已忽略构建目录 |
| 没有配置 user.name / email | `git config user.name "许付成"` 与 `git config user.email "你的邮箱"`（仅本仓库可加 `--local`） |

推送成功后，浏览器刷新仓库页，应能看到全部代码和 README。

---

## 第 6 步：美化仓库（10 分钟，提升观感）

在 GitHub 仓库页：

1. **About**（右侧齿轮）：Description 填  
   `C++/OpenGL 校招作品集：Blinn-Phong、Shadow Map、后处理`  
2. **Topics** 添加：`opengl` `cpp` `graphics` `game-dev` `shader` `cmake`  
3. **Pin** 到个人主页：Profile → Customize → 勾选该仓库置顶  

可选：在 README 顶部加演示链接与截图（见根 README 已留位置）。

---

## 第 7 步：写进简历

打开 [career/resume-rendering.md](../career/resume-rendering.md) 与 [career/resume-backend.md](../career/resume-backend.md)，把占位符换成真实链接：

```markdown
- **仓库**：https://github.com/你的用户名/game-rendering-portfolio
- **演示**：https://www.bilibili.com/video/你的BV号   （或网盘链接）
```

导出 PDF 前在浏览器 **无痕模式** 打开链接自测一遍。

---

## 第 8 步：投递时怎么用

- 投 **渲染 / 游戏客户端**：简历项目第一条链到该仓库；附言写「主项目 MiniForwardRenderer 见 README」  
- 投 **Java 后端**：用 backend 简历，图形仓库放「业余项目」一行即可  
- 面试前：能 **15 分钟** 讲清 Shadow → Scene → Post 三 Pass（见 MiniForwardRenderer README）

---

## 自检清单（上传后打勾）

```
[ ] GitHub 仓库 Public 可访问
[ ] README 含构建命令与项目结构
[ ] 本地 MiniForwardRenderer 至少成功运行过一次
[ ] 有截图或录屏链接
[ ] 简历中链接已更新且可打开
[ ] 未把 build/、.vs/ 等大文件夹 push 上去
```

---

## 之后每次改代码

```powershell
git add .
git commit -m "描述你改了什么，例如: feat: 添加 PCF 软阴影"
git push
```

有提交记录比「只上传一次」更能体现持续学习。
