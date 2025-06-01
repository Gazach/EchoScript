@echo off
setlocal

REM Path to Clang (adjust if needed)
set CLANG_PATH=C:\Program Files\LLVM\bin

REM Create build directory if it doesn't exist
if not exist build mkdir build
cd build

REM Generate build system with Clang and Ninja
cmake -G "Ninja" ^
  -DCMAKE_BUILD_TYPE=Debug ^
  -DCMAKE_C_COMPILER="%CLANG_PATH%\clang.exe" ^
  -DCMAKE_CXX_COMPILER="%CLANG_PATH%\clang++.exe" ^
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ^
  ..

REM Build the project
ninja

REM Run the program (optional)
EchoScript.exe

endlocal
pause
