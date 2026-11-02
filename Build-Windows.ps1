param(
 [Parameter(Mandatory=$true)][string]$EngineRoot,
 [string]$OutputDirectory = (Join-Path $PSScriptRoot 'Packaged')
)
$ErrorActionPreference = 'Stop'
$ProjectFile = Join-Path $PSScriptRoot 'VincentD.uproject'
$AutomationTool = Join-Path $EngineRoot 'Engine\Build\BatchFiles\RunUAT.bat'
if ($env:OS -ne 'Windows_NT') { throw 'Run this packaging script on Windows.' }
if (-not (Test-Path $AutomationTool)) { throw "Unreal not found at: $EngineRoot" }
if (-not (Test-Path $ProjectFile)) { throw "Missing project: $ProjectFile" }
$VisualMaterial = Join-Path $PSScriptRoot 'Content\Vincent\Materials\M_Plaster.uasset'
if (-not (Test-Path $VisualMaterial)) { throw 'Missing visual assets. Open the project in Unreal Editor, then run Vincent.PrepareAssets in the Editor Output Log before packaging.' }
Write-Host 'Building, cooking and packaging Vincent D for Windows. This may take a while.'
& $AutomationTool BuildCookRun "-project=$ProjectFile" -noP4 -platform=Win64 -clientconfig=Shipping -build -cook -stage -pak -archive "-archivedirectory=$OutputDirectory" -prereqs -utf8output -unattended -map=/Engine/Maps/Entry
if ($LASTEXITCODE -ne 0) { throw "Unreal packaging failed (exit $LASTEXITCODE). Read the build log above." }
$PackagedExe = Get-ChildItem -Path $OutputDirectory -Filter 'VincentD.exe' -Recurse | Select-Object -First 1
if (-not $PackagedExe) { throw 'Packaging returned successfully, but VincentD.exe was not found. Check the archive output.' }
Write-Host "Packaged executable: $($PackagedExe.FullName)"
Write-Host 'Test the game before uploading. Share the complete packaged folder, not the exe by itself.'
