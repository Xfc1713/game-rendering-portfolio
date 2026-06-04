@echo off
chcp 65001 >nul
setlocal EnableDelayedExpansion

echo ===== 复制工具链 + 源码到 C:\dev 再编译 =====
echo （WinLibs 在中文路径下也会导致 cannot find -lkernel32）
echo.

set "SRC_ROOT=%~dp0.."
set "SRC_PARENT=%~dp0..\.."
set "DST_ROOT=C:\dev\game-rendering-portfolio"
set "DST_CPP=%DST_ROOT%\cpp-foundation"
set "DST_TOOL=C:\dev\winlibs-toolchain"

if not exist "C:\dev" mkdir "C:\dev"

REM ---------- 1. 定位 E 盘上的 winlibs ----------
set "WINLIBS_SRC="
for /d %%D in ("%SRC_PARENT%\winlibs-*") do set "WINLIBS_SRC=%%D"
if not defined WINLIBS_SRC (
    echo [错误] 在 %SRC_PARENT% 下未找到 winlibs-* 文件夹
    pause
    exit /b 1
)
echo [信息] 源工具链: %WINLIBS_SRC%

REM ---------- 2. 复制 winlibs 到 C:\dev（仅首次较慢）----------
if exist "%DST_TOOL%\mingw32\bin\g++.exe" (
    echo [信息] 工具链已存在，跳过复制: %DST_TOOL%
) else if exist "%DST_TOOL%\mingw64\bin\g++.exe" (
    echo [信息] 工具链已存在，跳过复制: %DST_TOOL%
) else (
    echo [1/5] 首次复制 WinLibs 到 %DST_TOOL% （约 1~3 分钟，请等待）...
    if exist "%DST_TOOL%" rmdir /s /q "%DST_TOOL%" 2>nul
    robocopy "%WINLIBS_SRC%" "%DST_TOOL%" /E /NFL /NDL /NJH /NJS /NP
    if errorlevel 8 (
        echo [错误] 工具链复制失败
        pause
        exit /b 1
    )
)

REM ---------- 3. 确定 g++ ----------
set "GXX="
if exist "%DST_TOOL%\mingw32\bin\g++.exe" set "GXX=%DST_TOOL%\mingw32\bin\g++.exe"
if exist "%DST_TOOL%\mingw64\bin\g++.exe" set "GXX=%DST_TOOL%\mingw64\bin\g++.exe"
if not defined GXX (
    echo [错误] %DST_TOOL% 内未找到 mingw32\bin\g++.exe
    pause
    exit /b 1
)
for %%I in ("%GXX%") do set "GXX_DIR=%%~dpI"
set "PATH=%GXX_DIR%;%PATH%"
echo [OK] 使用英文路径下的 g++:
echo %GXX%
echo.

REM ---------- 4. 复制源码 ----------
echo [2/5] 复制 cpp-foundation ...
if not exist "%DST_ROOT%" mkdir "%DST_ROOT%"
robocopy "%SRC_ROOT%\cpp-foundation" "%DST_CPP%" /E /NFL /NDL /NJH /NJS /XD build build-nmake >nul

REM ---------- 5. 编译 ----------
echo [3/5] 编译 ...
cd /d "%DST_CPP%"
set "SRC_LIST=src\main.cpp src\vector_math.cpp src\raii_demo.cpp"
set "CXXFLAGS=-std=c++17 -O2 -Iinclude"

REM 优先完全静态（双击不依赖 DLL）
"%GXX%" %CXXFLAGS% -static %SRC_LIST% -o cpp_foundation_demo.exe 2>nul
if errorlevel 1 (
    echo [提示] 完全静态失败，使用动态链接 ...
    "%GXX%" %CXXFLAGS% %SRC_LIST% -o cpp_foundation_demo.exe
    if errorlevel 1 (
        echo [错误] 编译失败。
        pause
        exit /b 1
    )
)

echo [3.5/5] 复制运行库 DLL 到 exe 同目录（解决 libwinpthread 等缺失）...
for %%F in ("%GXX_DIR%lib*.dll") do (
    copy /Y "%%F" "%DST_CPP%\" >nul 2>&1
)
echo [信息] 已从 %GXX_DIR% 复制 lib*.dll
echo       现在可直接双击 cpp_foundation_demo.exe

echo.
echo [4/5] 运行 ...
echo.
cpp_foundation_demo.exe

echo.
echo [5/5] 成功
echo   exe: %DST_CPP%\cpp_foundation_demo.exe
echo   工具链: %DST_TOOL%
echo   下次编译直接再双击本脚本即可（工具链只复制一次）。
pause
