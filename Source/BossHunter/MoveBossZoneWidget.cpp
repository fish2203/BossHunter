// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveBossZoneWidget.h"
#include <Components/Button.h>
#include <Kismet/GameplayStatics.h>
#include "GateBattleBoss.h"
#include "BossHunterGameMode.h"
#include <Components/SpinBox.h>
#include "BossRoomGameStateBase.h"
#include "LoadingWidget.h"


void UMoveBossZoneWidget::NativeConstruct()
{
	//오버라이드함수
	Super::NativeConstruct();
	
	/*movebosszoneWidget = CreateWidget<UMoveBossZoneWidget>(this);
	movebosszoneWidget->AddToViewport();*/

	// 메인 화면 기능들
	//#include <Components/Button.h>
	Btn_MoveBossZone->OnClicked.AddUniqueDynamic(this, &UMoveBossZoneWidget::OnClickMoveBossZone);
	Btn_Cancel->OnClicked.AddDynamic(this, &UMoveBossZoneWidget::OnClickCancel);
	
}

void UMoveBossZoneWidget::OnClickMoveBossZone()
{
	//UGameplayStatics
	//#include "GateBattleBoss.h"
	gamestate = ABossRoomGameStateBase::Get();
	/*FString checkString = InputGold->GetText().ToString();*/
	if (/*checkString.IsNumeric()*/ InputGold->GetValue() >= 0)
	{
		//gamemode->gold = FCString::Atoi(*checkString);
		gamestate->gold = (int32)InputGold->GetValue();
	}
	else
	{
		gamestate->gold = 1234;
	}
	// 모든 플레이어한테 붙여야 함
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			APlayerController* PlayerController = It->Get();
			if (PlayerController)
			{
				ULoadingWidget* LoadingWidget = CreateWidget<ULoadingWidget>(GetWorld(), LoadingWidgetFactory);
				LoadingWidget->AddToViewport(5);
			}
		}
	}
	AActor* gate = UGameplayStatics::GetActorOfClass(GetWorld(), AGateBattleBoss::StaticClass());
	gatebattleBoss = Cast<AGateBattleBoss>(gate);

	gatebattleBoss->URLTravel();


}

void UMoveBossZoneWidget::OnClickCancel()
{
	//위젯을 감추자
	SetVisibility(ESlateVisibility::Hidden);

	// 마우스 포이터 감추자.
	//GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);

	//위젯을 지우자
	//RemoveFromParent();

}
