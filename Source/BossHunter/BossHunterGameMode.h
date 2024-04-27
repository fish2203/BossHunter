// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BossHunterGameMode.generated.h"


UCLASS(minimalapi)
class ABossHunterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABossHunterGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

public:

	void URLTravel();
	UFUNCTION(Server, Reliable)
	void ServerRPC_URLTravel();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_URLTravel();

};



