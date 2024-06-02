// Copyright Epic Games, Inc. All Rights Reserved.

#include "BossHunterGameMode.h"
#include "BossHunterCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include <Kismet/GameplayStatics.h>
#include "Test_Boss.h"
#include "GunPlayer.h"
#include "Boss/BossFsmTest.h"
#include "PlayerScoreComp.h"

ABossHunterGameMode::ABossHunterGameMode()
{
	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/
}

void ABossHunterGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("111111"));

	// 보스를 찾자
	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATest_Boss::StaticClass());

	if (actor == nullptr) return;
	ATest_Boss* boss = Cast<ATest_Boss>(actor);
	// player 를 boss - fsm - allTarget 넣자.
	AGunPlayer* player = Cast< AGunPlayer>(NewPlayer->GetPawn());
	boss->fsm->allTarget.Add(player);

	UActorComponent* findActorComp = player->FindComponentByClass<UPlayerScoreComp>();
	UPlayerScoreComp* playerScore = Cast<UPlayerScoreComp>(findActorComp);

	//UUserWidget* findWidget = Cast<UPlayerScoreWidget>(GetWorld());
	//playerScoreUI->AddToViewport(0);

}

void ABossHunterGameMode::URLTravel()
{
	if (HasAuthority() == false) return;
	{
		ServerRPC_URLTravel();
	}


}

void ABossHunterGameMode::ServerRPC_URLTravel_Implementation()
{
	FString URL = TEXT("/Game/BulePrint/Cheezebee/Map/BossHunter/BossZone?listen");
	GetWorld()->ServerTravel(URL, ETravelType::TRAVEL_Absolute);
}

void ABossHunterGameMode::MultiRPC_URLTravel_Implementation()
{
	if (HasAuthority() == true) 
	{
		return;
	}
}
