// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SessionGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BOSSHUNTER_API ASessionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASessionGameMode();

public:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USessionWidget> sessionWidgetFactory;
	UPROPERTY()
	class USessionWidget* sessionWidget;
	
};
