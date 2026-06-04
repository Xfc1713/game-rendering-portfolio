@echo off
chcp 65001 >nul
setlocal EnableDelayedExpansion

REM 自动查找 g++：支持 WinLibs 解压在本目录旁（你的 winlibs-i686-... 文件夹）
set "GXX="

call :find_gxx "C:\winlibs\mingw64\bin\g++.exe"
call :find_gxx "C:\winlibs\mingw32\bin\g++.exe"

REM 与 game-rendering-portfolio 同级的 winlibs-*（你的实际情况）
for /d %%D in ("%~dp0..\..\winlibs-*") do (
    call :find_gxx "%%D\mingw64\bin\g++.exe"
    call :find_gxx "%%D\mingw32\bin\g++.exe"
    call :find_gxx "%%D\bin\g++.exe"
)

if exist "C:\msys64\ucrt64\bin\g++.exe" call :find_gxx "C:\msys64\ucrt64\bin\g++.exe"

if not defined GXX (
    echo [错误] 未找到 g++.exe
    echo.
    echo 请确认 WinLibs 已解压，且存在以下之一:
    echo   ...\mingw64\bin\g++.exe
    echo   ...\mingw32\bin\g++.exe
    echo.
    echo 你当前的文件夹名类似 winlibs-i686-... 可以保留，脚本会自动搜索。
    echo 更推荐以后改用 x86_64 的 winlibs-x86_64-ucrt 版本（64位）。
    echo 详见 docs\绕过VS-完整方案.md
    pause
    exit /b 1
)

set "PATH=%GXX_DIR%;%PATH%"
echo [OK] 使用 g++:
echo %GXX%
"%GXX%" --version
echo.

cd /d "%~dp0..\cpp-foundation"
echo [信息] 编译目录: %CD%

set "OUT=cpp_foundation_demo.exe"
"%GXX%" -std=c++17 -O2 -Iinclude src\main.cpp src\vector_math.cpp src\raii_demo.cpp -o "%OUT%"

if errorlevel 1 (
    echo [错误] 编译失败。
    echo 若报错含 cannot find -lkernel32 或路径乱码，请双击:
    echo   scripts\一键编译-复制到Cdev.bat
    pause
    exit /b 1
)

echo.
echo [成功] 运行 %OUT% ...
echo.
"%OUT%"
echo.
echo 若看到 Lambert / RAII，项目已跑通。可截图并上传 GitHub。
pause
exit /b 0

:find_gxx
if defined GXX exit /b 0
if exist "%~1" (
    set "GXX=%~1"
    for %%I in ("%~1") do set "GXX_DIR=%%~dpI"
)
exit /b 0
