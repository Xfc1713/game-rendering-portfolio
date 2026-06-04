# Windows 环境安装（解决「cmake 不是内部或外部命令」）

## 你需要安装的两样东西

| 软件 | 作用 | 下载 |
|------|------|------|
| **Visual Studio 2022** | 编译 C++ | https://visualstudio.microsoft.com/zh-hans/downloads/ |
| **CMake** | 生成工程并编译 | https://cmake.org/download/ |

### Visual Studio 2022 安装时注意

安装器里勾选工作负载：**「使用 C++ 的桌面开发」**（Desktop development with C++）。

#### 安装器一直「0 B / 0 B、0 B/秒」怎么办

1. **先取消**，完全退出安装器（任务管理器里结束 `vs_installer` / `setup` 相关进程）。
2. 右键 **Visual Studio Installer** → **以管理员身份运行**。
3. 检查网络：浏览器能否正常打开 https://aka.ms ；若用校园网/公司网，换手机热点试一次。
4. 关闭 VPN/代理后重试；若必须用代理，在 Windows「设置 → 网络和 Internet → 代理」里配置正确。
5. 改用 **Build Tools**（体积更小，够编译本项目）：
   - 打开 https://visualstudio.microsoft.com/zh-hans/downloads/
   - 展开 **「用于 Visual Studio 的工具」**
   - 下载 **「生成工具」**（页面可能写 2026，**不要**下「远程工具」）
   - 安装时只勾选 **「使用 C++ 的桌面开发」** / Desktop development with C++
   - 若 `aka.ms` **拒绝连接**：见 [VS-生成工具-下载备选.md](VS-生成工具-下载备选.md)
6. 命令行安装（网络正常时）：

```cmd
winget install Microsoft.VisualStudio.2022.BuildTools --override "--wait --passive --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"
```

7. 仍卡住：下载 **离线安装包**（搜索「Visual Studio 2022 离线布局 layout」按微软文档操作），或换时间段/网络再试。

装好任一（完整 VS 或 Build Tools）后，能编译 C++ 即可，不必强求装完整 IDE 界面。

### CMake 安装时注意

**方式 A（推荐）**：下载 **Windows x64 Installer**（`.msi`）

1. 安装向导中勾选：**Add CMake to the system PATH**
2. 安装完成后 **关闭并重新打开** cmd

**方式 B（你当前的做法）**：下载 **zip 便携版** 并解压到项目根目录

- 文件夹名类似：`cmake-4.3.3-windows-x86_64`
- 必须包含：`cmake-4.3.3-windows-x86_64\bin\cmake.exe`
- **不必**手动加 PATH — 已更新 `scripts\build-cpp-foundation.bat` 会自动识别
- **不要**把该文件夹 push 到 GitHub（已在 `.gitignore` 忽略）

验证：双击 `scripts\检测环境.bat`，应显示 `[OK] cmake`。

### 验证是否装好

新开一个 **cmd**，输入：

```cmd
cmake --version
```

应显示 `cmake version 3.x.x`。若仍报错，见文末「仍找不到 cmake」。

---

## 正确目录（不要站在 career 里编译）

项目在：

```text
E:\许付成专用\AI漫剧\小说脚本\game-rendering-portfolio\
├── cpp-foundation\          ← 先在这里编译（最简单）
├── MiniForwardRenderer\     ← 主项目
├── opengl-tutorials\
└── career\                  ← 只有简历，没有 C++ 源码
```

你之前在 `career` 文件夹里执行 `cmake`，所以会失败。

---

## 推荐：用一键脚本编译（装好 VS + CMake 后）

在资源管理器中双击：

- `scripts\build-cpp-foundation.bat` — 先测这个
- `scripts\build-mini-renderer.bat` — 主项目（需联网下载 GLFW）

或在 cmd 中：

```cmd
cd /d "E:\许付成专用\AI漫剧\小说脚本\game-rendering-portfolio"
scripts\build-cpp-foundation.bat
```

---

## 手动命令（与脚本等价）

```cmd
cd /d "E:\许付成专用\AI漫剧\小说脚本\game-rendering-portfolio\cpp-foundation"
cmake -B build -S .
cmake --build build --config Release
build\Release\cpp_foundation_demo.exe
```

---

## 仍找不到 cmake

1. 确认 CMake 已安装：打开 `C:\Program Files\CMake\bin\cmake.exe` 是否存在  
2. 临时加入 PATH（当前 cmd 窗口有效）：

```cmd
set PATH=C:\Program Files\CMake\bin;%PATH%
cmake --version
```

3. 永久添加：Win → 搜索「环境变量」→ 用户 Path → 新建 → `C:\Program Files\CMake\bin`

4. 若已装 VS 2022 但未装独立 CMake，可改用 VS 自带（路径因版本而异）：

```cmd
set PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin;%PATH%
```

将 `Community` 换成你的版本（Professional / BuildTools）。

---

## 没有 Visual Studio 时

仅装 CMake **不够**，还需要 C++ 编译器。请安装 VS 2022 的「使用 C++ 的桌面开发」，或 Build Tools for Visual Studio 2022（同样勾选 C++）。

### 检测结果是 OK cmake + X cl 时

说明 **CMake 已好，缺 MSVC 编译器**。你截图里的错误：

- `Running 'nmake' failed` / `CMAKE_CXX_COMPILER not set`

就是因为 **还没装好 Visual Studio 的 C++ 组件**。CMake 不能单独编译 C++。

**处理步骤：**

1. 安装 [VS2022 生成工具](https://visualstudio.microsoft.com/zh-hans/downloads/)（页面底部）或完整社区版  
2. 安装界面只勾选：**使用 C++ 的桌面开发**  
3. 安装完成后 **重启电脑**（推荐）  
4. 再双击 `scripts\检测环境.bat`，应出现 `[OK] cl`  
5. 再双击 `scripts\build-cpp-foundation.bat`

构建脚本已会自动加载 `vcvars64` 并使用 **Visual Studio 2022** 工程生成器，无需手动打开「Native Tools 命令提示符」。
