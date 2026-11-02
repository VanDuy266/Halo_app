#include "VincentGame.h"
#include "Engine/World.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "Components/ScaleBox.h"
#include "Components/Overlay.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/ButtonSlot.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Styling/CoreStyle.h"
#include "Brushes/SlateRoundedBoxBrush.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

AVincentGameMode* UVincentUI::Game()const{return Cast<AVincentGameMode>(UGameplayStatics::GetGameMode(this));}
TSharedRef<SWidget> UVincentUI::RebuildWidget(){
 if(!WidgetTree)WidgetTree=NewObject<UWidgetTree>(this,TEXT("WidgetTree"));
 if(WidgetTree->RootWidget)return Super::RebuildWidget();
 UCanvasPanel* Root=WidgetTree->ConstructWidget<UCanvasPanel>();WidgetTree->RootWidget=Root;
 auto Text=[this](const FString& Value,int32 Size)->UTextBlock*{
  UTextBlock* T=WidgetTree->ConstructWidget<UTextBlock>();T->SetText(FText::FromString(Value));T->SetAutoWrapText(true);
  T->SetFont(FCoreStyle::GetDefaultFontStyle(Size>=24?TEXT("Bold"):TEXT("Regular"),Size));T->SetLineHeightPercentage(1.45f);T->SetColorAndOpacity(FSlateColor(FLinearColor(.84f,.87f,.88f)));return T;
 };
 Status=Text(TEXT("VINCENT D / CÙNG NHAU BỎ TRỐN"),18);Status->SetVisibility(ESlateVisibility::Collapsed);
 auto* S=Root->AddChildToCanvas(Status);S->SetPosition(FVector2D(26,22));S->SetSize(FVector2D(900,100));
 Prompt=Text(TEXT("+"),16);auto* P=Root->AddChildToCanvas(Prompt);P->SetAnchors(FAnchors(.5f,1.f));P->SetAlignment(FVector2D(.5f,1.f));P->SetPosition(FVector2D(0,-25));P->SetSize(FVector2D(850,60));Prompt->SetJustification(ETextJustify::Center);
 auto* Cross=Text(TEXT("+"),22);auto* X=Root->AddChildToCanvas(Cross);X->SetAnchors(FAnchors(.5f,.5f));X->SetAlignment(FVector2D(.5f,.5f));X->SetAutoSize(true);
 Dialog=WidgetTree->ConstructWidget<UBorder>();Dialog->SetBrushColor(FLinearColor(.025f,.035f,.043f,.98f));Dialog->SetPadding(FMargin(32));auto* D=Root->AddChildToCanvas(Dialog);D->SetAnchors(FAnchors(.5f,.5f));D->SetAlignment(FVector2D(.5f,.5f));D->SetAutoSize(true);
 USizeBox* Width=WidgetTree->ConstructWidget<USizeBox>();Width->SetWidthOverride(640);Dialog->SetContent(Width);
 UVerticalBox* Content=WidgetTree->ConstructWidget<UVerticalBox>();Width->SetContent(Content);
 Heading=Text(TEXT("VINCENT D"),24);Content->AddChildToVerticalBox(Heading)->SetPadding(FMargin(0,0,0,16));
 Body=Text(TEXT(""),16);Content->AddChildToVerticalBox(Body)->SetPadding(FMargin(0,0,0,20));
 Answer=WidgetTree->ConstructWidget<UEditableTextBox>();Answer->SetHintText(FText::FromString(TEXT("Nhập đáp án rồi bấm Kiểm tra")));Content->AddChildToVerticalBox(Answer)->SetPadding(FMargin(0,0,0,12));Answer->SetFont(FCoreStyle::GetDefaultFontStyle(TEXT("Regular"),18));
 Feedback=Text(TEXT(""),16);Content->AddChildToVerticalBox(Feedback);
 auto Button=[this,Content,Text](const FString& Label)->UButton*{auto* B=WidgetTree->ConstructWidget<UButton>();FButtonStyle Style=B->GetStyle();Style.Normal=FSlateRoundedBoxBrush(FLinearColor(.10f,.13f,.15f),5.f);Style.Hovered=FSlateRoundedBoxBrush(FLinearColor(.18f,.22f,.24f),5.f);Style.Pressed=FSlateRoundedBoxBrush(FLinearColor(.06f,.09f,.11f),5.f);B->SetStyle(Style);if(auto* Slot=Cast<UButtonSlot>(B->AddChild(Text(Label,15))))Slot->SetPadding(FMargin(18,11));Content->AddChildToVerticalBox(B)->SetPadding(FMargin(0,3));return B;};
 SubmitButton=Button(TEXT("Kiểm tra đáp án / mật mã"));SubmitButton->OnClicked.AddDynamic(this,&UVincentUI::Submit);
 ContinueButton=Button(TEXT("Tiếp tục / trở về khám phá"));ContinueButton->OnClicked.AddDynamic(this,&UVincentUI::Continue);
 AnButton=Button(TEXT("Mang ý thức của An ra ngoài"));AnButton->OnClicked.AddDynamic(this,&UVincentUI::SaveAn);
 TruthButton=Button(TEXT("Giữ bằng chứng về JANUS"));TruthButton->OnClicked.AddDynamic(this,&UVincentUI::SaveTruth);
 auto* R=Button(TEXT("Radio / nghe lời nhắn của An"));R->OnClicked.AddDynamic(this,&UVincentUI::Radio);
 auto* H=Button(TEXT("Gợi ý câu đố chưa giải"));H->OnClicked.AddDynamic(this,&UVincentUI::Hint);
 auto* Reset=Button(TEXT("Bắt đầu lại chiến dịch (có xác nhận)"));Reset->OnClicked.AddDynamic(this,&UVincentUI::Restart);
 Dialog->SetVisibility(ESlateVisibility::Collapsed);
 // Full-window intro is separate from the gameplay journal; no guide panels surround the view.
 IntroOverlay=WidgetTree->ConstructWidget<UBorder>();IntroOverlay->SetBrushColor(FLinearColor(.003f,.005f,.007f,1.f));
 auto* IntroSlot=Root->AddChildToCanvas(IntroOverlay);IntroSlot->SetAnchors(FAnchors(0,0,1,1));IntroSlot->SetOffsets(FMargin(0));
 auto* IntroCanvas=WidgetTree->ConstructWidget<UCanvasPanel>();IntroOverlay->SetContent(IntroCanvas);
 auto* IntroBox=WidgetTree->ConstructWidget<UVerticalBox>();auto* Center=IntroCanvas->AddChildToCanvas(IntroBox);Center->SetAnchors(FAnchors(.5f,.5f));Center->SetAlignment(FVector2D(.5f,.5f));Center->SetAutoSize(true);
 auto* ImageSize=WidgetTree->ConstructWidget<USizeBox>();ImageSize->SetWidthOverride(620);ImageSize->SetHeightOverride(250);IntroBox->AddChildToVerticalBox(ImageSize);
 auto* ImageScale=WidgetTree->ConstructWidget<UScaleBox>();ImageScale->SetStretch(EStretch::ScaleToFit);ImageSize->SetContent(ImageScale);IntroImage=WidgetTree->ConstructWidget<UImage>();ImageScale->AddChild(IntroImage);
 IntroTitle=Text(TEXT("UNREAL ENGINE"),32);IntroTitle->SetJustification(ETextJustify::Center);IntroBox->AddChildToVerticalBox(IntroTitle)->SetPadding(FMargin(0,18));
 IntroMenu=WidgetTree->ConstructWidget<UVerticalBox>();IntroBox->AddChildToVerticalBox(IntroMenu);
 auto* Play=WidgetTree->ConstructWidget<UButton>();if(auto* PlaySlot=Cast<UButtonSlot>(Play->AddChild(Text(TEXT("Vào game"),20))))PlaySlot->SetPadding(FMargin(24,14));IntroMenu->AddChildToVerticalBox(Play)->SetPadding(FMargin(100,10));Play->OnClicked.AddDynamic(this,&UVincentUI::PlayGame);
 auto* Skip=WidgetTree->ConstructWidget<UButton>();Skip->AddChild(Text(TEXT("Bỏ qua giới thiệu"),13));auto* SkipSlot=IntroCanvas->AddChildToCanvas(Skip);SkipSlot->SetAnchors(FAnchors(1,1));SkipSlot->SetAlignment(FVector2D(1,1));SkipSlot->SetPosition(FVector2D(-30,-25));SkipSlot->SetAutoSize(true);Skip->OnClicked.AddDynamic(this,&UVincentUI::MainMenu);
 IntroOverlay->SetVisibility(ESlateVisibility::Collapsed);
 return Super::RebuildWidget();
}
void UVincentUI::Show(const FString& Title,const FString& Message,bool NeedsInput){
 bDialog=true;Heading->SetText(FText::FromString(Title));Body->SetText(FText::FromString(Message));Feedback->SetText(FText::GetEmpty());Answer->SetText(FText::GetEmpty());
 Answer->SetVisibility(NeedsInput?ESlateVisibility::Visible:ESlateVisibility::Collapsed);SubmitButton->SetVisibility(NeedsInput?ESlateVisibility::Visible:ESlateVisibility::Collapsed);
 AnButton->SetVisibility(ESlateVisibility::Collapsed);TruthButton->SetVisibility(ESlateVisibility::Collapsed);ContinueButton->SetVisibility(ESlateVisibility::Visible);Dialog->SetVisibility(ESlateVisibility::Visible);
 if(auto* PC=GetOwningPlayer()){PC->bShowMouseCursor=true;FInputModeGameAndUI Mode;Mode.SetHideCursorDuringCapture(false);Mode.SetWidgetToFocus(NeedsInput?Answer->TakeWidget():ContinueButton->TakeWidget());PC->SetInputMode(Mode);}
}
void UVincentUI::Close(){bDialog=false;Dialog->SetVisibility(ESlateVisibility::Collapsed);if(auto* PC=GetOwningPlayer()){PC->bShowMouseCursor=false;PC->SetInputMode(FInputModeGameOnly());}}
void UVincentUI::RefreshStatus(const FString& Value){Status->SetText(FText::FromString(Value));}
void UVincentUI::SetPrompt(const FString& Value){Prompt->SetText(FText::FromString(Value));}
void UVincentUI::Choice(){Show(TEXT("CÙNG NHAU BỎ TRỐN / LỰA CHỌN CUỐI"),TEXT("Chỉ có một bộ nhớ: mang An ra ngoài hoặc giữ hồ sơ phơi bày JANUS. Bạn chọn điều gì?"));AnButton->SetVisibility(ESlateVisibility::Visible);TruthButton->SetVisibility(ESlateVisibility::Visible);ContinueButton->SetVisibility(ESlateVisibility::Collapsed);}
void UVincentUI::Submit(){if(auto* G=Game())G->Submit(Answer->GetText().ToString());}
void UVincentUI::Continue(){if(auto* G=Game())G->Continue();}
void UVincentUI::Radio(){if(auto* G=Game())G->Radio();}
void UVincentUI::Hint(){if(auto* G=Game())G->Hint();}
void UVincentUI::Restart(){if(auto* G=Game())G->Restart();}
void UVincentUI::SaveAn(){if(auto* G=Game())G->Finish(1);}
void UVincentUI::SaveTruth(){if(auto* G=Game())G->Finish(2);}

