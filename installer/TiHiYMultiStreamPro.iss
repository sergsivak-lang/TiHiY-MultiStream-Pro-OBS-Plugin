#define MyAppName "TiHiY MultiStream Pro"
#define MyAppVersion "1.9.0"
#define MyAppPublisher "TiHiY-DED"
#define MyAppExeName "obs64.exe"

[Setup]
AppId={{BDA22B10-CC0E-4D76-9822-19AC3D48E934}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName=F:\OOBS\obs-studio
DisableProgramGroupPage=yes
OutputDir=output
OutputBaseFilename=TiHiY_MultiStream_Pro_OBS_Plugin_Setup_v1.9
Compression=lzma2
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=admin
UninstallDisplayName={#MyAppName}
CloseApplications=yes
RestartApplications=no

[Languages]
Name: "ukrainian"; MessagesFile: "compiler:Languages\Ukrainian.isl"
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "Створити ярлик OBS на робочому столі"; GroupDescription: "Додатково:"; Flags: unchecked
Name: "launchobs"; Description: "Запустити OBS після встановлення"; GroupDescription: "Після встановлення:"; Flags: unchecked

[Files]
Source: "payload\obs-plugins\64bit\tihiy-multistream-pro.dll"; DestDir: "{app}\obs-plugins\64bit"; Flags: ignoreversion
Source: "payload\data\obs-plugins\tihiy-multistream-pro\*"; DestDir: "{app}\data\obs-plugins\tihiy-multistream-pro"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{autodesktop}\OBS Studio - TiHiY MultiStream"; Filename: "{app}\bin\64bit\obs64.exe"; WorkingDir: "{app}\bin\64bit"; Tasks: desktopicon

[Run]
Filename: "{app}\bin\64bit\obs64.exe"; Description: "Запустити OBS Studio"; Flags: nowait postinstall skipifsilent; Tasks: launchobs

[UninstallDelete]
Type: files; Name: "{app}\obs-plugins\64bit\tihiy-multistream-pro.dll"
Type: filesandordirs; Name: "{app}\data\obs-plugins\tihiy-multistream-pro"

[Code]
function IsOBSFolder(Dir: String): Boolean;
begin
  Result := FileExists(Dir + '\bin\64bit\obs64.exe') and DirExists(Dir + '\obs-plugins\64bit') and DirExists(Dir + '\data\obs-plugins');
end;

function NextButtonClick(CurPageID: Integer): Boolean;
begin
  Result := True;
  if CurPageID = wpSelectDir then
  begin
    if not IsOBSFolder(WizardDirValue) then
    begin
      MsgBox('Це не схоже на папку OBS Studio.' + #13#10 + #13#10 +
             'Вибери кореневу папку OBS, наприклад:' + #13#10 +
             'F:\OOBS\obs-studio' + #13#10 + #13#10 +
             'У ній має бути файл: bin\64bit\obs64.exe', mbError, MB_OK);
      Result := False;
    end;
  end;
end;
