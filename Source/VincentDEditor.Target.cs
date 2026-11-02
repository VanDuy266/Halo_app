using UnrealBuildTool;
using System.Collections.Generic;
public class VincentDEditorTarget : TargetRules {
 public VincentDEditorTarget(TargetInfo Target) : base(Target) { Type=TargetType.Editor; DefaultBuildSettings=BuildSettingsVersion.V4; ExtraModuleNames.AddRange(new string[]{"VincentD","VincentDTools"}); }
}
