// Fill out your copyright notice in the Description page of Project Settings.


#include "RestartBossZone.h"
#include <Components/Button.h>
#include "BossHunterGameMode.h"
#include <Kismet/GameplayStatics.h>

void URestartBossZone::NativeConstruct()
{
	
	//UButton
	Btn_restartBossZone->OnClicked.AddDynamic(this, &URestartBossZone::OnClickRestartBossZone);
	Btn_Cancel->OnClicked.AddDynamic(this, &URestartBossZone::OnClickCancel);
}

void URestartBossZone::OnClickRestartBossZone()
{	//#include <Kismet/GameplayStatics.h>
	AGameModeBase* findgm = UGameplayStatics::GetGameMode(GetWorld());
	bhGameMode = Cast<ABossHunterGameMode>(findgm);
	bhGameMode->URLTravel();
}

void URestartBossZone::OnClickCancel()
{	
	//위젯을 감추자
	SetVisibility(ESlateVisibility::Hidden);
}
