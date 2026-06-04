# Visual Studio 生成工具下载失败（aka.ms 无法访问）

若浏览器打开 `https://aka.ms/vs/stable/vs_BuildTools.exe` 出现 **「拒绝了连接」ERR_CONNECTION_REFUSED**，通常是网络/防火墙/代理导致短链接 `aka.ms` 不可用，**不是**你操作错误。

---

## 办法 1：手机热点（优先试）

1. 手机开热点，电脑连热点  
2. 关闭 VPN/代理  
3. 重新打开微软下载页，点 **生成工具 → 下载**

---

## 办法 2：用 winget（命令行安装，常能绕过浏览器）

以 **管理员** 打开 cmd 或 PowerShell：

```powershell
winget install Microsoft.VisualStudio.2022.BuildTools --override "--wait --passive --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"
```

若提示没有 winget，先在 Microsoft Store 安装「应用安装程序」。

安装结束后重启电脑，运行 `scripts\检测环境.bat`。

---

## 办法 3：直链下载（不经过 aka.ms）

在浏览器地址栏 **整段复制** 打开（若失效，用办法 1 或 2）：

**Visual Studio 2022 生成工具（常用）：**

```
https://download.visualstudio.microsoft.com/download/pr/3e542575-929e-4297-b6c6-bef34d0ee648/aa2222ad244c4fef290a61eddb43ba29dc7e73393a487682d7140269c1142090/vs_BuildTools.exe
```

**页面标注 2026 时，微软可能提供新直链，格式类似：**

```
https://download.visualstudio.microsoft.com/download/pr/c40d2503-2e56-4f54-96c2-d886056cfedb/44f811a46b7556514b4fadc906ccc9685f96b63eeb8a36333425b3b315999c22/vs_BuildTools.exe
```

下载得到 `vs_BuildTools.exe` 后 **双击运行**，勾选 **「使用 C++ 的桌面开发」**。

---

## 办法 4：完整 Visual Studio 社区版（安装器不同链接）

若生成工具直链也不行，可改下 **社区版**（体积更大，但含 C++）：

1. 打开 https://visualstudio.microsoft.com/zh-hans/vs/community/  
2. 点 **免费下载**（不要从 aka.ms 短链进）  
3. 安装时同样只勾选 **使用 C++ 的桌面开发**

---

## 办法 5：改 DNS / 检查代理

- DNS 改为 `223.5.5.5` 或 `8.8.8.8` 后重试  
- Windows 设置 → 网络 → 代理 → 若不用代理请 **关闭**  
- 暂时关闭安全软件对浏览器的拦截  

---

## 安装完成后

1. 重启电脑  
2. `scripts\检测环境.bat` → `[OK] cl`  
3. `scripts\build-cpp-foundation.bat`  

---

## 仍无法安装时

可到学校机房/同学电脑完成 **首次编译**，或请有网络环境的朋友代下 `vs_BuildTools.exe` 用 U 盘拷贝（官方安装包，约几 MB 在线安装器，运行后仍需联网拉组件）。
