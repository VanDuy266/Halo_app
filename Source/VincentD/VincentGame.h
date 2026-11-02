#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/SaveGame.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "LifeProgression.h"
#include "VincentGame.generated.h"

class AVincentGameMode;
class UCameraComponent;
class UTextBlock;
class UEditableTextBox;
class UButton;
class UBorder;
class UVerticalBox;
class UStaticMesh;
class UImage;
class UTexture2D;
class UMaterialInterface;

UCLASS()
class VINCENTD_API UVincentSave : public USaveGame {
 GENERATED_BODY()
public:
 UPROPERTY(SaveGame) int32 SchemaVersion=1;
 UPROPERTY(SaveGame) int64 Money=250000;
 UPROPERTY(SaveGame) int32 Day=1;
 UPROPERTY(SaveGame) int32 Minute=420;
 UPROPERTY(SaveGame) int32 Energy=80;
 UPROPERTY(SaveGame) int32 Hunger=75;
 UPROPERTY(SaveGame) int32 Stage=0;
 UPROPERTY(SaveGame) int32 Route=0;
 UPROPERTY(SaveGame) int32 MotherTrust=0;
 UPROPERTY(SaveGame) int32 MechanicTrust=0;
 UPROPERTY(SaveGame) int32 FriendTrust=0;
 UPROPERTY(SaveGame) int32 LastExtraShiftDay=0;
 UPROPERTY(SaveGame) bool BikeReady=false;
 UPROPERTY(SaveGame) bool HasParcel=false;
 UPROPERTY(SaveGame) bool FoundNotebook=false;
 UPROPERTY(SaveGame) bool OwnsPlant=false;
};

UCLASS()
class VINCENTD_API AVincentPawn : public ACharacter {
 GENERATED_BODY()
public:
 AVincentPawn();
 virtual void SetupPlayerInputComponent(UInputComponent* Input) override;
 virtual void Tick(float DeltaSeconds) override;
 UPROPERTY() UCameraComponent* Camera;
private:
 void Forward(float Value);
 void Right(float Value);
 void Yaw(float Value);
 void Pitch(float Value);
 void Interact();
 void Menu();
 FName Target;
};

UCLASS()
class VINCENTD_API UVincentUI : public UUserWidget {
 GENERATED_BODY()
 friend class AVincentGameMode;
public:
 virtual TSharedRef<SWidget> RebuildWidget() override;
 void Show(const FString& Title, const FString& Message, bool NeedsInput=false);
 void RefreshStatus(const FString& Text);
 void SetPrompt(const FString& Text);
 void Choice();
 void StartIntro(const FString& OpeningMessage);
 bool IsDialogOpen() const {return bDialog||bIntro;}
private:
 UPROPERTY() UTextBlock* Status;
 UPROPERTY() UTextBlock* Prompt;
 UPROPERTY() UTextBlock* Heading;
 UPROPERTY() UTextBlock* Body;
 UPROPERTY() UTextBlock* Feedback;
 UPROPERTY() UEditableTextBox* Answer;
 UPROPERTY() UButton* SubmitButton;
 UPROPERTY() UButton* ContinueButton;
 UPROPERTY() UButton* VillageButton;
 UPROPERTY() UButton* CityButton;
 UPROPERTY() UBorder* Dialog;
 UPROPERTY() UBorder* IntroOverlay;
 UPROPERTY() UImage* IntroImage;
 UPROPERTY() UTextBlock* IntroTitle;
 UPROPERTY() UVerticalBox* IntroMenu;
 UPROPERTY() UTexture2D* EngineLogo;
 UPROPERTY() UTexture2D* StudioLogo;
 UPROPERTY() UTexture2D* GameLogo;
 FTimerHandle IntroTimer;
 FString OpeningStory;
 bool bIntro=false;
 void StudioStage();
 UFUNCTION() void MainMenu();
 UFUNCTION() void PlayGame();
 bool bDialog=false;
 UFUNCTION() void Submit();
 UFUNCTION() void Continue();
 UFUNCTION() void Phone();
 UFUNCTION() void Hint();
 UFUNCTION() void Restart();
 UFUNCTION() void StayVillage();
 UFUNCTION() void VisitCity();
 void Close();
 AVincentGameMode* Game() const;
};

UCLASS()
class VINCENTD_API AVincentGameMode : public AGameModeBase {
 GENERATED_BODY()
public:
 AVincentGameMode();
 virtual void BeginPlay() override;
 void Inspect(FName Key);
 void Submit(const FString& Answer);
 void Continue();
 void Phone();
 void Hint();
 void Restart();
 void ChoosePath(int32 Choice);
 void SetPrompt(const FString& Value);
 FString InteractionName(FName Key) const;
 bool InDialog() const;

private:
 UPROPERTY() UVincentUI* UI;
 UPROPERTY() UStaticMesh* Cube;
 UPROPERTY() UStaticMesh* Cylinder;
 UPROPERTY() UStaticMesh* DetailCube;
 UPROPERTY() TArray<UMaterialInterface*> Surfaces;
 UPROPERTY() TArray<AActor*> RoomActors;
 UPROPERTY() AActor* PlantActor;
 VincentLife::State Progress;
 FName Active;
 bool bRestartConfirm=false;
 int32 ClockTicks=0;
 FTimerHandle LifeClockTimer;
 void TickLifeClock();
 void BuildNeighborhood();
 void LoadVisuals();
 void Save();
 void Load();
 void Refresh();
 void ShowOutcome(VincentLife::Result Result,const FString& SuccessText);
 FString OpeningText() const;
 FString ObjectiveText() const;
 AActor* Box(FVector Location,FVector Scale,FName Key=NAME_None,int32 MaterialIndex=-1);
 void Label(FVector Location,const FString& Value,float Size=22,FName Key=NAME_None);
};
