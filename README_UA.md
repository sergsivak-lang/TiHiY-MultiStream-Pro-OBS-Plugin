# TiHiY MultiStream Pro v1.9

Нативний OBS-плагін для одночасного стріму на YouTube 2K60 і Twitch 1080p60.

## Головне

- компактна масштабована панель;
- Start All / Stop All;
- окремі вікна YouTube, Twitch, Custom RTMP;
- відкриття чатів після старту;
- Twitch safe 1080 fix;
- автозапуск RutonyChat;
- **Rutony launch mode: EXE / Steam / Custom command**;
- installer-ready пакет.

## RutonyChat

Якщо RutonyChat показує:

```text
Please launch the program from your Steam client.
```

тоді в налаштуваннях Rutony вибери один із режимів:

```text
Steam protocol
```

або

```text
Custom command
```

Приклад custom command:

```text
"C:\Program Files (x86)\Steam\steam.exe" -applaunch 123456
```

## Інсталятор

Після Actions artifact можна створити Setup.exe:

```text
tools\MAKE_INSTALLER_FROM_ACTIONS_ARTIFACT.bat
```

Готовий файл буде тут:

```text
installer\output\TiHiY_MultiStream_Pro_OBS_Plugin_Setup_v1.9.exe
```
