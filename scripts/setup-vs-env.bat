@echo off
REM 加载 MSVC 环境（支持 VS2022 / 2025 / 2026 生成工具或社区版）

set "VCVARS="

for %%Y in (2026 2025 2022) do (
    for %%E in (BuildTools Community Professional Enterprise) do (
        if exist "C:\Program Files\Microsoft Visual Studio\%%Y\%%E\VC\Auxiliary\Build\vcvars64.bat" (
            set "VCVARS=C:\Program Files\Microsoft Visual Studio\%%Y\%%E\VC\Auxiliary\Build\vcvars64.bat"
            set "VS_YEAR=%%Y"
            goto :found
        )
    )
)

:found
if not defined VCVARS (
    set "VS_ENV_OK=0"
    exit /b 1
)

echo [信息] 加载 MSVC 环境: %VCVARS%
call "%VCVARS%" >nul 2>&1
if errorlevel 1 (
    set "VS_ENV_OK=0"
    exit /b 1
)

set "VS_ENV_OK=1"
exit /b 0
