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

	//�ڽ� ���۳�Ʈ �Ž��� �����տ� ���ϰ�
	UPROPERTY(EditAnywhere)
	class UBoxComponent* righthandBox;

	//fsm �ڷ������� ���� ����
	UPROPERTY(EditAnywhere)
	class UBossFsmTest* fsm;

	// ���� ������Ʈ�� ��������
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* comHpBar;

	//�ִ�ü�°�, ����ü��
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float maxbossHP = 1000;
	float currbossHP = 0;

	//������͸� Ƽ��̷� ������ ����.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AStoneActor> stoneFactory;

	// �Ѿ��� ������ ��� �ִ� ���� (źâ)
	UPROPERTY(VisibleAnywhere)
	TArray<class AStoneActor*> magazine;

	// �Ѿ��� źâ�� ���� �� 
	UFUNCTION()
	void InsertBullet(class AStoneActor* bullet);

	UPROPERTY(EditAnywhere)
	class USphereComponent* smashRange;

	// ���ο� Ǫ�µ� �ʿ��� �ð� ��� ���� - JINA
	float slowTime = 0;

	
public:
	//������ ���μ��� �����
	void DamageProcess(float damage);
	//Ÿ�ٿ� ���� ����
	void AttackTarget();

	
};
