#define MyAppName "TiHiY MultiStream Pro"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "TiHiY-DED"
#define ObsDir "F:\OOBS\obs-studio"

[Setup]
AppId={{F9F3772E-0B4C-4477-9C29-TIHIYMULTISTREAM}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={#ObsDir}
DisableProgramGroupPage=yes
OutputBaseFilename=TiHiY_MultiStream_Pro_Setup_v1.0
Compression=lzma
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=admin

[Files]
Source: "..\build\Release\tihiy-multistream-pro.dll"; DestDir: "{app}\obs-plugins\64bit"; Flags: ignoreversion; Check: FileExists(ExpandConstant('{src}\..\build\Release\tihiy-multistream-pro.dll'))
Source: "..\build\RelWithDebInfo\tihiy-multistream-pro.dll"; DestDir: "{app}\obs-plugins\64bit"; Flags: ignoreversion; Check: FileExists(ExpandConstant('{src}\..\build\RelWithDebInfo\tihiy-multistream-pro.dll'))
Source: "..\data\*"; DestDir: "{app}\data\obs-plugins\tihiy-multistream-pro"; Flags: recursesubdirs createallsubdirs ignoreversion

[Run]
Filename: "{app}\obs64.exe"; Description: "Launch OBS Studio"; Flags: nowait postinstall skipifsilent
