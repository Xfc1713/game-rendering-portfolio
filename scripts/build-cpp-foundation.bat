@echo off
chcp 65001 >nul
setlocal EnableDelayedExpansion

call "%~dp0setup-cmake-path.bat"

where cmake >nul 2>&1
if errorlevel 1 (
    echo [错误] 未找到 cmake。说明: docs\WINDOWS-环境安装.md
    pause
    exit /b 1
)

echo [OK] cmake:
cmake --version
echo.

REM 加载 VS 编译器；未安装则下面会失败并提示
call "%~dp0setup-vs-env.bat"
if errorlevel 1 (
    echo.
    echo [错误] 未找到 Visual Studio 的 C++ 环境。
    echo.
    echo 请安装「Visual Studio 生成工具」并勾选「使用 C++ 的桌面开发」:
    echo   下载页选: 用于 Visual Studio 的工具 -^> 生成工具（非远程工具）
    echo   页面可能写 2026，与 2022 生成工具是同一类，选带 C++ 的即可
    echo.
    echo 下载: https://visualstudio.microsoft.com/zh-hans/downloads/
    echo 安装完成后重新双击本脚本。
    pause
    exit /b 1
)

where cl >nul 2>&1
if errorlevel 1 (
    echo [错误] vcvars 已执行但仍无 cl，请修复 VS 安装或勾选 C++ 工作负载。
    pause
    exit /b 1
)

echo [OK] C++ 编译器:
where cl
echo.

cd /d "%~dp0..\cpp-foundation"
echo [信息] 当前目录: %CD%

REM 依次尝试 VS 生成器（兼容 2022 / 2025 / 2026 生成工具）
set "BUILD_DIR=build"
set "CONFIGURED=0"

cmake -B build -S . -G "Visual Studio 17 2022" -A x64 2>nul && set "CONFIGURED=1"
if "!CONFIGURED!"=="0" cmake -B build -S . -G "Visual Studio 18 2025" -A x64 2>nul && set "CONFIGURED=1"
if "!CONFIGURED!"=="0" cmake -B build -S . -G "Visual Studio 19 2026" -A x64 2>nul && set "CONFIGURED=1"
if "!CONFIGURED!"=="0" (
    echo [提示] 尝试让 CMake 自动选择生成器...
    cmake -B build -S . -A x64
    if not errorlevel 1 set "CONFIGURED=1"
)

if "!CONFIGURED!"=="0" (
    echo [错误] cmake 配置失败。请确认已勾选 C++ 桌面开发。
    pause
    exit /b 1
)

cmake --build build --config Release

if errorlevel 1 (
    echo [错误] 编译失败。
    pause
    exit /b 1
)

echo.
echo [成功] 正在运行 cpp_foundation_demo ...
if exist "!BUILD_DIR!\Release\cpp_foundation_demo.exe" (
    "!BUILD_DIR!\Release\cpp_foundation_demo.exe"
) else if exist "!BUILD_DIR!\cpp_foundation_demo.exe" (
    "!BUILD_DIR!\cpp_foundation_demo.exe"
) else (
    echo [警告] 未找到 exe，请检查 !BUILD_DIR! 目录。
)

pause
