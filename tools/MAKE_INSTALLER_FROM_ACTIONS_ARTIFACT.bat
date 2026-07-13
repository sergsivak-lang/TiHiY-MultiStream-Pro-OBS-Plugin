@echo off
chcp 65001 >nul
setlocal EnableExtensions EnableDelayedExpansion

echo ============================================================
echo  TiHiY MultiStream Pro - Setup.exe builder
echo ============================================================
echo.
echo Потрібно: Inno Setup 6 або 7.
echo Скачай: https://jrsoftware.org/isinfo.php
echo.

set "ROOT=%~dp0.."
cd /d "%ROOT%"

set "ISCC="
if exist "%ProgramFiles(x86)%\Inno Setup 7\ISCC.exe" set "ISCC=%ProgramFiles(x86)%\Inno Setup 7\ISCC.exe"
if exist "%ProgramFiles%\Inno Setup 7\ISCC.exe" set "ISCC=%ProgramFiles%\Inno Setup 7\ISCC.exe"
if exist "%ProgramFiles(x86)%\Inno Setup 6\ISCC.exe" set "ISCC=%ProgramFiles(x86)%\Inno Setup 6\ISCC.exe"
if exist "%ProgramFiles%\Inno Setup 6\ISCC.exe" set "ISCC=%ProgramFiles%\Inno Setup 6\ISCC.exe"

if not defined ISCC (
  echo [ERROR] Не знайдено ISCC.exe від Inno Setup.
  echo Встанови Inno Setup і запусти цей файл ще раз.
  pause
  exit /b 1
)

echo [OK] Inno Setup: %ISCC%
echo.

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

echo [OK] DLL: !DLL!

set "PAYLOAD=%ROOT%\installer\payload"
if exist "%PAYLOAD%" rmdir /s /q "%PAYLOAD%"
mkdir "%PAYLOAD%\obs-plugins\64bit" >nul 2>nul
mkdir "%PAYLOAD%\data\obs-plugins\tihiy-multistream-pro\locale" >nul 2>nul
copy /y "!DLL!" "%PAYLOAD%\obs-plugins\64bit\tihiy-multistream-pro.dll" >nul

set "DATA_SRC="
for /d /r %%D in (tihiy-multistream-pro) do (
  if exist "%%D\locale" (
    set "DATA_SRC=%%D"
    goto :foundData
  )
)
:foundData

if defined DATA_SRC (
  echo [OK] Data: !DATA_SRC!
  xcopy /e /i /y "!DATA_SRC!\*" "%PAYLOAD%\data\obs-plugins\tihiy-multistream-pro\" >nul
) else (
  echo [WARN] Data folder з artifact не знайдено. Використовую repo data.
  if exist "%ROOT%\data" xcopy /e /i /y "%ROOT%\data\*" "%PAYLOAD%\data\obs-plugins\tihiy-multistream-pro\" >nul
)

echo.
echo [BUILD] Створюю Setup.exe...
"%ISCC%" "%ROOT%\installer\TiHiYMultiStreamPro.iss"
if errorlevel 1 (
  echo.
  echo [ERROR] Inno Setup не зібрав інсталятор.
  pause
  exit /b 1
)

echo.
echo ============================================================
echo  ГОТОВО
echo  Файл тут:
echo  %ROOT%\installer\output\TiHiY_MultiStream_Pro_OBS_Plugin_Setup_v1.8.exe
echo ============================================================
pause
