@echo off
chcp 65001 >nul
setlocal EnableDelayedExpansion

set "MSYS_UCRT=C:\msys64\ucrt64\bin"
set "PROJECT_ROOT=%~dp0.."

if not exist "%MSYS_UCRT%\g++.exe" (
    echo [错误] 未找到 MSYS2 UCRT64。请先按 docs\绕过VS-完整方案.md 方案 B 安装 MSYS2。
    pause
    exit /b 1
)

set "PATH=%MSYS_UCRT%;%PATH%"

REM 便携 CMake（若已放在项目根目录）
for /d %%D in ("%PROJECT_ROOT%\cmake-*-windows*") do (
    if exist "%%D\bin\cmake.exe" set "PATH=%%D\bin;%PATH%"
)

where cmake >nul 2>&1
if errorlevel 1 (
    echo [错误] 未找到 cmake。请在 MSYS2 UCRT64 中: pacman -S mingw-w64-ucrt-x86_64-cmake
    pause
    exit /b 1
)

echo [建议] 若路径含中文导致失败，请复制项目到 C:\dev\game-rendering-portfolio 后修改本脚本 PROJECT_ROOT
echo.

cd /d "%PROJECT_ROOT%\MiniForwardRenderer"
echo [信息] 编译 MiniForwardRenderer ...
echo.

cmake -B build-mingw -S . -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DUSE_SYSTEM_GLFW=ON
if errorlevel 1 (
    echo [提示] 若 USE_SYSTEM_GLFW 失败，在 UCRT64 终端执行: pacman -S mingw-w64-ucrt-x86_64-glfw
    cmake -B build-mingw -S . -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
)
if errorlevel 1 (
    echo [错误] cmake 配置失败。
    pause
    exit /b 1
)

cmake --build build-mingw
if errorlevel 1 (
    echo [错误] 编译失败。
    pause
    exit /b 1
)

if exist "build-mingw\MiniForwardRenderer.exe" (
    start "" "build-mingw\MiniForwardRenderer.exe"
) else if exist "build-mingw\MiniForwardRenderer\MiniForwardRenderer.exe" (
    start "" "build-mingw\MiniForwardRenderer\MiniForwardRenderer.exe"
) else (
    echo [OK] 编译完成，请到 build-mingw 目录查找 exe
)

pause
