// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossFsmTest.generated.h"

// Enemy 상태를 정의 (enum 열거형)
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


// Enemy 공격 모션 정의
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

		
	// 현재 나의 상태를 담을 변수
	EEnemyState currState = EEnemyState::IDLE;
	EEnemyState currDamage = EEnemyState::DAMAGE;

	// 타겟 (Player)
	UPROPERTY()
	class AGunPlayer* target;

	//멀티플레이용 배열변수
	UPROPERTY()
	TArray<class AGunPlayer*> allTarget;

	
	
	// 이 컴포넌트가 어떤 액터에 붙어있는지
	UPROPERTY()
	class ATest_Boss* bossActor;
	
	// 스톤액터
	/*UPROPERTY()
	class AStoneActor* stoneActor;*/

	// 나한테 설정되어 있는 Animation class 가져올 변수
	UPROPERTY()
	class UAnimBossTest* anim;

	// Enemy Montage 담을 변수
	UPROPERTY(EditAnywhere)
	class UAnimMontage* montage;

	// 쫓아 갈 수 있는 범위 (인지 범위)
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float traceRange = 1500;
	// 공격 할 수 있는 범위 (공격 범위)
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float attackRange = 400;
	//펀치 공격 사거리
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float punchDagmageZone = 500;
	//플레이어 재추적 거리
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float playerReChase = 800;
	//돌진 공격 사거리
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float chargeDamageZone = 200;
	//돌진 전진 속도
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float chargeForwardSpeed = 800;	
	//돌진 추적 속도
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float chargeChaseSpeed = 500;

	
	//광역 공격의 딜레이
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float smashDamageDelay = 0.3;
	//광역 공격의 범위
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float smashDamageZone = 1000;
	// 내가 움직일 수 있는 범위
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	float moveRange = 10000;

	// 현재시간
	UPROPERTY()
	float currTime = 0;
	UPROPERTY()
	float smashTime=0;


	// 공격대기시간
	UPROPERTY(EditAnywhere)
	float attackDelayTime = 1;
	// 피격대기시간
	UPROPERTY(EditAnywhere)
	float damageDelayTime = 2;
	// IDLE 상태 대기 시간
	UPROPERTY(EditAnywhere)
	float idleDelayTime = 2;

	// AI Controller
	UPROPERTY(EditAnywhere)
	class AAIController* ai;


	UPROPERTY()
	class UParticleSystem* punchEffect;
	UPROPERTY()
	class UParticleSystem* chargeEffect;
	UPROPERTY()
	class UParticleSystem* throwEffect;
	UPROPERTY()
	class UParticleSystem* smashEffect;

	//전투시작
	bool huntStart = false;

	// 시야각 ( cos(시야각) )
	float viewAngle = 180;

	// 나의 처음 위치
	FVector originPos;
	// Patrol 해야하는 랜덤한 위치
	FVector patrolPos;
	
	UPROPERTY(EditAnywhere, Category = "MySettings|BossBattleOption")
	int32 randAttackAnim = 0;
	int32 randSkill = 0;
	bool isChase = false;
	bool isPunch = false;
	bool isCharge = false; 
	bool isThrow = false;
	bool isSmash = false;

public:
	// 애니메이션 동기화
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SetAnimation(EEnemyState s, int attackType);
	// 상태가 바뀔때 한번 실행 되는 함수
	void ChangeState(EEnemyState s);
	
	// 대기
	void UpdateIdle();
	// 이동
	void UpdateMove();
	// 공격
	void UpdateAttack();
	// 공격 대기
	void UpdateAttackDelay();
	// 피격
	void UpdateDamaged(float deltaTime);
	// 죽음
	void UpdateDie();
	// 리턴
	void UpdateReturn();

	// 시간 체크 함수
	bool IsWaitComplete(float delay);

	// 플레이어를 쫓아 갈 수 있는 상황인지
	// 움직임은 블루프린트에서 replicate 해서 동기화 됨.
	bool CanTrace();
	void MultiPlayTarget();
	//bool HuntStart();
	

	//플레이어 쪽을 바라보자.
	void BossViewAngle();

	//플레이어 추적
	void ChasePlayer();
	// 어택타입을 고른다.
	void ChangeAttackType();
	
	//펀치스킬
	void PunchSkill();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_PunchSkillEffect();
	

	//차지스킬
	void ChargeSkill();
	//차지 앞으로
	void ChargeForward();
	//차지 추적
	void ChargeChase();
	//돌던지기 스킬
	void ThorwSkill();
	//범위스킬
	UFUNCTION()
	void SmashSkill();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SmashSkillEffect();
	


	//스킬 딜레이
	bool skillDelay(float delay);
	//스킬 딜레이
	void DamageDelayToSkill(float delay);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_DamageDelayToSkill();

	//차지스킬 라인트레이스
	void ChargeSkillLinetrace();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ChargeSkillEffect();
	
	//오버렙 함수
	//딜리게이트 = 함수담는변수 , UFUNCTION()을 써줘야함.
	UFUNCTION()
	void SkillOverlap(UPrimitiveComponent* abc, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	//
	void PlayerDamageProcess(float damage);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_PlayerDamageProcess(float damage);

};
