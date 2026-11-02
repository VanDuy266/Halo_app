using UnrealBuildTool;
using System.Collections.Generic;
public class VincentDTarget : TargetRules {
 public VincentDTarget(TargetInfo Target) : base(Target) { Type=TargetType.Game; DefaultBuildSettings=BuildSettingsVersion.V4; ExtraModuleNames.Add("VincentD"); }
}
