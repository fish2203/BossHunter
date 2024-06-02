// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimBossTest.h"
#include "BossHunter/Test_Boss.h"

void UAnimBossTest::AnimNotify_OnDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("OnDamage!!!!!!"));

	// 1. ��(Pawn, Character, Enemy) �� ã��
	APawn* pawn = TryGetPawnOwner();
	//ATest_Boss ��Ŭ���
	ATest_Boss* myPawn = Cast<ATest_Boss>(pawn);
	// 2. target �� ������ �ִ� DamageProcess �Լ� ����
	myPawn->AttackTarget();
}
