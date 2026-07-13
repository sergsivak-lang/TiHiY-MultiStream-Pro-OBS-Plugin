@echo off
chcp 65001 >nul
setlocal EnableExtensions EnableDelayedExpansion

echo ============================================================
echo  TiHiY MultiStream Pro v2.1 - Setup.exe builder
echo ============================================================
echo.
echo 1) Спочатку скачай Windows artifact з GitHub Actions.
echo 2) Розпакуй artifact у цю папку або будь-яку підпапку.
echo 3) Запусти цей файл.
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
  echo Встанови Inno Setup 7 і запусти цей файл ще раз.
  pause
  exit /b 1
)

echo [OK] Inno Setup: %ISCC%
echo.

set "DLL="
for /f "delims=" %%F in ('dir /s /b "tihiy-multistream-pro.dll" 2^>nul') do (
  echo %%F | findstr /i "installer\\payload" >nul
  if errorlevel 1 (
    set "DLL=%%F"
    goto :foundDll
  )
)
:foundDll

if not defined DLL (
  echo [ERROR] Не знайдено tihiy-multistream-pro.dll.
  echo Скачай Windows artifact з GitHub Actions і розпакуй його сюди.
  pause
  exit /b 1
)

echo [OK] DLL: !DLL!

set "PAYLOAD=%ROOT%\installer\payload"
if exist "%PAYLOAD%" rmdir /s /q "%PAYLOAD%"
mkdir "%PAYLOAD%\obs-plugins\64bit" >nul 2>nul
mkdir "%PAYLOAD%\data\obs-plugins\tihiy-multistream-pro" >nul 2>nul
copy /y "!DLL!" "%PAYLOAD%\obs-plugins\64bit\tihiy-multistream-pro.dll" >nul

set "DATA_FOUND="
for /d /r %%D in (tihiy-multistream-pro) do (
  if exist "%%D\locale" (
    echo [OK] Data from artifact: %%D
    xcopy /e /i /y "%%D\*" "%PAYLOAD%\data\obs-plugins\tihiy-multistream-pro\" >nul
    set "DATA_FOUND=1"
    goto :dataDone
  )
)
:dataDone

if not defined DATA_FOUND (
  echo [WARN] Data folder з artifact не знайдено. Беру repo data.
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
echo  %ROOT%\installer\output\TiHiY_MultiStream_Pro_OBS_Plugin_Setup_v2.1.exe
echo ============================================================
pause
