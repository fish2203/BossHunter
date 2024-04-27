// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BossRoomGameStateBase.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FItem : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 itemIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float defancePoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackSpeedPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float moveSpeedPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float healthPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float manaPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float coolTimePoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString explain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 price;
};

UCLASS()
class BOSSHUNTER_API ABossRoomGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ABossRoomGameStateBase();
	UPROPERTY()
	class UDataTable* itemDataTable;
};
