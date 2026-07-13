# Чому це має бути native DLL, а не Lua

Lua-скрипт зручно перевіряє логіку і налаштування, але він не виглядає як повноцінний встановлений плагін OBS.

Повноцінний OBS-плагін для Windows має виглядати так:

```text
F:\OOBS\obs-studio\obs-plugins\64bit\tihiy-multistream-pro.dll
F:\OOBS\obs-studio\data\obs-plugins\tihiy-multistream-pro\locale\uk-UA.ini
```

Після запуску OBS він має зʼявитися як dock-панель, а не в меню скриптів.

## Що вже перевірено по твоєму OBS

- OBS 32.1.2 64-bit.
- RTX/NVENC доступний.
- `obs-multi-rtmp` вимкнений через менеджер плагінів.
- `rtmp-services`, `obs-outputs`, `obs-nvenc` завантажені.

## Наступний правильний крок

1. Створюємо GitHub repo на базі `obsproject/obs-plugintemplate`.
2. Вставляємо цей код у template.
3. GitHub Actions збирає `.dll` і installer.
4. Ти ставиш `.exe` і тестуєш у OBS.


## v1.3 Twitch Safe Fix
- Додано Twitch safe 1080 fix: x264 fallback для Twitch 1080p, коли OBS canvas 2560x1440.
- Додано кнопку Apply recommended 2K YouTube + 1080 Twitch.
- Додано Save settings для локального збереження серверів/ключів/бітрейтів.


---
TiHiY MultiStream Pro v2.0 CLEAN: RutonyChat and auto-open chats are removed.
