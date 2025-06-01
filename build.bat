@echo off
setlocal

REM Check for build type argument
if "%1"=="" (
    echo Please specify build type: debug or release
    goto end
)

REM Path to Clang (adjust if needed)
set CLANG_PATH=C:\Program Files\LLVM\bin

REM Normalize input
set BUILD_TYPE=%1
set BUILD_TYPE=%BUILD_TYPE:~0,1%%BUILD_TYPE:~1%

if /I "%BUILD_TYPE%"=="debug" (
    set CMAKE_BUILD_TYPE=Debug
    set BUILD_DIR=build-debug
) else if /I "%BUILD_TYPE%"=="release" (
    set CMAKE_BUILD_TYPE=Release
    set BUILD_DIR=build-release
) else (
    echo Invalid build type: %BUILD_TYPE%
    echo Use "debug" or "release"
    goto end
)

REM Create build directory if it doesn't exist
if not exist %BUILD_DIR% mkdir %BUILD_DIR%
cd %BUILD_DIR%

REM Generate build system
cmake -G "Ninja" ^
  -DCMAKE_BUILD_TYPE=%CMAKE_BUILD_TYPE% ^
  -DCMAKE_C_COMPILER="%CLANG_PATH%\clang.exe" ^
  -DCMAKE_CXX_COMPILER="%CLANG_PATH%\clang++.exe" ^
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ^
  ..

REM Build the project
ninja

REM Run the program (optional)
EchoScript.exe

:end
endlocal
pause
