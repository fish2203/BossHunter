// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionWidget.h"
#include <Components/Button.h>
#include <Components/WidgetSwitcher.h>
#include <Components/TextBlock.h>
#include <Components/EditableTextBox.h>
#include <Components/Slider.h>
#include "NetGameInstance.h"
#include "SessionInfoWidget.h"
#include <Components/ScrollBox.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/PlayerState.h>


void USessionWidget::NativeConstruct()
{	
	//오버라이드함수
	Super::NativeConstruct();

	// game instance 가져오자
	//UGameInstance* findGI = GetWorld()->GetGameInstance();
	//프로젝트세팅->맵모드->게임인스턴스->Netgameinstance로 수정
	gi = Cast<UNetGameInstance>(GetGameInstance());
	
	// 메인 화면 기능들
	//#include <Components/Button.h>
	btn_MoveCreateSession->OnClicked.AddDynamic(this, &USessionWidget::OnClickMoveCreateSession);
	btn_MoveSearchSession->OnClicked.AddDynamic(this, &USessionWidget::OnClickMoveSearchSession);
	

//세션생성 화면 기능

	// text_PlayerCount 를 초기값이 2로 설정
	text_PlayerCount->SetText(FText::AsNumber(slider_PlayerCount->GetValue()));
	// Slider 의 값이 변경될 때 함수 등록
	slider_PlayerCount->OnValueChanged.AddDynamic(this, &USessionWidget::OnValueChanged);
	//클릭하면 세션생성
	btn_CreateSession->OnClicked.AddDynamic(this, &USessionWidget::OnClickCreateSession);
	btn_Exit_CreateSession->OnClicked.AddDynamic(this, &USessionWidget::OnClickExit);

// 세션 검색 화면 
	//세션찾기
	btn_FindSession->OnClicked.AddDynamic(this, &USessionWidget::OnClickFindSession);
	//세션찾기완료
	gi->onSearchComplete.BindUObject(this, &USessionWidget::OnSearchComplete);
	btn_Exit_RoomList->OnClicked.AddDynamic(this, &USessionWidget::OnClickExit);

}

void USessionWidget::OnClickMoveCreateSession()
{
	// widget switcher 를 이용해서 1번째 Widget 이 활성화 되라!
	widgetSwitcher->SetActiveWidgetIndex(1);

	//플레이어의 이름을 가져오자.
	//#include <Kismet/GameplayStatics.h>
	//#include <GameFramework/PlayerState.h>
	APlayerState* ps = UGameplayStatics::GetPlayerState(GetWorld(), 0);
	UE_LOG(LogTemp, Warning, TEXT("Player Name : %s"), *ps->GetPlayerName());
}

void USessionWidget::OnClickMoveSearchSession()
{
	// widget switcher 를 이용해서 2번째 Widget 이 활성화 되라!
	widgetSwitcher->SetActiveWidgetIndex(2);
	// 바로 검색 시작
	OnClickFindSession();
}

void USessionWidget::OnValueChanged(float Value)
{
	// player count 텍스트 갱신
	//#include <Components/TextBlock.h>
	
	//float 값을 int로 변경
	int32 intValue = FMath::RoundToInt32(Value);

	//참가인원 표시를 int로 표현
	text_PlayerCount->SetText(FText::AsNumber(intValue));
}

void USessionWidget::OnClickCreateSession()
{
	//#include "NetGameInstance.h"
	//#include <Components/EditableTextBox.h>
	//#include <Components/Slider.h>
	gi->CreateMySession(edit_SessionName->GetText().ToString(),slider_PlayerCount->GetValue());

	text_CreateSession->SetText(FText::FromString(TEXT("생성중...")));
	btn_CreateSession->SetIsEnabled(false);
}


void USessionWidget::OnClickFindSession()
{
	//scroll_RoomList 의 자식들을 지우자
	scroll_RoomList->ClearChildren();

	/*for(int32 i = 0; i < scroll_RoomList->GetChildrenCount(); i++)
		scroll_RoomList->RemoveChildAt(0);*/

	gi->FindOtherSession();

	text_FindSession->SetText(FText::FromString(TEXT("검색중...")));
	btn_FindSession->SetIsEnabled(false);
}

void USessionWidget::OnSearchComplete(int32 idx, FString info)
{
	// SessionInfoWidget 생성
	// #include "SessionInfoWidget.h"
	// 오류 발생시 블루 프린트에서 세션위젯팩토리에 추가되어있는지 확인필요.
	auto widget = CreateWidget<USessionInfoWidget>(GetWorld(), sessionInfoWidgetFactory);
	// Scroll_RoomList 에 추가
	//#include <Components/ScrollBox.h>
	scroll_RoomList->AddChild(widget);
	// 만들어진 sessionInfo 에 데이터를 셋팅
	widget->SetInfo(idx, info);
}

void USessionWidget::OnClickExit()
{
	widgetSwitcher->SetActiveWidgetIndex(0);
}
