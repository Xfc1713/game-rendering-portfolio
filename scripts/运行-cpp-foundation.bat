@echo off
chcp 65001 >nul
setlocal

set "EXE=C:\dev\game-rendering-portfolio\cpp-foundation\cpp_foundation_demo.exe"
set "BIN=C:\dev\winlibs-toolchain\mingw32\bin"
if not exist "%BIN%\g++.exe" set "BIN=C:\dev\winlibs-toolchain\mingw64\bin"

if not exist "%EXE%" (
    echo [提示] 未找到程序，请先运行: 一键编译-复制到Cdev.bat
    pause
    exit /b 1
)

set "PATH=%BIN%;%PATH%"
echo 运行: %EXE%
echo.
"%EXE%"
echo.
pause
