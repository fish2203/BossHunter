// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Character.h"
#include "BossHunterCharacter.h"
#include "Test_Boss.generated.h"

/**
 * 
 */
UCLASS()
class BOSSHUNTER_API ATest_Boss : public ABossHunterCharacter//
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATest_Boss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	


public:

	//박스 콤퍼넌트 매쉬의 오른손에 붙일것
	UPROPERTY(EditAnywhere)
	class UBoxComponent* righthandBox;

	//fsm 자료형으로 변수 선언
	UPROPERTY(EditAnywhere)
	class UBossFsmTest* fsm;

	// 위젯 컴포넌트로 보여주자
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* comHpBar;

	//최대체력과, 현재체력
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float maxbossHP = 1000;
	float currbossHP = 0;

	//스톤액터를 티어레이로 가지고 있자.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AStoneActor> stoneFactory;

	// 총알을 여러개 들고 있는 변수 (탄창)
	UPROPERTY(VisibleAnywhere)
	TArray<class AStoneActor*> magazine;

	// 총알을 탄창에 넣을 때 
	UFUNCTION()
	void InsertBullet(class AStoneActor* bullet);

	UPROPERTY(EditAnywhere)
	class USphereComponent* smashRange;

	// 슬로우 푸는데 필요한 시간 재는 변수 - JINA
	float slowTime = 0;

	
public:
	//데미지 프로세스 만들기
	void DamageProcess(float damage);
	//타겟에 대한 정의
	void AttackTarget();

	
};
