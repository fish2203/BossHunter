// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerScoreComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOSSHUNTER_API UPlayerScoreComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerScoreComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY()
	int32 currScore = 0;

	UPROPERTY()
	int32 bestSocre = 0;
	
	int32 widgetIndex = 0;

public:
	UPROPERTY()
	class AGunPlayer* gunPlayer;
	
	UPROPERTY()
	class UNetGameInstance* netGameInstance;

	UPROPERTY()
	class UPlayerScoreWidget* playerScoreUI;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerScoreWidget> playerScorePakage;

public:
	//스코어를 더하자.
	UFUNCTION()
	void AddScore(int32 value);

	UFUNCTION()
	void SaveData();

	UFUNCTION()
	void LoadData();

	UFUNCTION()
	void ShowSCoreUI();

		
};
