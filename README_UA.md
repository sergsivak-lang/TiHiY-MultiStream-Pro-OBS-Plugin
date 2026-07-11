# TiHiY MultiStream Pro — Native OBS Plugin v1.0

Це вже не Lua-скрипт. Це структура **повноцінного OBS-плагіна** у форматі native DLL:

- `tihiy-multistream-pro.dll` після збірки має встановлюватися в `obs-plugins/64bit`;
- дані/локалізація мають встановлюватися в `data/obs-plugins/tihiy-multistream-pro`;
- OBS має показувати dock-панель **TiHiY MultiStream Pro** без меню `Інструменти → Скрипти`.

## Важливо

У цьому архіві лежить повний native-проєкт і автоматизація збірки/інсталяції. Готовий `.dll` я не можу скомпілювати в цьому середовищі, бо тут немає Windows + Visual Studio + OBS SDK/Qt. Для реального `.dll` потрібна Windows-збірка.

Офіційний шлях OBS для таких плагінів — C++/CMake/Visual Studio або GitHub Actions на базі OBS Plugin Template.

## Що всередині

```text
src/                         C++ код плагіна
CMakeLists.txt               CMake-проєкт
data/locale/                 локалізація
installer/                   Inno Setup інсталятор
.github/workflows/           GitHub Actions автозбірка
README_UA.md                 ця інструкція
```

## Логіка плагіна

Плагін створює dock-панель у OBS:

- YouTube 2K60 — 2560x1440 / 60 FPS / 24000 Kbps;
- Twitch 1080p60 — 1920x1080 / 60 FPS / 6000 Kbps;
- Custom RTMP;
- окремий encoder/output/service під кожну платформу;
- кнопки Start/Stop окремо для кожного виходу;
- NVENC H.264 за замовчуванням: `obs_nvenc_h264_tex`.

## Як отримати готовий DLL/EXE

### Варіант 1 — GitHub Actions

1. Створи GitHub-репозиторій.
2. Завантаж туди всі файли з цього архіву.
3. Відкрий вкладку **Actions**.
4. Запусти workflow **Build Windows OBS Plugin**.
5. У результаті має зʼявитися artifact з DLL/ZIP.

### Варіант 2 — локальна збірка

Потрібно:

- Visual Studio 2022;
- Desktop development with C++;
- CMake;
- OBS Studio / OBS development files.

Потім запускати:

```bat
tools\01_BUILD_LOCAL_WINDOWS.bat
```

## Чому не можна просто зробити DLL тут

Native OBS-плагін — це Windows DLL, яка лінкується з OBS/libobs і Qt. Linux-середовище ChatGPT не має потрібного Windows toolchain, тому тут можна підготувати код, структуру, інсталятор і CI, але не чесно видати фальшивий `.dll`.

