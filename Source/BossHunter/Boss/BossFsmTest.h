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
	SMASH,
	JUMPSMASH
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
	EEnemyState currDamage = EEnemyState::DAMAGE;

	// Ÿ�� (Player)
	UPROPERTY()
	class AGunPlayer* target;

	//��Ƽ�÷��̿� �迭����
	UPROPERTY()
	TArray<class AGunPlayer*> allTarget;

	
	
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

	UPROPERTY()
	class UBossSound* bossSound;

	// �Ѿ� �� �� �ִ� ���� (���� ����)
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float traceRange = 2500;
	// ���� �� �� �ִ� ���� (���� ����)
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float attackRange = 400;
	//��ġ ���� ��Ÿ�
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float punchDagmageZone = 500;
	//�÷��̾� ������ �Ÿ�
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float playerReChase = 800;
	//���� ���� ��Ÿ�
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float chargeDamageZone = 200;
	//���� ���� �ӵ�
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float chargeForwardSpeed = 800;	
	//���� ���� �ӵ�
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float chargeChaseSpeed = 500;

	
	//���� ������ ������
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float smashDamageDelay = 1.5;
	//���� ������ ����
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float smashDamageZone = 1000;
	// ���� ������ �� �ִ� ����
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float moveRange = 10000;

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
	float chargeAttackDelay = 0.3;

	// AI Controller
	UPROPERTY(EditAnywhere)
	class AAIController* ai;


	UPROPERTY()
	class UParticleSystem* punchEffect;
	UPROPERTY()
	class UParticleSystem* chargeEffect;
	UPROPERTY()
	class UParticleSystem* throwEffect;
	UPROPERTY(EditAnywhere)
	class UParticleSystem* smashEffect;

	//��������
	bool huntStart = false;

	// �þ߰� ( cos(�þ߰�) )
	float viewAngle = 180;

	// ���� ó�� ��ġ
	FVector originPos;
	// Patrol �ؾ��ϴ� ������ ��ġ
	FVector patrolPos;
	
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	int32 randAttackAnim = 0;
	int32 randSkill = 0;
	bool isChase = false;
	bool isPunch = false;
	bool isCharge = false; 
	bool isThrow = false;
	bool isSmash = false;
	bool isJumpSmash = false;
	bool isChaseJumpSmash = false;
	float currTargetDistance = 0;
	int32 targetplayerIndex = 0;
	int32 otherplayerIndex = 0;

public:
	
	// ���°� �ٲ� �ѹ� ���� �Ǵ� �Լ�
	void ChangeState(EEnemyState s);
	// �ִϸ��̼� ����ȭ
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SetAnimation(EEnemyState s, int attackType, int skillType);

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
	// �������� �������Ʈ���� replicate �ؼ� ����ȭ ��.
	bool CanTrace();
	void MultiPlayTarget();
	//bool HuntStart();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_HuntStartVoice();

	//�÷��̾� ���� �ٶ���.
	void BossViewAngle();

	//�÷��̾� ����
	void ChasePlayer();
	// ����Ÿ���� ����.
	void ChangeAttackType();
	
	//��ġ��ų
	void PunchSkill();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_PunchSkillVoice();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_PunchSkillEffect();
	

	//������ų
	void ChargeSkill();
	//���� ������
	void ChargeForward();
	//���� ����
	void ChargeChase();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ChargeSkillVoice();


	//�������� ��ų
	void ThorwSkill();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ThorwSkillVoice();
	
	//������ų
	UFUNCTION()
	void SmashSkill();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SmashSkillVoice();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SmashSkillEffect();
	
	//����������ų
	UFUNCTION()
	void JumpSmashSkill();
	UFUNCTION()
	void ChaseJumpSmashSkill();
	
	//��ų ������
	bool skillDelay(float delay);
	//��ų ������
	void DamageDelayToSkill(float delay);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_DamageDelayToSkill();

	//������ų ����Ʈ���̽�
	void ChargeSkillLinetrace();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ChargeSkillEffect();

	//������ ��Ҹ�
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_AttackDelayVoice();
	//���� ��Ҹ�
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_DieVoice();

	
	//������ �Լ�
	//��������Ʈ = �Լ���º��� , UFUNCTION()�� �������.
	UFUNCTION()
	void SkillOverlap(UPrimitiveComponent* abc, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	//
	void PlayerDamageProcess(float damage);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_PlayerDamageProcess(float damage);


	bool isDie;

	void TestTarget();
};
