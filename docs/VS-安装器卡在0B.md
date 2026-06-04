# Visual Studio 安装器一直「0 B / 0 B、0 B/秒」

说明 **安装程序已启动**，但 **连不上微软下载服务器**（与之前 aka.ms 失败是同一类网络问题）。

---

## 立刻可试（按顺序）

### 1. 换网络

- **手机热点** + 关闭 VPN/代理/加速器  
- 退出安装器后 **以管理员身份** 重新运行 `vs_BuildTools.exe`  
- 避开晚上高峰，或换时间段

### 2. 清理安装器缓存后重试

1. 点 **取消**，完全退出安装器  
2. 任务管理器结束 `vs_installer`、`setup` 相关进程  
3. 删除文件夹（若存在）：
   - `%ProgramData%\Microsoft\VisualStudio\Packages`
   - `%TEMP%\*` 里较大的 `chocolatey` / `dd_` / `vs` 临时项（可选）
4. 重新 **管理员运行** 安装程序

### 3. 用 winget 安装（常比图形安装器稳）

**管理员** PowerShell：

```powershell
winget source update
winget install Microsoft.VisualStudio.2022.BuildTools --override "--wait --passive --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"
```

过程可能仍要下载，但走另一条通道；请同样使用 **热点**。

### 4. 检查系统代理

**设置 → 网络和 Internet → 代理**

- 若不用代理：全部 **关闭**  
- 错误的手动代理会导致 0 B/秒

### 5. DNS

把 DNS 改为 `223.5.5.5` / `119.29.29.29` 或 `8.8.8.8`，重启网络后再开安装器。

### 6. 安全软件

暂时关闭 360/电脑管家等对「Visual Studio Installer」的联网拦截，或加入信任。

---

## 仍 0 B 时的备用方案

### A. 在能上网的电脑下载「布局」再拷贝（离线/半离线）

在 **网络正常的电脑** 上按微软文档做 [layout 离线包](https://learn.microsoft.com/zh-cn/visualstudio/install/create-an-offline-installation-of-visual-studio)，用 U 盘拷回本机安装。适合校园网长期封微软 CDN 的情况。

### B. 学校机房 / 同学电脑

在已装好 **C++ 桌面开发** 的 Windows 上：

1. 复制整个 `game-rendering-portfolio` 文件夹  
2. 运行 `scripts\build-cpp-foundation.bat`  
3. 截图/录屏后，你本机继续用 GitHub 上传 **源码**（不必在本机编译）

简历可写项目链接；面试前在机房再演示运行。

### C. MinGW 编译（不依赖 Visual Studio）

若短期无法装好 VS，可用 **MSYS2 + MinGW** 编译本项目（需另装，见下节）。

---

## 方案 C：MSYS2 + MinGW（无 VS 时）

1. 安装 MSYS2：https://www.msys2.org/  
2. 在 **MSYS2 UCRT64** 终端执行：

```bash
pacman -Syu
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-glfw
```

3. 在 **UCRT64** 终端进入项目：

```bash
cd /e/许付成专用/AI漫剧/小说脚本/game-rendering-portfolio/cpp-foundation
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/cpp_foundation_demo.exe
```

（路径按你实际盘符调整；中文路径若有问题，可把项目复制到 `C:\dev\game-rendering-portfolio`。）

---

## 安装成功的标志

运行 `scripts\检测环境.bat`：

```
[OK] cmake
[OK] cl
```

然后 `scripts\build-cpp-foundation.bat` 能输出 Lambert / RAII 文字。

---

## 建议你怎么选

| 情况 | 建议 |
|------|------|
| 能开热点 | 热点 + winget（办法 3） |
| 长期封微软 | 机房编译 或 layout 离线包 |
| 急着要 Demo | MSYS2 方案 C 或 中文路径改到 `C:\dev\` |
