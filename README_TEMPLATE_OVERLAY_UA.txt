# TiHiY MultiStream Pro v1.4 — START ALL + Twitch Safe Fix

Що нового:

- додано кнопку **Start All** поруч із **Stop All**;
- Start All запускає всі увімкнені виходи у безпечному порядку: YouTube → Twitch → Custom;
- перед Start All автоматично зберігаються налаштування/ключі через QSettings;
- залишено Twitch Safe 1080 Fix із v1.3;
- пресет: YouTube 2K60 + Twitch 1080p60.

Як оновити:

1. Розпакуй архів.
2. У GitHub template repo натисни **Add file → Upload files**.
3. Завантаж весь вміст розпакованої папки, не сам ZIP.
4. Commit changes.
5. Actions → дочекайся Windows artifact.
6. Закрий OBS.
7. Заміни `F:\OOBS\obs-studio\obs-plugins\64bit\tihiy-multistream-pro.dll`.
8. Запусти OBS.

Рекомендовано: спочатку натисни **Apply recommended 2K YouTube + 1080 Twitch**, встав ключі, натисни **Save settings**, потім тестуй **Start All**.


## v1.7 AUTO OPEN CHATS

- Додано автоматичне відкриття чатів після старту YouTube/Twitch/Custom.
- У вікнах YouTube/Twitch/Custom зʼявились поля `Chat URL` і `Open chat after start`.
- На головній компактній панелі додано кнопку `Open chats`.
- Twitch за замовчуванням має popout-chat URL для `tihiy_ded`; YouTube за замовчуванням відкриває YouTube Studio, а точний popout chat URL можна вставити вручну.
- Stream keys і Chat URLs зберігаються через `Save settings`.


## v1.7 RutonyChat Launcher
- Додано кнопку Rutony Chat у компактну панель.
- Можна вибрати шлях до RutonyChat.exe.
- Можна запускати RutonyChat вручну або автоматично після старту трансляції.
- Додано Start Rutony / Stop Rutony.

Майбутній етап: TiHiY Chat — власний аналог RutonyChat для Twitch + YouTube з ботом та імпортом налаштувань Rutony.
