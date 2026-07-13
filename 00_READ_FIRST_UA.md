# TiHiY MultiStream Pro v2.0 — CLEAN

Це чиста стабільна версія без RutonyChat і без відкриття чатів при старті.

## Що залишилось
- Start All / Stop All.
- YouTube 2K60.
- Twitch 1080p60.
- Custom RTMP.
- Save settings.
- Recommended settings.
- Twitch safe 1080 fix.
- Інсталятор / простий BAT для встановлення.

## Що прибрано
- RutonyChat повністю.
- Start Rutony / Stop Rutony.
- Rutony Steam mode.
- Open chats.
- Chat URL.
- Open chat after start.

## Встановлення
1. Завантаж весь вміст цієї папки у GitHub template repo.
2. Commit changes.
3. Actions → скачай Windows artifact.
4. Закрий OBS.
5. Заміни `tihiy-multistream-pro.dll` або зроби інсталятор через `tools/MAKE_INSTALLER_FROM_ACTIONS_ARTIFACT.bat`.

Ключі YouTube/Twitch збережуться, бо використовується та сама локальна QSettings-секція плагіна.
