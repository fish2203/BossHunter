// Copyright Epic Games, Inc. All Rights Reserved.

#include "BossHunterGameMode.h"
#include "BossHunterCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABossHunterGameMode::ABossHunterGameMode()
{
	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/
}
