// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossFsmTest.h"
//generated�� ��Ŭ��� ���ϴܿ� �־�� �մϴ�.
#include "AnimBossTest.generated.h"

/**
 * 
 */
UCLASS()
class BOSSHUNTER_API UAnimBossTest : public UAnimInstance
{
	GENERATED_BODY()

public:
	//FSM���� ������ ����ü�� �ҷ��ɴϴ�.
	//BossFsmTest ��Ŭ���
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEnemyState state;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackType attackType;

public:
	UFUNCTION()
	void AnimNotify_OnDamage();

	//void Test(void (*ttt) ());
};

