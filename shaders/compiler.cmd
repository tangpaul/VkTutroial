@echo off
cd /d "%~dp0"

set glslcexce=%VULKAN_SDK%\bin\glslc.exe

if exist %glslcexce% (
    echo "glslc.exe found: %glslcexce%"
) else (
    echo "glslc.exe not found"
    exit /b
)

REM Compile shaders
%glslcexce% ./firstshader.vert -o ./firstshader.vert.spv
%glslcexce% ./firstshader.frag -o ./firstshader.frag.spv

echo "Shaders compiled."
pause