// Editor-only, native C++ asset preparation. No Python runtime or editor plugin required.
#include "Modules/ModuleManager.h"
#include "HAL/IConsoleManager.h"
#include "Misc/Paths.h"
#include "Misc/PackageName.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "AssetImportTask.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Factories/MaterialFactoryNew.h"
#include "Factories/FbxFactory.h"
#include "Factories/FbxImportUI.h"
#include "Factories/FbxStaticMeshImportData.h"
#include "Engine/Texture2D.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Materials/MaterialExpressionTextureCoordinate.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "Materials/MaterialExpressionConstant.h"
#include "MaterialEditingLibrary.h"
#include "UObject/SavePackage.h"

DEFINE_LOG_CATEGORY_STATIC(LogVincentTools,Log,All);
class FVincentDToolsModule : public IModuleInterface {
 IConsoleObject* Command=nullptr;
 static FString ObjectPath(const FString& Package){return Package+TEXT(".")+FPackageName::GetLongPackageAssetName(Package);}
 static void Save(UObject* Asset){if(!Asset)return;UPackage* Package=Asset->GetOutermost();Package->MarkPackageDirty();FSavePackageArgs Args;Args.TopLevelFlags=RF_Public|RF_Standalone;const FString Filename=FPackageName::LongPackageNameToFilename(Package->GetName(),FPackageName::GetAssetPackageExtension());if(!UPackage::SavePackage(Package,Asset,*Filename,Args))UE_LOG(LogVincentTools,Error,TEXT("Could not save %s"),*Filename);}
 template<typename T> static T* Existing(const FString& Package){return LoadObject<T>(nullptr,*ObjectPath(Package),nullptr,LOAD_NoWarn);}
 static TArray<UObject*> Import(const FString& Filename,const FString& Directory,const FString& Name,UFactory* Factory=nullptr){
  if(!FPaths::FileExists(Filename)){UE_LOG(LogVincentTools,Error,TEXT("Missing source asset: %s"),*Filename);return {};}
  UAssetImportTask* Task=NewObject<UAssetImportTask>();Task->Filename=Filename;Task->DestinationPath=Directory;Task->DestinationName=Name;Task->bAutomated=true;Task->bReplaceExisting=false;Task->bSave=true;Task->Factory=Factory;
  FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools")).Get().ImportAssetTasks({Task});return Task->GetObjects();
 }
 static UTexture2D* Texture(const FString& Name,const FString& Kind){
  const FString AssetName=Name+TEXT("_")+Kind,Directory=TEXT("/Game/Vincent/Textures"),Package=Directory+TEXT("/")+AssetName;
  if(auto* Found=Existing<UTexture2D>(Package))return Found; // Preserve user-authored assets.
  const FString Filename=FPaths::ProjectDir()/TEXT("SourceAssets/Textures")/(AssetName+TEXT(".png"));
  UTexture2D* Result=nullptr;for(UObject* Imported:Import(Filename,Directory,AssetName))if(auto* T=Cast<UTexture2D>(Imported))Result=T;
  if(!Result)Result=Existing<UTexture2D>(Package);if(!Result){UE_LOG(LogVincentTools,Error,TEXT("Texture import failed: %s"),*Package);return nullptr;}
  if(Kind!=TEXT("BaseColor"))Result->SRGB=false;if(Kind==TEXT("Normal"))Result->CompressionSettings=TC_Normalmap;Result->PostEditChange();Save(Result);return Result;
 }
 static void Material(const FString& Name,float Metalness,float Repeat){
  const FString Directory=TEXT("/Game/Vincent/Materials"),Package=Directory/TEXT("M_")+Name;
  if(Existing<UMaterial>(Package)){UE_LOG(LogVincentTools,Display,TEXT("Preserving existing %s"),*Package);return;}
  UTexture2D* Images[3]={Texture(Name,TEXT("BaseColor")),Texture(Name,TEXT("Normal")),Texture(Name,TEXT("Roughness"))};for(auto* Image:Images)if(!Image)return;
  auto& Assets=FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools")).Get();auto* M=Cast<UMaterial>(Assets.CreateAsset(TEXT("M_")+Name,Directory,UMaterial::StaticClass(),NewObject<UMaterialFactoryNew>()));if(!M)return;
  auto* UV=Cast<UMaterialExpressionTextureCoordinate>(UMaterialEditingLibrary::CreateMaterialExpression(M,UMaterialExpressionTextureCoordinate::StaticClass(),-700,0));UV->UTiling=Repeat;UV->VTiling=Repeat;
  const EMaterialProperty Properties[3]={MP_BaseColor,MP_Normal,MP_Roughness};
  for(int32 I=0;I<3;I++){auto* Sample=Cast<UMaterialExpressionTextureSample>(UMaterialEditingLibrary::CreateMaterialExpression(M,UMaterialExpressionTextureSample::StaticClass(),-400,I*220));Sample->Texture=Images[I];Sample->SamplerType=I==1?SAMPLERTYPE_Normal:(I==2?SAMPLERTYPE_LinearColor:SAMPLERTYPE_Color);UMaterialEditingLibrary::ConnectMaterialExpressions(UV,TEXT(""),Sample,TEXT("UVs"));UMaterialEditingLibrary::ConnectMaterialProperty(Sample,I==2?TEXT("R"):TEXT("RGB"),Properties[I]);}
  auto* Metal=Cast<UMaterialExpressionConstant>(UMaterialEditingLibrary::CreateMaterialExpression(M,UMaterialExpressionConstant::StaticClass(),-100,700));Metal->R=Metalness;UMaterialEditingLibrary::ConnectMaterialProperty(Metal,TEXT(""),MP_Metallic);UMaterialEditingLibrary::RecompileMaterial(M);Save(M);
 }
 static void Branding(const FString& Name){
  const FString Directory=TEXT("/Game/Vincent/Branding"),Package=Directory/Name;if(Existing<UTexture2D>(Package))return;
  const FString Filename=FPaths::ProjectDir()/TEXT("SourceAssets/Branding")/(Name+TEXT(".png"));if(!FPaths::FileExists(Filename))return;
  for(UObject* Obj:Import(Filename,Directory,Name))if(auto* T=Cast<UTexture2D>(Obj)){T->CompressionSettings=TC_EditorIcon;T->LODGroup=TEXTUREGROUP_UI;T->PostEditChange();Save(T);}
 }
 void Prepare(){
  Material(TEXT("Plaster"),0,3);Material(TEXT("Tile"),0,3);Material(TEXT("Oak"),0,2);Material(TEXT("PaintedMetal"),.25f,2);Material(TEXT("Steel"),.8f,1);Material(TEXT("Fabric"),0,4);
  const FString MeshPackage=TEXT("/Game/Vincent/Meshes/SM_BevelCube");
  if(!Existing<UStaticMesh>(MeshPackage)){
   auto* Factory=NewObject<UFbxFactory>();Factory->ImportUI->bImportAsSkeletal=false;Factory->ImportUI->bImportMaterials=false;Factory->ImportUI->bImportTextures=false;Factory->ImportUI->MeshTypeToImport=FBXIT_StaticMesh;Factory->ImportUI->StaticMeshImportData->bCombineMeshes=true;
   Import(FPaths::ProjectDir()/TEXT("SourceAssets/Meshes/SM_BevelCube.obj"),TEXT("/Game/Vincent/Meshes"),TEXT("SM_BevelCube"),Factory);
  }
  Branding(TEXT("StudioLogo"));Branding(TEXT("UnrealEngineLogo"));
  UE_LOG(LogVincentTools,Display,TEXT("Vincent asset preparation finished. Inspect errors above, Save All, then test Standalone Game before packaging. Existing assets were preserved."));
 }
public:
 virtual void StartupModule()override{Command=IConsoleManager::Get().RegisterConsoleCommand(TEXT("Vincent.PrepareAssets"),TEXT("Prepare missing Vincent PBR materials, bevel mesh and branding. Existing assets are preserved."),FConsoleCommandDelegate::CreateRaw(this,&FVincentDToolsModule::Prepare),ECVF_Default);}
 virtual void ShutdownModule()override{if(Command)IConsoleManager::Get().UnregisterConsoleObject(Command);}
};
IMPLEMENT_MODULE(FVincentDToolsModule,VincentDTools)
