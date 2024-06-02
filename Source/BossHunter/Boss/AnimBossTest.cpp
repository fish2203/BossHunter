// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimBossTest.h"
#include "BossHunter/Test_Boss.h"

void UAnimBossTest::AnimNotify_OnDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("OnDamage!!!!!!"));

	// 1. 나(Pawn, Character, Enemy) 를 찾자
	APawn* pawn = TryGetPawnOwner();
	//ATest_Boss 인클루드
	ATest_Boss* myPawn = Cast<ATest_Boss>(pawn);
	// 2. target 이 가지고 있는 DamageProcess 함수 실행
	myPawn->AttackTarget();
}
