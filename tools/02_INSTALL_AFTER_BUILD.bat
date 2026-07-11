@echo off
setlocal
set OBS_DIR=F:\OOBS\obs-studio
set DLL1=%~dp0\..\build\RelWithDebInfo\tihiy-multistream-pro.dll
set DLL2=%~dp0\..\build\Release\tihiy-multistream-pro.dll

if not exist "%OBS_DIR%\obs64.exe" set OBS_DIR=C:\Program Files\obs-studio

if exist "%DLL1%" set DLL=%DLL1%
if exist "%DLL2%" set DLL=%DLL2%
if "%DLL%"=="" (
  echo DLL not found. Build first.
  pause
  exit /b 1
)

mkdir "%OBS_DIR%\obs-plugins\64bit" 2>nul
mkdir "%OBS_DIR%\data\obs-plugins\tihiy-multistream-pro\locale" 2>nul
copy /Y "%DLL%" "%OBS_DIR%\obs-plugins\64bit\tihiy-multistream-pro.dll"
xcopy /E /I /Y "%~dp0\..\data" "%OBS_DIR%\data\obs-plugins\tihiy-multistream-pro"

echo Installed to %OBS_DIR%
pause
