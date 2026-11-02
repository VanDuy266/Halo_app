using UnrealBuildTool;
public class VincentD : ModuleRules {
 public VincentD(ReadOnlyTargetRules Target) : base(Target) {
  PCHUsage=PCHUsageMode.UseExplicitOrSharedPCHs;
  PublicDependencyModuleNames.AddRange(new string[]{"Core","CoreUObject","Engine","InputCore","UMG","Slate","SlateCore"});
 }
}
