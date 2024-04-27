// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveBossZoneWidget.h"
#include <Components/Button.h>
#include <Kismet/GameplayStatics.h>
#include "GateBattleBoss.h"


void UMoveBossZoneWidget::NativeConstruct()
{
	//오버라이드함수
	Super::NativeConstruct();
	
	/*movebosszoneWidget = CreateWidget<UMoveBossZoneWidget>(this);
	movebosszoneWidget->AddToViewport();*/

	// 메인 화면 기능들
	//#include <Components/Button.h>
	Btn_MoveBossZone->OnClicked.AddDynamic(this, &UMoveBossZoneWidget::OnClickMoveBossZone);
	Btn_Cancel->OnClicked.AddDynamic(this, &UMoveBossZoneWidget::OnClickCancel);
	
}

void UMoveBossZoneWidget::OnClickMoveBossZone()
{
	//UGameplayStatics
	//#include "GateBattleBoss.h"
	AActor* gate = UGameplayStatics::GetActorOfClass(GetWorld(), AGateBattleBoss::StaticClass());
	gatebattleBoss = Cast<AGateBattleBoss>(gate);

	gatebattleBoss->URLTravel();


}

void UMoveBossZoneWidget::OnClickCancel()
{
	//위젯을 감추자
	SetVisibility(ESlateVisibility::Hidden);

	//위젯을 지우자
	//RemoveFromParent();

}
