// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveBossZoneWidget.h"
#include <Components/Button.h>
#include <Kismet/GameplayStatics.h>
#include "GateBattleBoss.h"


void UMoveBossZoneWidget::NativeConstruct()
{
	//�������̵��Լ�
	Super::NativeConstruct();
	
	/*movebosszoneWidget = CreateWidget<UMoveBossZoneWidget>(this);
	movebosszoneWidget->AddToViewport();*/

	// ���� ȭ�� ��ɵ�
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
	//������ ������
	SetVisibility(ESlateVisibility::Hidden);

	//������ ������
	//RemoveFromParent();

}
