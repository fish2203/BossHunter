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

	// 타겟 (Player)
	UPROPERTY()
	class AGunPlayer* target;

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
	UPROPERTY(EditAnywhere)
	float traceRange = 1000;

	// 공격 할 수 있는 범위 (공격 범위)
	UPROPERTY(EditAnywhere)
	float attackRange = 200;

	// 내가 움직일 수 있는 범위
	UPROPERTY(EditAnywhere)
	float moveRange = 3000;

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
	UPROPERTY(EditAnywhere)
	float chargeSpeed = 500;

	// AI Controller
	UPROPERTY(EditAnywhere)
	class AAIController* ai;

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

	//UPROPERTY()
	FVector posCheck;

	bool isCharge = false; 


public:
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
	bool CanTrace();

	//플레이어 쪽을 바라보자.
	void BossViewAngle();

	// 어택타입을 고른다.
	void ChangeAttackType();

	//펀치스킬
	void PunchSkill();

	//차지스킬
	void ChargeSkill();

	//돌던지기 스킬
	void ThorwSkill();

	//범위스킬
	UFUNCTION()
	void SmashSkill();

	//스킬 딜레이

	bool skillDelay(float delay);

	//오버렙 함수
	UFUNCTION()
	void ThrowSkillOverlap(UPrimitiveComponent* abc, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//
	void PlayerDamageProcess(float damage);

};
