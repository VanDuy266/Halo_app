param([string]$EngineRoot='D:\FXServer\UE_5.8')
$ErrorActionPreference='Stop'
$Editor=Join-Path $EngineRoot 'Engine\Binaries\Win64\UnrealEditor.exe'
$Project=Join-Path $PSScriptRoot 'VincentD.uproject'
if (-not (Test-Path $Editor)) { throw "Unreal Editor not found at $Editor. Supply -EngineRoot with your actual engine folder." }
if (-not (Test-Path $Project)) { throw 'VincentD.uproject missing beside this script.' }
$ProjectArgument='"'+$Project+'"'
Start-Process -FilePath $Editor -WorkingDirectory $PSScriptRoot -ArgumentList @($ProjectArgument)
