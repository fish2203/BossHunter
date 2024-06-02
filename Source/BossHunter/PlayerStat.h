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
	UPROPERTY()
	float attackPoint;
	UPROPERTY()
	float defancePoint;
	UPROPERTY()
	float attackSpeedPoint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float moveSpeedPoint;
	//curr hp
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float healthPoint;
	UPROPERTY()
	float manaPoint;
	UPROPERTY()
	float coolTimePoint;
	//full
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float fullHP;
	UPROPERTY()
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
