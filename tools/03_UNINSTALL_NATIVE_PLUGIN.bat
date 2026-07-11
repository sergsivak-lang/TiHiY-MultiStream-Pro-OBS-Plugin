@echo off
setlocal
set OBS_DIR=F:\OOBS\obs-studio
if not exist "%OBS_DIR%\obs64.exe" set OBS_DIR=C:\Program Files\obs-studio

del "%OBS_DIR%\obs-plugins\64bit\tihiy-multistream-pro.dll" 2>nul
rmdir /S /Q "%OBS_DIR%\data\obs-plugins\tihiy-multistream-pro" 2>nul

echo TiHiY native plugin removed from %OBS_DIR%
pause
