@echo off
chcp 65001 >nul

echo ===== 环境检测 =====

echo.



call "%~dp0setup-cmake-path.bat"



where cmake >nul 2>&1

if errorlevel 1 (

    echo [X] cmake：未找到

    echo     确认存在: cmake-4.3.3-windows-x86_64\bin\cmake.exe

) else (

    echo [OK] cmake：

    cmake --version

    where cmake

)



echo.

call "%~dp0setup-vs-env.bat"

if errorlevel 1 (

    echo [X] Visual Studio C++：未安装或未勾选 C++ 工作负载

    echo     请安装 VS2022 或 Build Tools，勾选「使用 C++ 的桌面开发」

    echo     下载: https://visualstudio.microsoft.com/zh-hans/downloads/

) else (

    where cl >nul 2>&1

    if errorlevel 1 (

        echo [X] cl 编译器：vcvars 存在但 cl 仍不可用

    ) else (

        echo [OK] cl 编译器：

        where cl

    )

)



echo.

echo 项目根目录: %~dp0..

echo 两项均为 OK 后，请运行: scripts\build-cpp-foundation.bat

echo.

pause

