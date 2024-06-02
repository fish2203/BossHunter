// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SavePlayerScore.generated.h"

/**
 * 
 */
UCLASS()
class BOSSHUNTER_API USavePlayerScore : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 saveBestScore = 0;
	UPROPERTY()
	int32 savePlayerSkinIdx = 0;
	UPROPERTY()
	TArray <class USavePlayerScore*>  playerSkinDataArray;
	UPROPERTY()
	int32 saveHaveAmorSkin = 1;
};
