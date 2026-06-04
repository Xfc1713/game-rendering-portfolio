# 绕过 Visual Studio 安装器（热点仍 0 B 时用本方案）

VS 安装器依赖微软 CDN，你这边长期 **0 B/秒** 时，**不要再等 VS**。改用 **MinGW（g++）** 即可编译本仓库，一样能写进简历、上传 GitHub。

推荐顺序：**方案 A（约 30 分钟出结果）→ 方案 B（能跑 OpenGL 主项目）**。

---

## 方案 A：WinLibs 便携 GCC（不装 VS、不装 MSYS2 安装器）

适合：先跑通 `cpp-foundation`，证明「项目能编译」。

### A1. 下载（不要用 aka.ms）

1. 浏览器打开：**https://github.com/brechtsanders/winlibs_mingw/releases**  
2. 下载 **最新** 的 `winlibs-x86_64-*-ucrt-*.7z` 或 `.zip`（带 `gcc` 字样的 UCRT 版）  
3. 若 GitHub 慢，用手机流量或复制到 U 盘；也可搜「winlibs mingw 下载」找国内网盘镜像  

### A2. 解压位置（重要）

**工具链本身也不能在中文路径下**（否则会 `cannot find -lkernel32` 且路径乱码）。

任选其一：

- 解压到 `C:\winlibs\mingw64`（推荐），或  
- 解压在 E 盘也行，但编译时必须用脚本 **`一键编译-复制到Cdev.bat`**（会把 winlibs **整份复制** 到 `C:\dev\winlibs-toolchain`）

确认存在：`...\mingw32\bin\g++.exe` 或 `...\mingw64\bin\g++.exe`

### A3. 一键编译（无需 CMake）

**路径含中文（如 `许付成专用`、`小说脚本`）时，必须用这个：**

```text
scripts\一键编译-复制到Cdev.bat
```

（自动复制到 `C:\dev\game-rendering-portfolio` 再编译，避免 `cannot find -lkernel32` 乱码错误。）

纯英文路径下可双击：

```text
scripts\build-cpp-foundation-mingw.bat
```

若提示找不到 g++，用记事本打开该 bat，把顶部的 `WINLIBS_BIN` 改成你的 `bin` 路径。

### A4. 成功标志

黑窗口输出含 `Lambert`、`RAII` → **你已经有可演示的本地项目**。  
可截图、可写简历「C++17 工程练习」，再 push GitHub 源码。

---

## 方案 B：MSYS2 + 清华镜像（编译 OpenGL / MiniForwardRenderer）

适合：要跑 **图形窗口** 和 **MiniForwardRenderer**。

### B1. 安装 MSYS2（安装包较小）

1. 打开清华镜像说明：https://mirrors.tuna.tsinghua.edu.cn/help/msys2/  
2. 按说明下载 **msys2-x86_64-*.exe**（从清华镜像页给的链接下，不要走卡死的通道）  
3. 安装到默认 `C:\msys64`  

### B2. 换清华源（在 MSYS2 里执行）

开始菜单打开 **「MSYS2 UCRT64」**（必须是 UCRT64），粘贴：

```bash
sed -i 's#https://mirror.msys2.org/#https://mirrors.tuna.tsinghua.edu.cn/#g' /etc/pacman.d/mirrorlist.*
pacman -Syu
```

提示关闭窗口时，关掉 UCRT64 再开一次，再执行：

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-glfw
```

输入 `Y` 确认（从清华镜像拉包，一般比 VS 安装器可靠）。

### B3. 编译项目

**把项目复制到**（避免中文路径导致工具链异常）：

```text
C:\dev\game-rendering-portfolio
```

在 **UCRT64** 终端：

```bash
export PATH="/c/dev/game-rendering-portfolio/cmake-4.3.3-windows-x86_64/bin:$PATH"
cd /c/dev/game-rendering-portfolio/cpp-foundation
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/cpp_foundation_demo.exe
```

或双击 Windows 脚本（已配好 PATH 时）：

```text
scripts\build-opengl-msys2.bat
```

### B4. 成功标志

- `cpp-foundation` 有输出  
- `MiniForwardRenderer` 弹出 3D 窗口  

---

## 方案 C：不折腾本机网络（务实）

1. 把整个 `game-rendering-portfolio` 拷到 **网吧/同学电脑/学校机房**（已装 VS 或能装 MSYS2 的机器）  
2. 在那台机器运行 `build-cpp-foundation.bat` 或方案 B  
3. **录屏 + 截图** 带回本机  
4. 本机只负责 **Git push 源码**（CMake/代码不需要微软安装器）  

简历写 GitHub 链接 + 演示视频链接即可，**不要求 HR 在你笔记本上现场编译**。

---

## 上传 GitHub（与是否装好 VS 无关）

```powershell
cd C:\dev\game-rendering-portfolio
git init
git add .
git commit -m "feat: 游戏渲染作品集"
git remote add origin https://github.com/你的用户名/game-rendering-portfolio.git
git push -u origin main
```

不要提交 `cmake-4.3.3-windows-x86_64/`、`C:\winlibs`、MSYS2 本体。

---

## 对照表：该用哪个？

| 你的情况 | 用哪个 |
|----------|--------|
| VS 安装器永远 0 B | **放弃 VS**，用方案 A 或 B |
| 只想尽快有东西写简历 | **方案 A** + 方案 C 录屏 |
| 要渲染 Demo 窗口 | **方案 B** 或 方案 C |
| GitHub 也慢 | 方案 C + 手机 4G 开热点只 push 一次 |

---

## 安装成功后不要再跑

- `vs_BuildTools.exe` / Visual Studio 安装器（可卸载或忽略）  
- 依赖 `cl.exe` 的 `build-cpp-foundation.bat`（除非以后 VS 装好了）

优先用：

- `build-cpp-foundation-mingw.bat`（方案 A）  
- `build-opengl-msys2.bat`（方案 B）  