void UVincentUI::StartIntro(const FString& Message){
 OpeningStory=Message;bIntro=true;Dialog->SetVisibility(ESlateVisibility::Collapsed);IntroOverlay->SetVisibility(ESlateVisibility::Visible);IntroMenu->SetVisibility(ESlateVisibility::Collapsed);
 EngineLogo=LoadObject<UTexture2D>(nullptr,TEXT("/Game/Vincent/Branding/UnrealEngineLogo.UnrealEngineLogo"),nullptr,LOAD_NoWarn);
 StudioLogo=LoadObject<UTexture2D>(nullptr,TEXT("/Game/Vincent/Branding/StudioLogo.StudioLogo"),nullptr,LOAD_NoWarn);
 GameLogo=LoadObject<UTexture2D>(nullptr,TEXT("/Game/Vincent/Branding/GameLogo.GameLogo"),nullptr,LOAD_NoWarn);
 IntroTitle->SetText(FText::FromString(TEXT("ĐƯỢC PHÁT TRIỂN VỚI\nUNREAL ENGINE")));
 IntroImage->SetVisibility(EngineLogo?ESlateVisibility::Visible:ESlateVisibility::Collapsed);if(EngineLogo)IntroImage->SetBrushFromTexture(EngineLogo,true);
 if(auto* PC=GetOwningPlayer()){PC->bShowMouseCursor=true;PC->SetInputMode(FInputModeUIOnly());}
 GetWorld()->GetTimerManager().SetTimer(IntroTimer,this,&UVincentUI::StudioStage,2.f,false);
}
void UVincentUI::StudioStage(){
 IntroImage->SetVisibility(StudioLogo?ESlateVisibility::Visible:ESlateVisibility::Collapsed);if(StudioLogo)IntroImage->SetBrushFromTexture(StudioLogo,true);
 IntroTitle->SetText(FText::FromString(StudioLogo?TEXT(""):TEXT("VINCENT D STUDIO")));
 GetWorld()->GetTimerManager().SetTimer(IntroTimer,this,&UVincentUI::MainMenu,2.3f,false);
}
void UVincentUI::MainMenu(){
 GetWorld()->GetTimerManager().ClearTimer(IntroTimer);IntroImage->SetVisibility(GameLogo?ESlateVisibility::Visible:ESlateVisibility::Collapsed);if(GameLogo)IntroImage->SetBrushFromTexture(GameLogo,true);
 IntroTitle->SetText(FText::FromString(GameLogo?TEXT("Khám phá. Giải mã. Cùng nhau bỏ trốn."):TEXT("VINCENT D: CÙNG NHAU BỎ TRỐN")));IntroMenu->SetVisibility(ESlateVisibility::Visible);
}
void UVincentUI::PlayGame(){bIntro=false;IntroOverlay->SetVisibility(ESlateVisibility::Collapsed);Show(TEXT("VINCENT D / CÙNG NHAU BỎ TRỐN"),OpeningStory);}
