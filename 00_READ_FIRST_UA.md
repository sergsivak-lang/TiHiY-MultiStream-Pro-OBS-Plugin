# TiHiY MultiStream Pro v2.1 CLEAN INSTALLER FIXED

Це чиста версія без Rutony і без автовідкриття чатів.

Виправлено інсталятор:
- більше НЕ шукає `build\Release\tihiy-multistream-pro.dll`;
- спочатку бере DLL з розпакованого GitHub Actions artifact;
- сам складає `installer\payload`;
- після цього збирає нормальний Setup.exe.

## Правильний порядок

1. Завантаж весь вміст цієї папки в GitHub repo через **Add file → Upload files**.
2. Натисни **Commit changes**.
3. Перейди в **Actions** і дочекайся збірки.
4. Скачай Windows artifact.
5. Розпакуй artifact у папку з проєктом або в будь-яку підпапку.
6. Запусти:

```bat
tools\MAKE_INSTALLER_FROM_ACTIONS_ARTIFACT.bat
```

Готовий файл буде тут:

```text
installer\output\TiHiY_MultiStream_Pro_OBS_Plugin_Setup_v2.1.exe
```

Не компілюй `.iss` вручну до того, як artifact розпакований і payload створений батником.
