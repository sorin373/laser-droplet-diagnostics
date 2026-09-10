@echo off
setlocal

echo ============================
echo Configuring project...
echo ============================

cmake -S . -B build -G "MinGW Makefiles"

if errorlevel 1 (
    echo.
    echo CMake configuration failed.
    pause
    exit /b 1
)

echo.
echo ============================
echo Building project...
echo ============================

cmake --build build

if errorlevel 1 (
    echo.
    echo Build failed.
    pause
    exit /b 1
)

echo.
echo ============================
echo Build successful.
echo ============================

build\bin\droplet_cli.exe

pause