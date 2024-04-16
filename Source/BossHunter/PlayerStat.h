// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStat.generated.h"

USTRUCT(BlueprintType)
struct Fstat
{
	GENERATED_BODY()
public:
	float attackPoint;
	float defancePoint;
	float attackSpeedPoint;
	float moveSpeedPoint;
	//curr hp
	UPROPERTY(BlueprintReadOnly)
	float healthPoint;
	float manaPoint;
	float coolTimePoint;
	//full
	float fullHP;
	float fullMP;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOSSHUNTER_API UPlayerStat : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStat();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	
};
