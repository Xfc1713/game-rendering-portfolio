@echo off
chcp 65001 >nul
set "DST_CPP=C:\dev\game-rendering-portfolio\cpp-foundation"
set "BIN=C:\dev\winlibs-toolchain\mingw32\bin"
if not exist "%BIN%\g++.exe" set "BIN=C:\dev\winlibs-toolchain\mingw64\bin"

if not exist "%DST_CPP%\cpp_foundation_demo.exe" (
    echo 请先运行 一键编译-复制到Cdev.bat
    pause
    exit /b 1
)

echo 复制 %BIN%\lib*.dll 到 %DST_CPP%
for %%F in ("%BIN%lib*.dll") do copy /Y "%%F" "%DST_CPP%\"
echo 完成。请再双击 cpp_foundation_demo.exe
pause
