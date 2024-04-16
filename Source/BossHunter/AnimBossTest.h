// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossFsmTest.h"
//generated는 인클루드 최하단에 있어야 합니다.
#include "AnimBossTest.generated.h"

/**
 * 
 */
UCLASS()
class BOSSHUNTER_API UAnimBossTest : public UAnimInstance
{
	GENERATED_BODY()

public:
	//FSM에서 열겨형 구조체를 불러옵니다.
	//BossFsmTest 인클루드
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEnemyState state;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackType attackType;

public:
	UFUNCTION()
	void AnimNotify_OnDamage();

	//void Test(void (*ttt) ());
};

