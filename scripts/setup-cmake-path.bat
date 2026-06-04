@echo off
REM 被 call 调用：把 CMake 加入当前 cmd 窗口的 PATH（不启用 setlocal，以便传回父脚本）

for /d %%D in ("%~dp0..\cmake-*-windows*") do (
    if exist "%%D\bin\cmake.exe" (
        set "PATH=%%D\bin;%PATH%"
        goto :after_portable
    )
)
:after_portable

if exist "C:\Program Files\CMake\bin\cmake.exe" (
    set "PATH=C:\Program Files\CMake\bin;%PATH%"
)
if exist "C:\Program Files (x86)\CMake\bin\cmake.exe" (
    set "PATH=C:\Program Files (x86)\CMake\bin;%PATH%"
)
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" (
    set "PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin;%PATH%"
)
if exist "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" (
    set "PATH=C:\Program Files\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin;%PATH%"
)

exit /b 0
