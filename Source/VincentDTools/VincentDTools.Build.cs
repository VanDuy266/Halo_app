using UnrealBuildTool;
public class VincentDTools : ModuleRules {
 public VincentDTools(ReadOnlyTargetRules Target) : base(Target) {
  PCHUsage=PCHUsageMode.UseExplicitOrSharedPCHs;
  PrivateDependencyModuleNames.AddRange(new string[]{"Core","CoreUObject","Engine","UnrealEd","AssetTools","AssetRegistry","MaterialEditor"});
 }
}
