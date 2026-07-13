#define MyAppName "TiHiY MultiStream Pro"
#define MyAppVersion "2.1.0"
#define MyAppPublisher "TiHiY-DED"
#define ObsDir "F:\\OOBS\\obs-studio"

[Setup]
AppId={{F9F3772E-0B4C-4477-9C29-54EF81D3B315}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={#ObsDir}
DisableProgramGroupPage=yes
OutputDir=output
OutputBaseFilename=TiHiY_MultiStream_Pro_OBS_Plugin_Setup_v2.1
Compression=lzma
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=admin
UninstallDisplayName={#MyAppName}

[Files]
Source: "payload\\obs-plugins\\64bit\\tihiy-multistream-pro.dll"; DestDir: "{app}\\obs-plugins\\64bit"; Flags: ignoreversion
Source: "payload\\data\\obs-plugins\\tihiy-multistream-pro\\*"; DestDir: "{app}\\data\\obs-plugins\\tihiy-multistream-pro"; Flags: recursesubdirs createallsubdirs ignoreversion

[Run]
Filename: "{app}\\bin\\64bit\\obs64.exe"; Description: "Launch OBS Studio"; Flags: nowait postinstall skipifsilent; Check: FileExists(ExpandConstant('{app}\\bin\\64bit\\obs64.exe'))
Filename: "{app}\\obs64.exe"; Description: "Launch OBS Studio"; Flags: nowait postinstall skipifsilent; Check: FileExists(ExpandConstant('{app}\\obs64.exe'))
