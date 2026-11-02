#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/SaveGame.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
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

struct FVincentPuzzle {
 FString Name, Question, Answer, Digit, Result, Hint;
};
struct FVincentChapter {
 FString Name, Intro, Radio, Reveal, Board, Note;
 FLinearColor Color;
 int32 Hour=4;
 TArray<FVincentPuzzle> Puzzles;
};

UCLASS()
class VINCENTD_API UVincentSave : public USaveGame {
 GENERATED_BODY()
public:
 UPROPERTY(SaveGame) int32 Chapter=0;
 UPROPERTY(SaveGame) int32 Solved=0;
 UPROPERTY(SaveGame) bool Open=false;
 UPROPERTY(SaveGame) int32 Ending=0;
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
 UPROPERTY() UButton* AnButton;
 UPROPERTY() UButton* TruthButton;
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
 UFUNCTION() void Radio();
 UFUNCTION() void Hint();
 UFUNCTION() void Restart();
 UFUNCTION() void SaveAn();
 UFUNCTION() void SaveTruth();
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
 void Radio();
 void Hint();
 void Restart();
 void Finish(int32 Choice);
 void SetPrompt(const FString& Value);
 bool InDialog() const;
 int32 Chapter=0;
 int32 Solved=0;
 bool DoorOpen=false;
 int32 Ending=0;
private:
 UPROPERTY() UVincentUI* UI;
 UPROPERTY() UStaticMesh* Cube;
 UPROPERTY() UStaticMesh* Cylinder;
 UPROPERTY() UStaticMesh* DetailCube;
 UPROPERTY() TArray<UMaterialInterface*> Surfaces;
 UPROPERTY() TArray<AActor*> RoomActors;
 UPROPERTY() AActor* DoorActor;
 TArray<FVincentChapter> Chapters;
 int32 Active=-1;
 bool bDoorInput=false;
 bool bAdvance=false;
 bool bRestartConfirm=false;
 int32 HintIndex=0;
 void BuildRoom();
 void LoadVisuals();
 void Save();
 void Refresh();
 void LoadStory();
 AActor* Box(FVector Location,FVector Scale,FName Key=NAME_None,int32 MaterialIndex=-1);
 void Label(FVector Location,const FString& Value,float Size=22,FName Key=NAME_None);
};
