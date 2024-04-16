// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossFsmTest.generated.h"

// Enemy ���¸� ���� (enum ������)
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	IDLE,
	MOVE,
	PATROL,
	RETURN,
	ATTACK,
	ATTACK_DELAY,
	DAMAGE,
	DIE
};


// Enemy ���� ��� ����
UENUM(BlueprintType)
enum class EAttackType : uint8
{
	PUNCH,
	Charge,
	THROW,
	SMASH
	//RAGE

};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOSSHUNTER_API UBossFsmTest : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossFsmTest();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	// ���� ���� ���¸� ���� ����
	EEnemyState currState = EEnemyState::IDLE;

	// Ÿ�� (Player)
	UPROPERTY()
	class AGunPlayer* target;

	// �� ������Ʈ�� � ���Ϳ� �پ��ִ���
	UPROPERTY()
	class ATest_Boss* bossActor;
	
	// �������
	/*UPROPERTY()
	class AStoneActor* stoneActor;*/

	// ������ �����Ǿ� �ִ� Animation class ������ ����
	UPROPERTY()
	class UAnimBossTest* anim;

	// Enemy Montage ���� ����
	UPROPERTY(EditAnywhere)
	class UAnimMontage* montage;

	// �Ѿ� �� �� �ִ� ���� (���� ����)
	UPROPERTY(EditAnywhere)
	float traceRange = 1000;

	// ���� �� �� �ִ� ���� (���� ����)
	UPROPERTY(EditAnywhere)
	float attackRange = 200;

	// ���� ������ �� �ִ� ����
	UPROPERTY(EditAnywhere)
	float moveRange = 3000;

	// ����ð�
	UPROPERTY()
	float currTime = 0;
	UPROPERTY()
	float smashTime=0;


	// ���ݴ��ð�
	UPROPERTY(EditAnywhere)
	float attackDelayTime = 1;
	// �ǰݴ��ð�
	UPROPERTY(EditAnywhere)
	float damageDelayTime = 2;
	// IDLE ���� ��� �ð�
	UPROPERTY(EditAnywhere)
	float idleDelayTime = 2;
	UPROPERTY(EditAnywhere)
	float chargeSpeed = 500;

	// AI Controller
	UPROPERTY(EditAnywhere)
	class AAIController* ai;

	UPROPERTY()
	class UParticleSystem* smashEffect;

	//��������
	bool huntStart = false;

	// �þ߰� ( cos(�þ߰�) )
	float viewAngle = 180;

	// ���� ó�� ��ġ
	FVector originPos;
	// Patrol �ؾ��ϴ� ������ ��ġ
	FVector patrolPos;

	//UPROPERTY()
	FVector posCheck;

	bool isCharge = false; 


public:
	// ���°� �ٲ� �ѹ� ���� �Ǵ� �Լ�
	void ChangeState(EEnemyState s);
	// ���
	void UpdateIdle();
	// �̵�
	void UpdateMove();
	// ����
	void UpdateAttack();
	// ���� ���
	void UpdateAttackDelay();
	// �ǰ�
	void UpdateDamaged(float deltaTime);
	// ����
	void UpdateDie();
	// ����
	void UpdateReturn();

	// �ð� üũ �Լ�
	bool IsWaitComplete(float delay);

	// �÷��̾ �Ѿ� �� �� �ִ� ��Ȳ����
	bool CanTrace();

	//�÷��̾� ���� �ٶ���.
	void BossViewAngle();

	// ����Ÿ���� ����.
	void ChangeAttackType();

	//��ġ��ų
	void PunchSkill();

	//������ų
	void ChargeSkill();

	//�������� ��ų
	void ThorwSkill();

	//������ų
	UFUNCTION()
	void SmashSkill();

	//��ų ������

	bool skillDelay(float delay);

	//������ �Լ�
	UFUNCTION()
	void ThrowSkillOverlap(UPrimitiveComponent* abc, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//
	void PlayerDamageProcess(float damage);

};
