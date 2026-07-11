@echo off
setlocal
cd /d "%~dp0\.."

echo TiHiY MultiStream Pro Native OBS Plugin build

echo.
echo This requires Windows, Visual Studio 2022 C++ workload, CMake, OBS dev libraries.
echo If you use official obs-plugintemplate, copy src/data/CMakeLists pieces into that template.
echo.

if not exist build mkdir build
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
if errorlevel 1 goto fail
cmake --build build --config RelWithDebInfo
if errorlevel 1 goto fail

echo.
echo Build finished. Look in build\RelWithDebInfo or build\Release for tihiy-multistream-pro.dll
pause
exit /b 0

:fail
echo.
echo Build failed. This usually means OBS development files are not connected.
echo Recommended: use the official OBS Plugin Template/GitHub Actions route.
pause
exit /b 1
