; �ű��� Inno Setup �ű��� ���ɣ�
; �йش��� Inno Setup �ű��ļ�����ϸ��������İ����ĵ���

#define MyAppName "MY_APP_NAME"
#define MyAppVersion "MY_APP_VERSION"
#define MyAppPublisher "MY_APP_PUBLISHER"
#define MyAppExeName "MY_APP_EXE_NAME"
#define MyAppId "APP_ID"

[Setup]
; ע: AppId��ֵΪ������ʶ��Ӧ�ó���
; ��ҪΪ������װ����ʹ����ͬ��AppIdֵ��
; (�����µ�GUID����� ����|��IDE������GUID��)
AppId={#MyAppId}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir=OUTPUT_DIR
OutputBaseFilename=OUTPUT_BASE_FILENAME
SetupIconFile=SOURCE_PATH\ZBXH.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Messages]
UninstalledMost=%1 ��˳���ش����ĵ�����ɾ����

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "SOURCE_EXE_PATH"; DestDir: "{app}"; Flags: ignoreversion
Source: "SOURCE_PATH\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

[Run]
; ��װ�����
Filename: "{app}\RegOcx.exe";
Filename: "{app}\ZskAz.exe";

[UninstallRun]
; ж��ǰ����
Filename: "{app}\kill.bat";Flags:RunHidden SkipIfDoesntExist;
Filename: "{app}\ZskXz.exe";Flags:RunHidden SkipIfDoesntExist;

[UninstallDelete]
Name: {app}; Type: filesandordirs

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}";

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, "&", "&&")}}"; Flags: nowait postinstall skipifsilent

Root:HKCU;Subkey:"REGISTRY_SUBKEY";Flags:uninsdeletekeyifempty
Root:HKCU;Subkey:"REGISTRY_SUBKEY";ValueType:string;ValueName:"InstallPath";ValueData:"{app}";Flags:uninsdeletekey
Root:HKCU;Subkey:"REGISTRY_SUBKEY";ValueType:string;ValueName:"AppName";ValueData:"{#MyAppName}";Flags:uninsdeletekey

[code]
function InitializeSetup(): Boolean;
var sInstallPath: String;
var sAppName: String;
begin 
      if RegValueExists(HKEY_CURRENT_USER,'REGISTRY_SUBKEY', 'InstallPath') then
               begin 
                   RegQueryStringValue(HKEY_CURRENT_USER, 'REGISTRY_SUBKEY', 'AppName', sAppName);
                   MsgBox('�ü�����Ѿ���װͬ����������'+sAppName+'��,����ж��Ȼ��װ,��װ���򽫹رա�',mbError,MB_OK);
                   result:=false;
               end else
               begin result:=true;
               end;
end;

procedure CurUninstallStepChanged(CurUninstallStep : TUninstallStep);
begin
     if CurUninstallStep= usUninstall then
     RegDeleteKeyIncludingSubkeys(HKEY_CURRENT_USER,'REGISTRY_SUBKEY');
end;

