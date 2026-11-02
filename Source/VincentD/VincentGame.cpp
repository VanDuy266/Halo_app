#include "VincentGame.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/PointLightComponent.h"
#include "Engine/PointLight.h"
#include "Engine/RectLight.h"
#include "Components/RectLightComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

AVincentGameMode::AVincentGameMode(){
 DefaultPawnClass=AVincentPawn::StaticClass();
 static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));Cube=CubeAsset.Object;
 static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));Cylinder=CylinderAsset.Object;
 LoadStory();
}
void AVincentGameMode::BeginPlay(){
 Super::BeginPlay();
 LoadVisuals();
 if(auto* S=Cast<UVincentSave>(UGameplayStatics::LoadGameFromSlot(TEXT("VincentD"),0))){Chapter=FMath::Clamp(S->Chapter,0,2);Solved=S->Solved&7;DoorOpen=S->Open&&Solved==7;Ending=(Chapter==2&&DoorOpen)?FMath::Clamp(S->Ending,0,2):0;}
 BuildRoom();
 GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this,[this](){
  if(auto* PC=UGameplayStatics::GetPlayerController(this,0)){
   UI=CreateWidget<UVincentUI>(PC,UVincentUI::StaticClass());if(UI){UI->AddToViewport();Refresh();UI->StartIntro(Ending?TEXT("Chiến dịch đã hoàn thành. Chọn Bắt đầu lại để chơi lại."):Chapters[Chapter].Intro);}
   if(auto* Pawn=PC->GetPawn()){Pawn->SetActorLocation(FVector(0,350,100),false,nullptr,ETeleportType::TeleportPhysics);PC->SetControlRotation(FRotator(0,-90,0));}
  }
 }));
}
AActor* AVincentGameMode::Box(FVector Location,FVector Scale,FName Key,int32 MaterialIndex){
 AActor* Actor=GetWorld()->SpawnActor<AActor>();if(!Actor)return nullptr;
 auto* Mesh=NewObject<UStaticMeshComponent>(Actor);Actor->SetRootComponent(Mesh);const bool Detailed=Scale.GetAbsMax()<3.f;
 Mesh->SetStaticMesh(Detailed&&DetailCube?DetailCube:Cube);
 int32 M=MaterialIndex;
 if(M<0){M=3;if(Scale.X>=7||Scale.Y>=7)M=Location.Z<0?1:0;else if(Key=="desk")M=2;else if(Key=="clock")M=4;}
 if(Surfaces.IsValidIndex(M)&&Surfaces[M])Mesh->SetMaterial(0,Surfaces[M]);
 Mesh->SetMobility(EComponentMobility::Movable);Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);Mesh->SetCollisionResponseToAllChannels(ECR_Block);Mesh->RegisterComponent();Actor->SetActorLocation(Location);Actor->SetActorScale3D(Scale);if(!Key.IsNone())Actor->Tags.Add(Key);RoomActors.Add(Actor);return Actor;
}
void AVincentGameMode::Label(FVector Location,const FString& Value,float Size,FName Key){
 auto* Actor=GetWorld()->SpawnActor<AActor>();if(!Actor)return;auto* Text=NewObject<UTextRenderComponent>(Actor);Actor->SetRootComponent(Text);Text->SetText(FText::FromString(Value));Text->SetWorldSize(Size);Text->SetHorizontalAlignment(EHTA_Center);Text->SetVerticalAlignment(EVRTA_TextCenter);Text->SetTextRenderColor(Chapters[Chapter].Color.ToFColor(true));Text->SetCollisionEnabled(ECollisionEnabled::NoCollision);Text->RegisterComponent();Actor->SetActorLocationAndRotation(Location,FRotator(0,90,0));if(!Key.IsNone())Actor->Tags.Add(Key);RoomActors.Add(Actor);
}
void AVincentGameMode::LoadVisuals(){
 Surfaces.Reset();for(const TCHAR* Name:{TEXT("Plaster"),TEXT("Tile"),TEXT("Oak"),TEXT("PaintedMetal"),TEXT("Steel"),TEXT("Fabric")}){
  const FString Path=FString::Printf(TEXT("/Game/Vincent/Materials/M_%s.M_%s"),Name,Name);
  Surfaces.Add(LoadObject<UMaterialInterface>(nullptr,*Path,nullptr,LOAD_NoWarn));
 }
 DetailCube=LoadObject<UStaticMesh>(nullptr,TEXT("/Game/Vincent/Meshes/SM_BevelCube.SM_BevelCube"),nullptr,LOAD_NoWarn);
}
void AVincentGameMode::BuildRoom(){
 for(auto* A:RoomActors)if(IsValid(A))A->Destroy();RoomActors.Reset();DoorActor=nullptr;
 const auto& C=Chapters[Chapter];
 Box(FVector(0,0,-10),FVector(10,10,.2));Box(FVector(0,0,360),FVector(10,10,.2));for(float X:{-500.f,500.f}){for(float Y:{-292.5f,292.5f})Box(FVector(X,Y,175),FVector(.2,4.15,3.5));Box(FVector(X,0,307.5),FVector(.2,1.7,.85));}Box(FVector(0,500,175),FVector(10,.2,3.5));
 // Rear wall split around the exit; the doorway remains a real physical gap.
 Box(FVector(-150,-500,175),FVector(7,.2,3.5));Box(FVector(425,-500,175),FVector(1.5,.2,3.5));Box(FVector(275,-500,325),FVector(1.5,.2,.5));
 auto Light=[this](FVector L,FLinearColor Color,float Intensity,bool Shadows=false){auto* A=GetWorld()->SpawnActor<APointLight>(L,FRotator::ZeroRotator);if(A){A->GetLightComponent()->SetMobility(EComponentMobility::Movable);A->GetPointLightComponent()->SetLightColor(Color);A->GetPointLightComponent()->SetIntensity(Intensity);A->GetPointLightComponent()->SetAttenuationRadius(1100);A->GetPointLightComponent()->SetCastShadows(Shadows);RoomActors.Add(A);}};
 Light(FVector(-100,-100,310),FLinearColor(1.f,.9f,.77f),18000,true);Light(FVector(300,-430,180),FLinearColor(.74f,.82f,.88f),1800);Light(FVector(0,350,250),FLinearColor(.4f,.5f,.6f),5000);
 Box(FVector(-250,-478,220),FVector(2,.12,1.35),"painting");Label(FVector(-250,-468,220),C.Board,19,"painting");
 auto* Face=Box(FVector(0,-478,230),FVector(.9,.08,.9),"clock");if(Face){auto* M=Cast<UStaticMeshComponent>(Face->GetRootComponent());M->SetStaticMesh(Cylinder);Face->SetActorScale3D(FVector(.9,.9,.07));Face->SetActorRotation(FRotator(0,0,90));}
 for(int32 I=1;I<=12;I++){float A=FMath::DegreesToRadians(I*30.f);Label(FVector(FMath::Sin(A)*34,-469,230+FMath::Cos(A)*34),FString::FromInt(I),8);}
 Box(FVector(0,-463,244),FVector(.025,.025,.28),"clock");const float A=FMath::DegreesToRadians(C.Hour*30.f);auto* Hand=Box(FVector(FMath::Sin(A)*11,-460,230+FMath::Cos(A)*11),FVector(.04,.025,.22),"clock");if(Hand)Hand->SetActorRotation(FRotator(-C.Hour*30.f,0,0));
 Box(FVector(-250,-330,95),FVector(2.3,1.1,.13),"desk",2);for(float X:{-350.f,-150.f})for(float Y:{-370.f,-290.f})Box(FVector(X,Y,45),FVector(.08,.08,.9));Box(FVector(-190,-330,65),FVector(.7,.9,.45),"desk");
 // Upright note is readable without crouching; the desktop and note both inspect the same puzzle.
 Box(FVector(-250,-351,126),FVector(1.45,.08,.48),"desk");Label(FVector(-250,-345,126),C.Note,11,"desk");
 Box(FVector(-330,-310,115),FVector(.3,.18,.25),"radio");Label(FVector(-330,-299,115),TEXT("AN / 03"),5,"radio");
 DoorActor=Box(FVector(275,-485,140),FVector(1.45,.12,2.8),"door");Label(FVector(275,-470,230),FString::Printf(TEXT("D / 0%d"),Chapter+1),25,"door");Box(FVector(322,-469,135),FVector(.16,.08,.3),"door");
 // Connected villa wings: living, dining, library, kitchen and master bedroom.
 for(float X:{-900.f,900.f}){Box(FVector(X,0,-10),FVector(8,10,.2),NAME_None,2);Box(FVector(X,0,360),FVector(8,10,.2),NAME_None,0);Box(FVector(X,-900,-10),FVector(8,8,.2),NAME_None,2);Box(FVector(X,-900,360),FVector(8,8,.2),NAME_None,0);Box(FVector(X,500,175),FVector(8,.2,3.5),NAME_None,0);for(float Offset:{-242.5f,242.5f})Box(FVector(X+Offset,-500,175),FVector(3.15,.2,3.5),NAME_None,0);Box(FVector(X,-500,307.5),FVector(1.7,.2,.85),NAME_None,0);Light(FVector(X,0,300),FLinearColor(1.f,.9f,.78f),16000);Light(FVector(X,-900,300),FLinearColor(.9f,.93f,1.f),15000);}
 for(float X:{-1300.f,1300.f})Box(FVector(X,-400,175),FVector(.2,18,3.5),NAME_None,0);Box(FVector(0,-1300,175),FVector(26,.2,3.5),NAME_None,0);Box(FVector(0,-900,-10),FVector(10,8,.2),NAME_None,2);Box(FVector(0,-900,360),FVector(10,8,.2),NAME_None,0);Light(FVector(0,-900,300),FLinearColor(1.f,.9f,.8f),14000);
 for(float X:{-500.f,500.f}){for(float Y:{-1142.5f,-657.5f})Box(FVector(X,Y,175),FVector(.2,3.15,3.5),NAME_None,0);Box(FVector(X,-900,307.5),FVector(.2,1.7,.85),NAME_None,0);}
 Label(FVector(-900,-480,230),TEXT("LIBRARY"),20);Label(FVector(900,-480,230),TEXT("KITCHEN"),20);
 // Soft chamfered seating, coffee table and an oval dining table.
 Box(FVector(-1060,210,40),FVector(2.7,.95,.35),NAME_None,5);Box(FVector(-1060,255,90),FVector(2.7,.15,.85),NAME_None,5);Box(FVector(-1200,210,70),FVector(.2,.95,.65),NAME_None,5);Box(FVector(-920,210,70),FVector(.2,.95,.65),NAME_None,5);Box(FVector(-1000,0,50),FVector(1.7,1,.06),NAME_None,2);
 auto* Table=Box(FVector(900,0,78),FVector(2.3,3.3,.07),NAME_None,2);if(Table){auto* M=Cast<UStaticMeshComponent>(Table->GetRootComponent());M->SetStaticMesh(Cylinder);}Box(FVector(900,0,38),FVector(.25,.25,.75),NAME_None,4);
 for(float X:{755.f,1045.f})for(float Y:{-95.f,95.f}){Box(FVector(X,Y,48),FVector(.6,.6,.1),NAME_None,5);Box(FVector(X,Y+26,80),FVector(.6,.1,.55),NAME_None,5);}
 // Library shelving and reading desk.
 for(float X:{-1170.f,-960.f,-750.f}){Box(FVector(X,-1260,130),FVector(1.75,.4,2.6),NAME_None,2);for(int32 Row=0;Row<5;Row++)for(int32 B=0;B<12;B++)Box(FVector(X-65+B*11,-1230,45+Row*43),FVector(.055,.22,.29),NAME_None,B%2?2:3);}
 Box(FVector(-900,-900,80),FVector(2.2,1,.08),NAME_None,2);for(float X:{-990.f,-810.f})for(float Y:{-935.f,-865.f})Box(FVector(X,Y,39),FVector(.07,.07,.78),NAME_None,4);
 // Kitchen fronts, stone worktop, metal pulls and an island.
 for(int32 I=0;I<5;I++){float X=620+I*125;Box(FVector(X,-1250,43),FVector(1.22,.7,.86),NAME_None,3);Box(FVector(X,-1250,89),FVector(1.25,.75,.05),NAME_None,1);Box(FVector(X,-1211,70),FVector(.23,.04,.025),NAME_None,4);}Box(FVector(900,-900,45),FVector(2.4,1.05,.9),NAME_None,2);Box(FVector(900,-900,94),FVector(2.55,1.18,.05),NAME_None,1);
 // Master bedroom and padded headboard.
 Box(FVector(0,-1100,20),FVector(2.3,2.7,.4),NAME_None,2);Box(FVector(0,-1100,52),FVector(2.2,2.6,.3),NAME_None,5);Box(FVector(0,-1235,82),FVector(2.4,.2,1.4),NAME_None,5);for(float X:{-58.f,58.f})Box(FVector(X,-1190,75),FVector(.85,.5,.18),NAME_None,5);
 // Architectural trim and ceiling rails; furniture is built at human scale.
 for(float X:{-487.f,487.f}){for(float Y:{-292.5f,292.5f}){Box(FVector(X,Y,55),FVector(.06,4.15,1.1),NAME_None,3);Box(FVector(X,Y,7),FVector(.07,4.15,.14),NAME_None,4);}}
 Box(FVector(-150,-487,55),FVector(7,.05,1.1),NAME_None,3);Box(FVector(425,-487,55),FVector(1.5,.05,1.1),NAME_None,3);
 for(float X:{-400.f,-200.f,0.f,200.f,400.f})Box(FVector(X,0,345),FVector(.025,9.8,.025),NAME_None,4);
 for(float Y:{-300.f,0.f,300.f}){Box(FVector(0,Y,337),FVector(1.4,.4,.08),NAME_None,3);Box(FVector(0,Y,331),FVector(1.2,.29,.025),NAME_None,4);}
 // Left-wall window assembly with blinds and broad daylight illumination.
 Box(FVector(-480,-180,230),FVector(.08,2.7,1.55),NAME_None,4);
 for(int32 I=0;I<11;I++)Box(FVector(-466,-180,167+I*12.6f),FVector(.16,2.5,.025),NAME_None,4);
 Box(FVector(-470,-180,152),FVector(.26,2.85,.07),NAME_None,4);
 auto* Day=GetWorld()->SpawnActor<ARectLight>(FVector(-445,-180,230),FRotator(0,0,0));if(Day){auto* L=Day->GetRectLightComponent();L->SetMobility(EComponentMobility::Movable);L->SetLightColor(FLinearColor(.78f,.86f,1.f));L->SetIntensity(1400);L->SetSourceWidth(220);L->SetSourceHeight(130);L->SetAttenuationRadius(1000);L->SetCastShadows(true);RoomActors.Add(Day);}
 // Desktop monitor, keyboard, desk lamp and cable support.
 Box(FVector(-185,-365,130),FVector(.65,.06,.42),"desk",3);Label(FVector(-185,-360,130),TEXT("JANUS / OFFLINE"),7,"desk");Box(FVector(-185,-365,106),FVector(.045,.045,.18),NAME_None,4);Box(FVector(-185,-365,98),FVector(.26,.18,.025),NAME_None,3);
 Box(FVector(-190,-305,103),FVector(.53,.18,.025),"desk",3);for(int32 I=0;I<10;I++)Box(FVector(-211+I*4.6,-305,105),FVector(.025,.14,.008),NAME_None,4);
 Box(FVector(-340,-305,102),FVector(.18,.18,.025),NAME_None,4);Box(FVector(-340,-305,128),FVector(.02,.02,.5),NAME_None,4);Box(FVector(-330,-305,155),FVector(.25,.14,.06),NAME_None,3);
 // Upholstered chair and five-spoke rolling base.
 Box(FVector(-250,-185,51),FVector(.58,.58,.12),NAME_None,5);Box(FVector(-250,-158,83),FVector(.58,.12,.56),NAME_None,5);Box(FVector(-250,-185,25),FVector(.04,.04,.4),NAME_None,4);
 for(int32 I=0;I<5;I++){const float R=I*2.f*PI/5;auto* Leg=Box(FVector(-250+FMath::Sin(R)*15,-185+FMath::Cos(R)*15,8),FVector(.04,.32,.025),NAME_None,4);if(Leg)Leg->SetActorRotation(FRotator(0,-FMath::RadiansToDegrees(R),0));Box(FVector(-250+FMath::Sin(R)*29,-185+FMath::Cos(R)*29,4),FVector(.065,.065,.07),NAME_None,3);}
 // Ventilation cover and utility cabinet.
 Box(FVector(130,-480,300),FVector(.9,.07,.3),NAME_None,4);for(int32 I=0;I<8;I++)Box(FVector(130,-475,288+I*3),FVector(.77,.04,.01),NAME_None,3);
 for(int32 I=0;I<3;I++){Box(FVector(430,0,30+I*52),FVector(1,.7,.5),NAME_None,3);Box(FVector(430,37,45+I*52),FVector(.22,.035,.03),NAME_None,4);}
 if(Chapter==0){for(int32 I=0;I<3;I++)Box(FVector(-420,0,28+I*56),FVector(.8,.8,.55));}
 if(Chapter==1){for(float Y:{-100.f,120.f}){Box(FVector(-425,Y,125),FVector(1.1,.8,2.5));for(int32 I=0;I<7;I++){Box(FVector(-425,Y+43,30+I*30),FVector(.9,.05,.18));Label(FVector(-425,Y+48,30+I*30),TEXT("|||     ."),10);}}}
 if(Chapter==2){auto* Core=Box(FVector(-410,0,100),FVector(1.2,1.2,2));if(Core){Cast<UStaticMeshComponent>(Core->GetRootComponent())->SetStaticMesh(Cylinder);}Label(FVector(-410,65,130),TEXT("JANUS\n1010"),26);for(float Z:{45.f,95.f,145.f})Box(FVector(-410,0,Z),FVector(1.4,1.4,.05));}
 if(DoorOpen&&DoorActor){DoorActor->SetActorLocation(FVector(405,-450,140));DoorActor->SetActorRotation(FRotator(0,85,0));}
 if(auto* PC=UGameplayStatics::GetPlayerController(this,0)){if(auto* P=Cast<AVincentPawn>(PC->GetPawn())){P->GetCharacterMovement()->StopMovementImmediately();P->SetActorLocation(FVector(0,350,100),false,nullptr,ETeleportType::TeleportPhysics);PC->SetControlRotation(FRotator(0,-90,0));}}
}
void AVincentGameMode::Save(){auto* S=Cast<UVincentSave>(UGameplayStatics::CreateSaveGameObject(UVincentSave::StaticClass()));if(S){S->Chapter=Chapter;S->Solved=Solved;S->Open=DoorOpen;S->Ending=Ending;UGameplayStatics::SaveGameToSlot(S,TEXT("VincentD"),0);}}
void AVincentGameMode::Refresh(){if(!UI)return;int32 Count=0;FString Digits;for(int32 I=0;I<3;I++){bool Known=(Solved&(1<<I))!=0;Count+=Known?1:0;Digits+=(Known?Chapters[Chapter].Puzzles[I].Digit:TEXT("?"))+TEXT("  ");}UI->RefreshStatus(FString::Printf(TEXT("VINCENT D / %s\nChương %d/3 | Câu đố %d/3 | Khóa: %s%s"),*Chapters[Chapter].Name,Chapter+1,Count,*Digits,Ending?TEXT(" | ĐÃ HOÀN THÀNH"):TEXT("")));}
void AVincentGameMode::SetPrompt(const FString& Value){if(UI)UI->SetPrompt(Value);}
bool AVincentGameMode::InDialog()const{return !UI||UI->IsDialogOpen();}
void AVincentGameMode::Inspect(FName Key){
 if(!UI||InDialog()||Ending)return;Active=-1;bDoorInput=false;bAdvance=false;bRestartConfirm=false;
 if(Key=="radio"){Radio();return;}
 if(Key=="door") {if(DoorOpen){bAdvance=true;Continue();return;}bDoorInput=true;UI->Show(TEXT("CỬA JANUS / 3 CHỐT"),TEXT("Giải cả ba câu đố rồi ghép chữ số theo thứ tự: bảng → đồng hồ → tài liệu. Nhập mật mã ba chữ số."),true);return;}
 const TArray<FName> Keys={FName("painting"),FName("clock"),FName("desk")};Active=Keys.IndexOfByKey(Key);if(Active<0)return;const auto& P=Chapters[Chapter].Puzzles[Active];bool Known=(Solved&(1<<Active))!=0;UI->Show(P.Name,Known?P.Result:P.Question,!Known);if(Known)Active=-1;
}
void AVincentGameMode::Submit(const FString& Value){
 if(!UI||Ending)return;FString Answer=Value.TrimStartAndEnd().ToLower();
 if(bDoorInput){if(Solved!=7){UI->Feedback->SetText(FText::FromString(TEXT("Chưa mở đủ ba chốt. Giải các câu đố trước.")));return;}FString Code;for(const auto& P:Chapters[Chapter].Puzzles)Code+=P.Digit;if(Answer!=Code){UI->Feedback->SetText(FText::FromString(TEXT("Mật mã sai. Kiểm tra thứ tự chữ số trong nhật ký.")));return;}DoorOpen=true;bDoorInput=false;bAdvance=true;if(DoorActor){DoorActor->SetActorLocation(FVector(405,-450,140));DoorActor->SetActorRotation(FRotator(0,85,0));}UI->Show(TEXT("CỬA ĐÃ MỞ"),Chapters[Chapter].Reveal);Refresh();Save();return;}
 if(Active<0||Active>=3)return;const auto& P=Chapters[Chapter].Puzzles[Active];if(Answer!=P.Answer){UI->Feedback->SetText(FText::FromString(TEXT("Chưa đúng. Quan sát lại hoặc dùng Gợi ý; không mất lượt.")));return;}Solved|=1<<Active;Active=-1;UI->Show(TEXT("MỘT MẢNH GHÉP ĐÃ SÁNG TỎ"),P.Result);Refresh();Save();
}
void AVincentGameMode::Continue(){
 if(!UI)return;
 if(bRestartConfirm){Chapter=0;Solved=0;DoorOpen=false;Ending=0;Active=-1;bDoorInput=false;bAdvance=false;bRestartConfirm=false;HintIndex=0;BuildRoom();Save();Refresh();UI->Show(TEXT("CHƯƠNG 1 / PHÒNG LƯU TRỮ"),Chapters[0].Intro);return;}
 if(bAdvance){bAdvance=false;if(Chapter==2){UI->Choice();return;}Chapter++;Solved=0;DoorOpen=false;Active=-1;HintIndex=0;BuildRoom();Save();Refresh();UI->Show(Chapters[Chapter].Name,Chapters[Chapter].Intro);return;}
 UI->Close();
}
void AVincentGameMode::Radio(){if(!UI)return;Active=-1;bDoorInput=false;bAdvance=false;bRestartConfirm=false;UI->Show(TEXT("RADIO / AN"),Chapters[Chapter].Radio);}
void AVincentGameMode::Hint(){if(!UI)return;TArray<int32> Left;for(int32 I=0;I<3;I++)if(!(Solved&(1<<I)))Left.Add(I);const FString Message=Left.Num()?Chapters[Chapter].Puzzles[Left[HintIndex++%Left.Num()]].Hint:TEXT("Ghép ba chữ số theo thứ tự bảng, đồng hồ, tài liệu rồi nhập tại cửa.");UI->Feedback->SetText(FText::FromString(Message));}
void AVincentGameMode::Restart(){if(!UI)return;Active=-1;bDoorInput=false;bAdvance=false;bRestartConfirm=true;UI->Show(TEXT("XÁC NHẬN CHƠI LẠI"),TEXT("Bấm Tiếp tục để xóa tiến trình và về chương 1. Bấm Radio để hủy thao tác này."));}
void AVincentGameMode::Finish(int32 Choice){if(!UI||Chapter!=2||!DoorOpen||Ending)return;Ending=Choice==1?1:2;bAdvance=false;UI->Show(Ending==1?TEXT("KẾT THÚC / CÙNG NHAU"):TEXT("KẾT THÚC / NGƯỜI MANG SỰ THẬT"),Ending==1?TEXT("Bạn mang ý thức của An ra ngoài và mất bằng chứng JANUS. Radio vang lên: Lần này, chúng ta cùng đi. Vincent bước ra bình minh cùng một lời hứa."):TEXT("Bạn giữ hồ sơ để phơi bày JANUS. An nói: Đừng để họ làm điều này với ai nữa. Cánh cửa mở, nhưng bạn bước ra một mình."));Save();Refresh();}
