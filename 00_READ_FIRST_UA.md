# TiHiY MultiStream Pro v1.9 — Rutony Steam Mode + Installer Ready

## Що додано у v1.9

- Rutony launch mode: **EXE direct / Steam protocol / Custom command**.
- Якщо Rutony показує `Please launch the program from your Steam client`, використовуй **Steam protocol** або **Custom command**.
- Збереження Steam App ID та custom command у налаштуваннях.
- Інсталятор залишився в пакеті v1.9.

## Як оновити

1. Розпакуй архів.
2. GitHub → Add file → Upload files.
3. Завантаж **весь вміст** цієї папки з заміною.
4. Commit changes.
5. Actions → скачай Windows artifact.
6. Закрий OBS.
7. Замінити `tihiy-multistream-pro.dll`.
8. Запусти OBS.

## Як запускати Rutony через Steam

1. В OBS відкрий панель **TiHiY MultiStream Pro**.
2. Натисни **Rutony Chat**.
3. У `Launch mode` вибери **Steam protocol**.
4. У `Steam App ID` встав ID RutonyChat зі Steam.
5. Увімкни `Start RutonyChat after stream start`.
6. Натисни **Save settings**.

Якщо не знаєш App ID — створи ярлик RutonyChat у Steam на робочий стіл і подивись у властивостях посилання `steam://rungameid/XXXXXX`. Це число і є Steam App ID.

## Custom command приклад

```text
"C:\Program Files (x86)\Steam\steam.exe" -applaunch 123456
```

Заміни `123456` на реальний Steam App ID RutonyChat.
