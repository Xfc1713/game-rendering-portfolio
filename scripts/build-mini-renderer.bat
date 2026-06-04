@echo off
chcp 65001 >nul
setlocal EnableDelayedExpansion

call "%~dp0setup-cmake-path.bat"
call "%~dp0setup-vs-env.bat"
if errorlevel 1 (
    echo [错误] 请先安装 VS2022 或 Build Tools 的 C++ 工作负载。
    pause
    exit /b 1
)

cd /d "%~dp0..\MiniForwardRenderer"
echo [信息] 当前目录: %CD%
echo [信息] 首次构建需联网下载 GLFW...

cmake -B build -S . -G "Visual Studio 17 2022" -A x64
if errorlevel 1 (
    echo [错误] cmake 配置失败。
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
echo [成功] 正在启动 MiniForwardRenderer ...
if exist "build\MiniForwardRenderer\Release\MiniForwardRenderer.exe" (
    start "" "build\MiniForwardRenderer\Release\MiniForwardRenderer.exe"
) else if exist "build\Release\MiniForwardRenderer.exe" (
    start "" "build\Release\MiniForwardRenderer.exe"
) else (
    echo [警告] 未找到 exe。
)

pause
