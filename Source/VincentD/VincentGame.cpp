#include "VincentGame.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/PointLightComponent.h"
#include "Engine/PointLight.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Components/SkyLightComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

namespace {const TCHAR* LifeSlot=TEXT("VincentLife_v1");}
AVincentGameMode::AVincentGameMode(){
 DefaultPawnClass=AVincentPawn::StaticClass();
 static ConstructorHelpers::FObjectFinder<UStaticMesh> C(TEXT("/Engine/BasicShapes/Cube.Cube"));Cube=C.Object;
 static ConstructorHelpers::FObjectFinder<UStaticMesh> Y(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));Cylinder=Y.Object;
}
void AVincentGameMode::BeginPlay(){
 Super::BeginPlay();LoadVisuals();Load();BuildNeighborhood();
 GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this,[this](){
  if(auto* PC=UGameplayStatics::GetPlayerController(this,0)){
   UI=CreateWidget<UVincentUI>(PC,UVincentUI::StaticClass());if(UI){UI->AddToViewport();Refresh();UI->StartIntro(OpeningText());}
   if(auto* P=PC->GetPawn()){P->SetActorLocation(FVector(0,0,110),false,nullptr,ETeleportType::TeleportPhysics);PC->SetControlRotation(FRotator(0,90,0));}
  }
 }));
 GetWorldTimerManager().SetTimer(LifeClockTimer,this,&AVincentGameMode::TickLifeClock,1.f,true);
}
void AVincentGameMode::TickLifeClock(){if(InDialog())return;VincentLife::Advance(Progress,1);if(++ClockTicks%10==0)Save();}
AActor* AVincentGameMode::Box(FVector Location,FVector Scale,FName Key,int32 MaterialIndex){
 auto* A=GetWorld()->SpawnActor<AActor>();if(!A)return nullptr;
 auto* M=NewObject<UStaticMeshComponent>(A);A->SetRootComponent(M);M->SetStaticMesh(Scale.GetAbsMax()<3.f&&DetailCube?DetailCube:Cube);
 const int32 Index=MaterialIndex<0?3:MaterialIndex;if(Surfaces.IsValidIndex(Index)&&Surfaces[Index])M->SetMaterial(0,Surfaces[Index]);
 M->SetMobility(EComponentMobility::Movable);M->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);M->SetCollisionResponseToAllChannels(ECR_Block);M->RegisterComponent();
 A->SetActorLocation(Location);A->SetActorScale3D(Scale);if(!Key.IsNone())A->Tags.Add(Key);RoomActors.Add(A);return A;
}
void AVincentGameMode::Label(FVector Location,const FString& Value,float Size,FName Key){
 auto* A=GetWorld()->SpawnActor<AActor>();if(!A)return;auto* T=NewObject<UTextRenderComponent>(A);A->SetRootComponent(T);T->SetText(FText::FromString(Value));T->SetWorldSize(Size);T->SetHorizontalAlignment(EHTA_Center);T->SetVerticalAlignment(EVRTA_TextCenter);T->SetTextRenderColor(FColor(225,217,195));T->SetCollisionEnabled(ECollisionEnabled::NoCollision);T->RegisterComponent();A->SetActorLocationAndRotation(Location,FRotator(0,-90,0));if(!Key.IsNone())A->Tags.Add(Key);RoomActors.Add(A);
}
void AVincentGameMode::LoadVisuals(){
 Surfaces.Reset();for(const TCHAR* Name:{TEXT("Plaster"),TEXT("Tile"),TEXT("Oak"),TEXT("PaintedMetal"),TEXT("Steel"),TEXT("Fabric")}){const FString Path=FString::Printf(TEXT("/Game/Vincent/Materials/M_%s.M_%s"),Name,Name);Surfaces.Add(LoadObject<UMaterialInterface>(nullptr,*Path,nullptr,LOAD_NoWarn));}
 DetailCube=LoadObject<UStaticMesh>(nullptr,TEXT("/Game/Vincent/Meshes/SM_BevelCube.SM_BevelCube"),nullptr,LOAD_NoWarn);
}
void AVincentGameMode::BuildNeighborhood(){
 for(auto* A:RoomActors)if(IsValid(A))A->Destroy();RoomActors.Reset();PlantActor=nullptr;
 // A small test alley, not the final art environment. Discarded PhuQuocCity assets are not used.
 Box(FVector(0,700,-15),FVector(40,40,.3),NAME_None,1);
 for(float X:{-1985.f,1985.f})Box(FVector(X,700,60),FVector(.3,40,1.2),NAME_None,0);for(float Y:{-1285.f,2685.f})Box(FVector(0,Y,60),FVector(40,.3,1.2),NAME_None,0);
 Box(FVector(0,800,1),FVector(5,30,.02),NAME_None,3);
 for(float X:{-275.f,275.f})Box(FVector(X,800,9),FVector(.5,30,.18),NAME_None,1);
 // Duy's room: bed, work table, notebook, plant position and a passable front door.
 Box(FVector(0,0,0),FVector(8,7,.05),NAME_None,2);
 Box(FVector(0,-350,155),FVector(8,.15,3.1),NAME_None,0);
 for(float X:{-400.f,400.f})Box(FVector(X,0,155),FVector(.15,7,3.1),NAME_None,0);
 for(float X:{-240.f,240.f})Box(FVector(X,350,155),FVector(3.2,.15,3.1),NAME_None,0);
 Box(FVector(0,350,280),FVector(1.6,.15,.6),NAME_None,0);
 Box(FVector(0,0,320),FVector(8,7,.1),NAME_None,0);
 Box(FVector(-240,-180,28),FVector(1.5,2.1,.45),"bed",2);Box(FVector(-240,-180,60),FVector(1.4,2,.2),"bed",5);Box(FVector(-240,-265,78),FVector(1.2,.4,.14),"bed",5);
 Box(FVector(220,-180,78),FVector(1.6,.65,.07),"home",2);for(float X:{155.f,285.f})Box(FVector(X,-180,38),FVector(.06,.5,.76),NAME_None,4);
 Box(FVector(200,-180,84),FVector(.26,.19,.025),"notebook",2);Box(FVector(260,-180,86),FVector(.28,.15,.045),"home",3);
 Label(FVector(0,337,245),TEXT("PHÒNG CỦA DUY / 21 TUỔI"),17);
 PlantActor=Box(FVector(315,-260,72),FVector(.25,.25,.35),NAME_None,2);if(PlantActor){PlantActor->SetActorHiddenInGame(!Progress.OwnsPlant);PlantActor->SetActorEnableCollision(Progress.OwnsPlant);}
 auto Light=[this](FVector P){auto* L=GetWorld()->SpawnActor<APointLight>(P,FRotator::ZeroRotator);if(L){L->GetPointLightComponent()->SetIntensity(16000);L->GetPointLightComponent()->SetAttenuationRadius(1000);L->GetPointLightComponent()->SetLightColor(FLinearColor(1,.89f,.73f));RoomActors.Add(L);}};
 Light(FVector(0,0,275));
 auto* Sun=GetWorld()->SpawnActor<ADirectionalLight>(FVector(0,0,1000),FRotator(-40,-35,0));if(Sun){Sun->GetLightComponent()->SetMobility(EComponentMobility::Movable);Sun->GetLightComponent()->SetIntensity(3);RoomActors.Add(Sun);}
 auto* Sky=GetWorld()->SpawnActor<ASkyLight>();if(Sky){Sky->GetLightComponent()->SetMobility(EComponentMobility::Movable);Sky->GetLightComponent()->SetIntensity(1);Sky->GetLightComponent()->SetLowerHemisphereColor(FLinearColor(.3f,.32f,.35f));RoomActors.Add(Sky);}
 // Open-front storefronts are interaction points; final characters/models are a separate art milestone.
 auto Shop=[this,&Light](float X,float Y,const FString& Name,FName Key){Box(FVector(X,Y,-1),FVector(5,4,.08),NAME_None,1);Box(FVector(X,Y+195,150),FVector(5,.1,3),NAME_None,0);for(float Side:{-245.f,245.f})Box(FVector(X+Side,Y,150),FVector(.1,4,3),NAME_None,0);Box(FVector(X,Y,310),FVector(5.3,4.3,.15),NAME_None,0);Box(FVector(X,Y-150,85),FVector(2,.6,.08),Key,2);Box(FVector(X,Y-150,125),FVector(.45,.08,.6),Key,3);Label(FVector(X,Y-188,235),Name,24);Light(FVector(X,Y,250));};
 Shop(-800,700,TEXT("QUÁN MẸ HẠNH"),"cafe");Shop(800,700,TEXT("TIỆM CHÚ BẢY"),"scooter");Shop(800,1600,TEXT("NHÀ SỐ 17 / NHẬN HÀNG"),"customer");
 Box(FVector(-260,2150,100),FVector(.12,1,2),"bus",3);Label(FVector(-250,2080,215),TEXT("BẾN XE / HẢI MINH"),20,"bus");
 Box(FVector(-800,1000,110),FVector(.8,.2,1),"food",2);Label(FVector(-800,985,145),TEXT("SUẤT ĂN 20.000"),12,"food");
 Box(FVector(-500,1500,70),FVector(.8,.8,1.4),"decor",2);Label(FVector(-500,1455,125),TEXT("CÂY CẢNH 30.000"),12,"decor");
 if(auto* PC=UGameplayStatics::GetPlayerController(this,0)){if(auto* P=Cast<AVincentPawn>(PC->GetPawn())){P->GetCharacterMovement()->StopMovementImmediately();P->SetActorLocation(FVector(0,0,110),false,nullptr,ETeleportType::TeleportPhysics);PC->SetControlRotation(FRotator(0,90,0));}}
}
void AVincentGameMode::Save(){if(!VincentLife::Valid(Progress))return;auto* S=Cast<UVincentSave>(UGameplayStatics::CreateSaveGameObject(UVincentSave::StaticClass()));if(!S)return;
 S->Money=Progress.Money;S->Day=Progress.Day;S->Minute=Progress.Minute;S->Energy=Progress.Energy;S->Hunger=Progress.Hunger;S->Stage=int(Progress.Current);S->Route=int(Progress.Direction);S->MotherTrust=Progress.MotherTrust;S->MechanicTrust=Progress.MechanicTrust;S->FriendTrust=Progress.FriendTrust;S->LastExtraShiftDay=Progress.LastExtraShiftDay;S->BikeReady=Progress.BikeReady;S->HasParcel=Progress.HasParcel;S->FoundNotebook=Progress.FoundNotebook;S->OwnsPlant=Progress.OwnsPlant;UGameplayStatics::SaveGameToSlot(S,LifeSlot,0);
}
void AVincentGameMode::Load(){auto* S=Cast<UVincentSave>(UGameplayStatics::LoadGameFromSlot(LifeSlot,0));if(!S||S->SchemaVersion!=1)return;VincentLife::State Candidate;
 Candidate.Money=S->Money;Candidate.Day=S->Day;Candidate.Minute=S->Minute;Candidate.Energy=S->Energy;Candidate.Hunger=S->Hunger;Candidate.Current=static_cast<VincentLife::Quest>(S->Stage);Candidate.Direction=static_cast<VincentLife::Path>(S->Route);Candidate.MotherTrust=S->MotherTrust;Candidate.MechanicTrust=S->MechanicTrust;Candidate.FriendTrust=S->FriendTrust;Candidate.LastExtraShiftDay=S->LastExtraShiftDay;Candidate.BikeReady=S->BikeReady;Candidate.HasParcel=S->HasParcel;Candidate.FoundNotebook=S->FoundNotebook;Candidate.OwnsPlant=S->OwnsPlant;if(VincentLife::Valid(Candidate))Progress=Candidate;
}
void AVincentGameMode::Refresh(){if(!UI)return;UI->RefreshStatus(FString::Printf(TEXT("DUY / 21 TUỔI | Ngày %d %02d:%02d\nTiền: %lld | Thể lực: %d | No: %d\n%s"),Progress.Day,Progress.Minute/60,Progress.Minute%60,static_cast<long long>(Progress.Money),Progress.Energy,Progress.Hunger,*ObjectiveText()));}
void AVincentGameMode::SetPrompt(const FString& V){if(UI)UI->SetPrompt(V);}
bool AVincentGameMode::InDialog()const{return !UI||UI->IsDialogOpen();}
void AVincentGameMode::ShowOutcome(VincentLife::Result Result,const FString& Text){
 if(!UI)return;using Outcome=VincentLife::Result;
 if(Result==Outcome::Success){Active=NAME_None;Refresh();Save();UI->Show(TEXT("NHẬT KÝ CỦA DUY"),Text+TEXT("\n\n")+ObjectiveText());}
 else{const TCHAR* Why=TEXT("Việc này chưa sẵn sàng. Xem mục tiêu trong điện thoại.");if(Result==Outcome::InvalidAnswer)Why=TEXT("Chưa đúng với thông tin trên phiếu. Hãy đọc lại tình huống và thử tiếp.");if(Result==Outcome::InsufficientFunds)Why=TEXT("Bạn chưa đủ tiền. Không có khoản nào bị trừ.");if(Result==Outcome::Exhausted)Why=TEXT("Bạn cần về giường ngủ để hồi thể lực trước.");if(Result==Outcome::AlreadyDone)Why=TEXT("Bạn đã hoàn thành việc này rồi.");UI->Feedback->SetText(FText::FromString(Why));}
}
void AVincentGameMode::Inspect(FName Key){
 if(!UI||InDialog())return;Active=NAME_None;bRestartConfirm=false;
 if(Key=="home"){const auto R=VincentLife::StartDay(Progress);Refresh();Save();UI->Show(TEXT("CĂN PHÒNG ĐẦU TIÊN"),R==VincentLife::Result::Success?TEXT("Bạn đặt ba lô xuống. Mẹ nhắn đang cần người phụ quán sáng. Trên bàn còn quyển sổ cũ; tối về hãy đọc thêm."):ObjectiveText());return;}
 if(Key=="notebook"){Progress.FoundNotebook=true;Save();UI->Show(TEXT("QUYỂN SỔ CŨ"),TEXT("Một khoản tiền sửa khu sinh hoạt được ghi đã thanh toán, nhưng công trình chưa từng hoàn thành. Trang cuối có tên vài người trong xóm. Đây là manh mối cho tuyến truyện mở rộng; chưa có đủ bằng chứng để kết luận ai lấy tiền."));return;}
 if(Key=="bed"){VincentLife::Sleep(Progress);Refresh();Save();UI->Show(TEXT("NGHỈ NGƠI"),TEXT("Bạn ngủ tám giờ. Thể lực đã hồi phục, thời gian trong xóm tiếp tục trôi."));return;}
 if(Key=="bus"){if(Progress.Current==VincentLife::Quest::Decision){UI->Choice();}else{Phone();}return;}
 if(Key=="decor"){UI->Show(TEXT("QUẦY CÂY CẢNH"),TEXT("Một chậu cây giá 30.000 đồng. Nhập MUA để mua; cây sẽ xuất hiện ở vị trí đặt sẵn trong phòng. Đây chưa phải hệ thống kéo/thả nội thất."),true);Active=Key;return;}
 if(Key=="food"){UI->Show(TEXT("SUẤT ĂN"),TEXT("Một suất ăn giá 20.000 đồng, hồi mức no. Nhập AN để mua."),true);Active=Key;return;}
 if(Key=="cafe"&&Progress.Current!=VincentLife::Quest::Breakfast){UI->Show(TEXT("CA LÀM THÊM / MẸ HẠNH"),TEXT("Sau ca đầu, bạn có thể làm thêm một ca mỗi ngày để kiếm 40.000 đồng. Cần 20 thể lực, mất hai giờ. Nhập LAM THEM; nếu đã làm hôm nay, hãy quay lại ngày khác."),true);Active=Key;return;}
 if(Key=="cafe"){UI->Show(TEXT("CA SÁNG / MẸ HẠNH"),TEXT("Mẹ: Không cần nhanh nhất, nhớ khách gọi gì trước đã. Phiếu bàn số 2 ghi: hai tô mì. Hãy nhập số tô cần mang. Tiền công ca đầu: 70.000 đồng."),true);Active=Key;return;}
 if(Key=="scooter"){UI->Show(TEXT("SỬA XE / CHÚ BẢY"),TEXT("Đèn xe yếu, tiếng đề lạch cạch. Chú Bảy đo điện áp bình thấp; bình xăng vẫn đủ. Nhập BINH DIEN để thay đúng bộ phận. Phí sửa 50.000 đồng; xong việc chú gửi một gói hàng D-03 cho nhà số 17."),true);Active=Key;return;}
 if(Key=="customer"){UI->Show(TEXT("GIAO HÀNG / NHÀ SỐ 17"),TEXT("Người nhận kiểm tra phiếu. Nhập mã gói hàng được chú Bảy giao để xác nhận đúng kiện. Tiền công: 90.000 đồng. Bản này giao bằng đi bộ, chưa có hệ thống lái xe hoàn chỉnh."),true);Active=Key;}
}
void AVincentGameMode::Submit(const FString& Value){if(!UI)return;const FString Answer=Value.TrimStartAndEnd().ToLower();
 if(Active=="cafe"&&(Answer==TEXT("lam them")||Answer==TEXT("làm thêm"))){ShowOutcome(VincentLife::WorkExtraShift(Progress),TEXT("Bạn hoàn thành ca làm thêm. Nhận 40.000 đồng; hôm nay không thể nhận thêm ca này."));return;}
 if(Active=="cafe"){ShowOutcome(VincentLife::ServeBreakfast(Progress,Answer==TEXT("2")?2:0),TEXT("Bạn đã phục vụ đúng đơn. Nhận 70.000 đồng và tăng sự tin cậy với mẹ."));return;}
 if(Active=="scooter"){ShowOutcome(VincentLife::RepairScooter(Progress,Answer==TEXT("binh dien")||Answer==TEXT("bình điện")),TEXT("Xe đã sửa theo tình huống thử nghiệm. Trừ 50.000 đồng, nhận gói hàng D-03. Vật lý lái xe chưa được triển khai trong bản này."));return;}
 if(Active=="customer"){ShowOutcome(VincentLife::Deliver(Progress,Answer==TEXT("d-03")),TEXT("Giao đúng gói D-03 tại nhà số 17. Nhận 90.000 đồng. Minh nhắn có chỗ làm thử ở Hải Minh."));return;}
 if(Active=="decor"){if(Answer!=TEXT("mua")){ShowOutcome(VincentLife::Result::InvalidAnswer,TEXT(""));return;}const auto R=VincentLife::BuyPlant(Progress);if(R==VincentLife::Result::Success&&PlantActor){PlantActor->SetActorHiddenInGame(false);PlantActor->SetActorEnableCollision(true);}ShowOutcome(R,TEXT("Bạn mua chậu cây, trừ 30.000 đồng. Góc phòng đã có một thay đổi nhỏ của riêng bạn."));return;}
 if(Active=="food"){ShowOutcome(Answer==TEXT("an")?VincentLife::Eat(Progress):VincentLife::Result::InvalidAnswer,TEXT("Bạn ăn một bữa, trừ 20.000 đồng. Mức no đã hồi phục."));}
}
void AVincentGameMode::Continue(){if(!UI)return;if(bRestartConfirm){Progress=VincentLife::State{};bRestartConfirm=false;Active=NAME_None;BuildNeighborhood();Save();Refresh();UI->Show(TEXT("MỘT NGÀY BẮT ĐẦU LẠI"),OpeningText());return;}UI->Close();}
void AVincentGameMode::Phone(){if(!UI)return;Active=NAME_None;bRestartConfirm=false;Refresh();UI->Show(TEXT("ĐIỆN THOẠI / NHẬT KÝ"),FString::Printf(TEXT("Ngày %d %02d:%02d | Tiền %lld đồng\nThể lực %d/100 · No %d/100\nTin cậy: mẹ %d · chú Bảy %d · Minh %d\n\n%s"),Progress.Day,Progress.Minute/60,Progress.Minute%60,static_cast<long long>(Progress.Money),Progress.Energy,Progress.Hunger,Progress.MotherTrust,Progress.MechanicTrust,Progress.FriendTrust,*ObjectiveText()));}
void AVincentGameMode::Hint(){if(UI)UI->Feedback->SetText(FText::FromString(ObjectiveText()));}
void AVincentGameMode::Restart(){if(!UI)return;Active=NAME_None;bRestartConfirm=true;UI->Show(TEXT("BẮT ĐẦU LẠI?"),TEXT("Bấm Tiếp tục để xóa tiến trình life simulator và về ngày đầu. Mở Điện thoại để hủy."));}
void AVincentGameMode::ChoosePath(int32 Choice){if(!UI)return;const auto Path=Choice==1?VincentLife::Path::Village:VincentLife::Path::City;const auto R=VincentLife::Choose(Progress,Path);ShowOutcome(R,Choice==1?TEXT("Bạn chọn ở lại bắt đầu từ xóm. Chú Bảy đề nghị cùng mở lại góc sửa xe. Có thể đổi hướng trong phần truyện mở rộng; bản thử chưa dựng tuyến đó."):TEXT("Bạn hẹn Minh và đặt kế hoạch lên Hải Minh. Khu thành phố chưa có trong bản thử này; bạn vẫn có thể tiếp tục sinh hoạt tại xóm."));}

FString AVincentGameMode::InteractionName(FName Key)const{
 if(Key=="home")return TEXT("Bàn / bắt đầu ngày mới");if(Key=="bed")return TEXT("Giường / ngủ");if(Key=="notebook")return TEXT("Quyển sổ cũ");if(Key=="cafe")return TEXT("Mẹ Hạnh / ca sáng");if(Key=="scooter")return TEXT("Chú Bảy / kiểm tra xe");if(Key=="customer")return TEXT("Nhà số 17 / giao hàng");if(Key=="bus")return TEXT("Bến xe / tin nhắn Minh");if(Key=="food")return TEXT("Mua suất ăn");if(Key=="decor")return TEXT("Mua cây cảnh");return TEXT("");
}
