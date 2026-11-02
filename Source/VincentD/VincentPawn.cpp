#include "VincentGame.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

AVincentPawn::AVincentPawn() {
 PrimaryActorTick.bCanEverTick=true;
 Camera=CreateDefaultSubobject<UCameraComponent>(TEXT("Eyes"));
 Camera->SetupAttachment(RootComponent);
 Camera->SetRelativeLocation(FVector(0,0,65));
 Camera->bUsePawnControlRotation=true;
 bUseControllerRotationYaw=true;
 GetCharacterMovement()->MaxWalkSpeed=260;
 GetCharacterMovement()->bOrientRotationToMovement=false;
}
void AVincentPawn::SetupPlayerInputComponent(UInputComponent* Input) {
 Super::SetupPlayerInputComponent(Input);
 Input->BindAxis("Forward",this,&AVincentPawn::Forward);
 Input->BindAxis("Right",this,&AVincentPawn::Right);
 Input->BindAxis("LookYaw",this,&AVincentPawn::Yaw);
 Input->BindAxis("LookPitch",this,&AVincentPawn::Pitch);
 Input->BindAction("Interact",IE_Pressed,this,&AVincentPawn::Interact);
 Input->BindAction("Menu",IE_Pressed,this,&AVincentPawn::Menu);
}
void AVincentPawn::Forward(float V){auto* G=Cast<AVincentGameMode>(UGameplayStatics::GetGameMode(this));if(G&&!G->InDialog())AddMovementInput(GetActorForwardVector(),V);}
void AVincentPawn::Right(float V){auto* G=Cast<AVincentGameMode>(UGameplayStatics::GetGameMode(this));if(G&&!G->InDialog())AddMovementInput(GetActorRightVector(),V);}
void AVincentPawn::Yaw(float V){auto* G=Cast<AVincentGameMode>(UGameplayStatics::GetGameMode(this));if(G&&!G->InDialog())AddControllerYawInput(V);}
void AVincentPawn::Pitch(float V){auto* G=Cast<AVincentGameMode>(UGameplayStatics::GetGameMode(this));if(G&&!G->InDialog())AddControllerPitchInput(V);}
void AVincentPawn::Tick(float Dt){
 Super::Tick(Dt);
 auto* G=Cast<AVincentGameMode>(UGameplayStatics::GetGameMode(this));if(!G||G->InDialog())return;
 FHitResult Hit;FCollisionQueryParams Params;Params.AddIgnoredActor(this);
 const FVector Start=Camera->GetComponentLocation();
 Target=NAME_None;
 if(GetWorld()->LineTraceSingleByChannel(Hit,Start,Start+Camera->GetForwardVector()*330,ECC_Visibility,Params)&&Hit.GetActor()) {
  for(FName Tag:Hit.GetActor()->Tags)if(Tag=="painting"||Tag=="clock"||Tag=="desk"||Tag=="door"||Tag=="radio"){Target=Tag;break;}
 }
 G->SetPrompt(Target.IsNone()?TEXT(""):FString::Printf(TEXT("[E] Khám phá: %s"),*Target.ToString()));
}
void AVincentPawn::Interact(){auto* G=Cast<AVincentGameMode>(UGameplayStatics::GetGameMode(this));if(G&&!G->InDialog()&&!Target.IsNone())G->Inspect(Target);}
void AVincentPawn::Menu(){auto* G=Cast<AVincentGameMode>(UGameplayStatics::GetGameMode(this));if(G&&!G->InDialog())G->Radio();}
