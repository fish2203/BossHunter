// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingWidget.h"
#include "BossHunterCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "BossRoomGameStateBase.h"
#include "GunPlayer.h"

void ULoadingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	gamestate = ABossRoomGameStateBase::Get();
}

void ULoadingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	TArray<AActor*> countArray;
	ABossHunterCharacter* player = Cast<ABossHunterCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	UGameplayStatics::GetAllActorsOfClass(this, AGunPlayer::StaticClass(), countArray);
	count = countArray.Num();

	UE_LOG(LogTemp, Warning, TEXT("1----- %d : %d"), count, gamestate->playerCount);
	if (count == gamestate->playerCount)
	{
		if (GetWorld()->GetMapName().Contains("Loading")) {
			if (player->HasAuthority()) {
				FString URL = TEXT("/Game/BulePrint/Cheezebee/Map/BossHunter/BossZone?listen");
				GetWorld()->ServerTravel(URL, ETravelType::TRAVEL_Absolute);
			}
		}
		else if (GetWorld()->GetMapName().Contains("Boss")) {
			bIsReady = true;
		}
	}
}
