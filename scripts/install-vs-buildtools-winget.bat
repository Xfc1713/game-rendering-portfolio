@echo off
chcp 65001 >nul
echo ===== 通过 winget 安装 VS2022 生成工具（C++）=====
echo 需要: 管理员权限 + 可访问微软服务器的网络（建议手机热点）
echo.

net session >nul 2>&1
if errorlevel 1 (
    echo [错误] 请右键本文件 -^> 以管理员身份运行
    pause
    exit /b 1
)

where winget >nul 2>&1
if errorlevel 1 (
    echo [错误] 未找到 winget。请在 Microsoft Store 安装「应用安装程序」后重试。
    pause
    exit /b 1
)

echo [信息] 更新 winget 源...
winget source update

echo.
echo [信息] 开始安装 Build Tools + C++ 工作负载（较久，请勿关闭窗口）...
winget install Microsoft.VisualStudio.2022.BuildTools --override "--wait --passive --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended" --accept-package-agreements --accept-source-agreements

if errorlevel 1 (
    echo.
    echo [错误] winget 安装失败。请换热点后重试，或阅读 docs\VS-安装器卡在0B.md
    pause
    exit /b 1
)

echo.
echo [成功] 安装命令已执行。请重启电脑后运行 scripts\检测环境.bat
pause
