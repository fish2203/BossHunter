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

	//bossSoundComp 자료형 변수
	UPROPERTY(EditAnywhere)
	class UBossSound* bossSoundComp;

	//widget을 컴포넌트롤 붙여넣자.
	UPROPERTY(EditAnywhere)
	class UBossWidgetComponent* bossWidget;

	//위젯 컴포넌트로 보여주자
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* comHpBar;

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

	//드롭 아이템
	UPROPERTY()
	class ABossDropItem* dropItem;
	//드롭 아이템 위치
	UPROPERTY(EditAnywhere)
	class UBoxComponent* dropItemPos;



//변수
public:
	//최대체력과, 현재체력
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float maxbossHP = 1000;
	float currbossHP = 0;

	// 슬로우 푸는데 필요한 시간 재는 변수 - JINA
	float slowTime = 0;

	//아이템을 떨어뜨렸는지
	bool isDropItem = false;

public:
	//데미지 프로세스 만들기
	void DamageProcess(float damage);
	//타겟에 대한 정의
	void AttackTarget();
	//클리어 아이테 드롭
	void DropItem();
	UPROPERTY()
	class ABossDropItem* bossDropItem;


	void ReSpawnBoss();
	void DieBoss();
	

	
};
