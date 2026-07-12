@echo off
chcp 65001 >nul
setlocal EnableExtensions EnableDelayedExpansion

echo ============================================================
echo  TiHiY MultiStream Pro - просте встановлення без Setup.exe
echo ============================================================
echo.

set "OBS_DIR=F:\OOBS\obs-studio"
if not exist "%OBS_DIR%\bin\64bit\obs64.exe" (
  echo Не знайдено OBS за шляхом: %OBS_DIR%
  set /p OBS_DIR=Введи шлях до кореневої папки OBS Studio: 
)

if not exist "%OBS_DIR%\bin\64bit\obs64.exe" (
  echo [ERROR] Це не папка OBS Studio. Має існувати bin\64bit\obs64.exe
  pause
  exit /b 1
)

set "ROOT=%~dp0.."
cd /d "%ROOT%"
set "DLL="
for /f "delims=" %%F in ('dir /s /b "tihiy-multistream-pro.dll" 2^>nul') do (
  set "DLL=%%F"
  goto :foundDll
)
:foundDll

if not defined DLL (
  echo [ERROR] Не знайдено tihiy-multistream-pro.dll.
  echo Розпакуй Windows artifact з GitHub Actions у цю папку або в підпапку.
  pause
  exit /b 1
)

echo Закрий OBS перед встановленням.
pause

mkdir "%OBS_DIR%\obs-plugins\64bit" >nul 2>nul
mkdir "%OBS_DIR%\data\obs-plugins\tihiy-multistream-pro" >nul 2>nul

if exist "%OBS_DIR%\obs-plugins\64bit\tihiy-multistream-pro.dll" (
  copy /y "%OBS_DIR%\obs-plugins\64bit\tihiy-multistream-pro.dll" "%OBS_DIR%\obs-plugins\64bit\tihiy-multistream-pro.dll.bak" >nul
)

copy /y "!DLL!" "%OBS_DIR%\obs-plugins\64bit\tihiy-multistream-pro.dll" >nul
if exist "%ROOT%\data" xcopy /e /i /y "%ROOT%\data\*" "%OBS_DIR%\data\obs-plugins\tihiy-multistream-pro\" >nul

echo.
echo [OK] Плагін встановлено в:
echo %OBS_DIR%
echo.
pause
