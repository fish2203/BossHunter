// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFsmTest.h"
#include <Kismet/GameplayStatics.h>
#include "BossHunter/TestPlayer.h"
#include "BossHunter/Test_Boss.h"
#include "AnimBossTest.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include <../../../../../../../Source/Runtime/AIModule/Classes/AIController.h>
#include "BossHunter/GunPlayer.h"
#include "BossHunter/StoneActor.h"
#include <Components/SphereComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>
#include "BossHunter/PlayerStat.h"
#include "BossHunter/BossHunterCharacter.h"
#include <../../../../../../../Source/Runtime/CoreUObject/Public/UObject/ObjectMacros.h>
#include <Components/BoxComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
//플레이어 점수 컴포넌트
#include <BossHunter/PlayerScoreComp.h>
//플레이어 점수 위젯
#include "BossHunter/PlayerScoreWidget.h"
#include "../BossSound.h"


// Sets default values for this component's properties
UBossFsmTest::UBossFsmTest()
{
	PrimaryComponentTick.bCanEverTick = true;

	// montage 읽어오자
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("/Script/Engine.AnimMontage'/Game/BulePrint/Cheezebee/AM_TestBoss.AM_TestBoss'"));
	if (tempMontage.Succeeded())
	{
		montage = tempMontage.Object;
	}

	//punch효과, 
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempPunchEffect(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonRampage/FX/Particles/Rampage_v001_IceBlue/FX/P_Rampage_Ice_Melee_Impact.P_Rampage_Ice_Melee_Impact'"));

	if (tempPunchEffect.Succeeded())
	{
		punchEffect = tempPunchEffect.Object;
	}
	//chargeEffect효과, 스매쉬아크
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempChargeEffect(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonRampage/FX/Particles/Rampage_v001_IceBlue/FX/P_Rampage_Ice_SmashArc.P_Rampage_Ice_SmashArc'"));

	if (tempChargeEffect.Succeeded())
	{
		chargeEffect = tempChargeEffect.Object;
	}
	//throwEffect효과
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempThrowEffect(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonRampage/FX/Particles/Rampage_v001_IceBlue/FX/P_Rampage_Ice_Melee_Impact.P_Rampage_Ice_Melee_Impact'"));

	if (tempThrowEffect.Succeeded())
	{
		throwEffect = tempThrowEffect.Object;
	}
	//smashEffect효과, 아이스 캐스트 불러오자.
	//ConstructorHelpers::FObjectFinder<UParticleSystem> tempEffect(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonRampage/FX/Particles/Rampage_v001_IceBlue/FX/P_Rampage_Ice_Enrage_Cast.P_Rampage_Ice_Enrage_Cast'"));

	//smash효과 어스그라운드 어택 불러오자.
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempSmashEffect(TEXT("/Script/Engine.ParticleSystem'/Game/GroundAttacks/Fx/Earth/P_EarthGroundAttack.P_EarthGroundAttack'"));

	if (tempSmashEffect.Succeeded())
	{
		smashEffect = tempSmashEffect.Object;
	}

}


// Called when the game starts
void UBossFsmTest::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어를 찾자
	//UGameplayStatics 인클루드
	//ATestPlayer 인클루드
	//플레이어를 찾자.
	AActor* findActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGunPlayer::StaticClass());
	target = Cast<AGunPlayer>(findActor);
	UE_LOG(LogTemp, Warning, TEXT("beginplay find player"));

	//배열 액터의 주소값을 저장하는 지역변수
	TArray<AActor*> findallPlayer;
	//게임에서 모든클래스를 찾아서, findallPlayer 넣자.
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGunPlayer::StaticClass(), findallPlayer);
	//UE_LOG(LogTemp, Warning, TEXT("find player"));

	//플레이어를 AActor로 가져와도 되지만 Gunplayer가 가진 변수나 함수를 호출하기 위해서는 Gunplayer 자료형을 써야합니다. ex) 플레이어의 체력을 가져올때

	//찾은 플레이어의 숫자만큼 실행
	
	for (int32 i = 0; i < findallPlayer.Num(); i++)
	{
		//올타켓은(멤버)에 캐스팅한 플레이어 배열 자료형 넣기
		//allTarget.Add(Cast<AGunPlayer>(findallPlayer[i]));

	}
	UE_LOG(LogTemp, Warning, TEXT("find player : %d"), findallPlayer.Num());

	// 나의 액터를 찾자
	//ATest_Boss 인클루드
	bossActor = Cast<ATest_Boss>(GetOwner());
	UE_LOG(LogTemp, Warning, TEXT("find Boss"));

	// 나한테 설정되어 있는 Anim Class 가져오자
	// 인클루드 #include "AnimBossTest.h"
	USkeletalMeshComponent* mesh = bossActor->GetMesh();
	UAnimInstance* animInstance = mesh->GetAnimInstance();
	anim = Cast<UAnimBossTest>(animInstance);

	// 시야각을 cos(시야각) 으로 하자
	float radianViewAngle = FMath::DegreesToRadians(viewAngle * 0.5f);
	viewAngle = FMath::Cos(radianViewAngle);

	// 나의 처음 위치를 담아두자
	originPos = bossActor->GetActorLocation();

	//충돌시 호출되는 함수를 등록
	bossActor->GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &UBossFsmTest::SkillOverlap);

	//플레이어의 액터 컴포넌트
	UActorComponent* findComp = target->FindComponentByClass<UPlayerScoreComp>();
	target->playerScore = Cast<UPlayerScoreComp>(findComp);

	//보스 사운드 컴포넌트
	UActorComponent* findSoundComp = bossActor->FindComponentByClass<UBossSound>();
	bossSound = Cast<UBossSound>(findSoundComp);
}


// Called every frame
void UBossFsmTest::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bossActor->HasAuthority() == false) return;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 만약에 plater(target) 이 없다면 함수를 나가자
	if (target == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT(" bossfsm tick null target"));
		return;
	}

	switch (currState)
	{
		//UE_LOG(LogTemp, Warning, TEXT(" bossfsm tick idle"));
	case EEnemyState::IDLE:
		UpdateIdle();
		break;
	case EEnemyState::MOVE:
		UpdateMove();
		break;
	case EEnemyState::ATTACK:
		UpdateAttack();
		break;
	case EEnemyState::ATTACK_DELAY:
		UpdateAttackDelay();
		break;
	case EEnemyState::DAMAGE:
		UpdateDamaged(DeltaTime);
		break;
	case EEnemyState::DIE:
		UpdateDie();
		break;
	case EEnemyState::RETURN:
		UpdateReturn();
		break;

	default:
		break;


	}

	

}


//한번만 실행할것은 여기에 추가합니다.
void UBossFsmTest::ChangeState(EEnemyState s)
{
	// 바뀌는 상태를 출력하자
	UE_LOG(LogTemp, Warning, TEXT("%d --> %d"), (int)(currState), (int)s);

	currState = s;

	// 애니메이션 상태 갱신 (동기화)
	anim->state = currState;

	// 현재 시간을 초기화
	currTime = 0;

	

	switch (currState)
	{
	case EEnemyState::IDLE:
		break;
	case EEnemyState::MOVE:
		break;

	case EEnemyState::ATTACK:
	{

		//보스 방향을 플레이어 방향으로 돌립니다.
		BossViewAngle();
		// 4가지중 랜덤 공격할지 설정
		//randAttackAnim = 1;
		randAttackAnim = FMath::RandRange(0, 3);
		//2가지 속성을 가진 공격 타입은 여기서 결정
		randSkill = FMath::RandRange(0, 1);


		//애니메이션 결정
		//anim->attackType = (EAttackType)randAttackAnim;

		//모든 플레이어에게.
		//MultiRPC_ChangeState(s);

		//PUNCH 0, Charge 1, THROW 2, SMASH 3
		//if (1) 
		if(randAttackAnim == 0)
		{
			//isChase = true;
			isPunch = true;
			MultiRPC_PunchSkillVoice();
			//PunchSkill();
			//UE_LOG(LogTemp, Warning,TEXT("000000"));
			//isCharge = true;
			//ChargeSkill();
			
			//ThorwSkill();

			//SmashSkill();

			//isChaseJumpSmash = true;
			
			//isJumpSmash = true;
			
			
			//UE_LOG(LogTemp, Warning, TEXT("state attack no0 punch Skill"));
		}
		else if (randAttackAnim == 1)
		{
			isCharge = true;
			MultiRPC_ChargeSkillVoice();
			//ChargeSkill();
			//UE_LOG(LogTemp, Warning, TEXT("state attack no1 charge Skill"));
		}
		else if (randAttackAnim == 2)
		{
			
			ThorwSkill();
			MultiRPC_ThorwSkillVoice();
			//UE_LOG(LogTemp, Warning, TEXT("state attack no2 ThorwSkill"));
		}
		else
		{
			
			SmashSkill();
			MultiRPC_SmashSkillVoice();
			//UE_LOG(LogTemp, Warning, TEXT("state attack no3 SmashSkill"));
		}
	
	}
	//UE_LOG(LogTemp, Warning, TEXT("state attack  break"));
	break;
	}
	switch (currState)
	{
	case EEnemyState::DAMAGE:
	{// 1. 랜덤한 값을 뽑는다. (1, 2)
		int32 rand = FMath::RandRange(1, 2);
		// 2. Damage01, Damage02 란 문자열을 만든다.
		FString sectionName = FString::Printf(TEXT("Damage0%d"), rand);
		// 3. Montage 플레이
		//myActor->PlayAnimMontage(montage, 1.0f, FName(*sectionName));
		bossActor->PlayAnimMontage(montage, 1.0f, TEXT("hurt"));
	}
	break;

	case EEnemyState::DIE:
	{
		//충돌 처리 되지 않게 하자
		//UCapsuleComponent
		//bossActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	break;

	case EEnemyState::RETURN:
		bossActor->SetActorLocation(originPos);
		break;

	default:
		break;

	}

	// 애니메이션 동기화
	MultiRPC_SetAnimation(currState, randAttackAnim, randSkill);
}


void UBossFsmTest::MultiRPC_SetAnimation_Implementation(EEnemyState s, int attackType, int skillType)
{
	if(anim != nullptr)
	{
		// 애니메이션 상태 갱신 (동기화)
		anim->state = s;

		// 공격 애니메이션 결정
		anim->attackType = (EAttackType)attackType;
		//UE_LOG(LogTemp, Warning, TEXT("multi setanim : %d"), randAttackAnim);
	}
	

	skillType = randSkill;

	//피격 애니메이션 결정
	if (s == EEnemyState::DAMAGE)
	{
		bossActor->PlayAnimMontage(montage, 1.0f, TEXT("hurt"));
	}
	//죽음 애니메이션 결정
	
	
	if (s == EEnemyState::DIE)
	{

		//bossActor->Chracter
		bossActor->PlayAnimMontage(montage, 1.0f, TEXT("die"));
		bossActor->SetActorEnableCollision(ECollisionEnabled::NoCollision);
		bossActor->DropItem();
	}

}

void UBossFsmTest::UpdateIdle()
{
	//UE_LOG(LogTemp, Warning, TEXT(" update idle"));

	// 만약에 플레이어를 쫓아갈 수 있다면
	if (CanTrace())
	{

		//UE_LOG(LogTemp, Warning, TEXT(" can trace"));

		int32 rand = FMath::RandRange(0, 1);
		anim->attackType = (EAttackType)rand;

		if (rand == 0)
		{	
			
			// 상태를 Move 로 바꿔라
			ChangeState(EEnemyState::MOVE);
			//UE_LOG(LogTemp, Warning, TEXT(" updateidle change move"));
			
		}
		else

		{
			ChangeState(EEnemyState::ATTACK);
			//UE_LOG(LogTemp, Warning, TEXT(" updateidle change attack"));

		}
	}
	// 그렇지 않고 idleDelayTime 을 지났다면
	else if (IsWaitComplete(idleDelayTime))
	{
		// 상태를 PATROL 로 바꿔라
		ChangeState(EEnemyState::IDLE);

		UE_LOG(LogTemp, Warning, TEXT(" no trace "));

	}
}

// 틱에서 동작합니다.
void UBossFsmTest::UpdateMove()
{
	//플레이어의 위치
	FVector playerPos = target->GetActorLocation() - bossActor->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("UpdateMove %f "), dir.Length());

	// 처음 위치와 나의 위치의 거리
	float distance = FVector::Distance(originPos, bossActor->GetActorLocation());

	// 만약에 distance 가 moveRange 보다 커지면 
	if (distance > moveRange)
	{
		// Return 상태로 전환
		ChangeState(EEnemyState::RETURN);
		//UE_LOG(LogTemp, Warning, TEXT(" update move change RETURN "));
	}

	else
	{
		// 2. target 위치로 움직이자 (Navigation  기능을 통해서)
		//ai->MoveToLocation(target->GetActorLocation());

		//// 2. 그 방향으로 움직이자. 
		bossActor->AddMovementInput(playerPos.GetSafeNormal());
		//UE_LOG(LogTemp, Warning, TEXT("UpdateMove  AddMovementInput"));

		// 3. 플레이어와의 거리가 공격 범위보다 작으면
		float dist = playerPos.Length();
		//UE_LOG(LogTemp, Warning, TEXT("dist : %f"), dist);

		currTime += GetWorld()->DeltaTimeSeconds;
		//UE_LOG(LogTemp, Warning, TEXT(" update move time %f"), currTime);


		//플레이어와의 거리가 playerReChase보다 작을때
		if (dist < playerReChase)
		{

			//거리가 attackRange 적을때 공격
			if (dist < attackRange)
			{

				ChangeState(EEnemyState::ATTACK);
				//UE_LOG(LogTemp, Warning, TEXT(" update move ischase"));
				currTime = 0;
			}

			//2초가 지나면 공격
			else if (currTime > 2)
			{

				// 4. 현재 상태를 ATTACK 로 바꾸자
				ChangeState(EEnemyState::ATTACK);
				//UE_LOG(LogTemp, Warning, TEXT(" upmove change Attack "));
				currTime = 0;
			}

		}


	}

}

//틱에서 계속 실행할겁니다.
void UBossFsmTest::UpdateAttack()
{
	if (isChase)
	{
		//추적스킬을 틱마다 실행
		ChasePlayer();
	}

	else if (isPunch)
	{
		//펀치 스킬을 틱마다 실행
		PunchSkill();
		//UE_LOG(LogTemp, Warning, TEXT("update attack ispunch"));
	}

	else if (isCharge)
	{
		//차지스킬을 틱마다 실행
		ChargeSkill();
	}
	else if (isSmash)
	{
		DamageDelayToSkill(1);
	}
	else if (isJumpSmash)
	{
		JumpSmashSkill();
	}
	//1초대기 후 어택 딜레이로
	else if (isChaseJumpSmash)
	{
		ChaseJumpSmashSkill();
	}
	else
	{
		isPunch = false;
		isCharge = false;
		
		currTime += GetWorld()->DeltaTimeSeconds;

		if (currTime > 1)
		{
			//딜레이 보이스
			MultiRPC_AttackDelayVoice();
			isPunch = false;
			isCharge = false;
			currTime = 0;
			UE_LOG(LogTemp, Warning, TEXT("UpdateAttack chansge attack delay after 1 second "));
			ChangeState(EEnemyState::ATTACK_DELAY);

		}
	}
}

void UBossFsmTest::UpdateAttackDelay()
{

	isPunch = false;
	isCharge = false;
	// 1. 시간을 흐르게 한다.
	currTime += GetWorld()->DeltaTimeSeconds;
	// 2. 만약에 Attack Delay 시간이 지나면
	//attackDelayTime = 1
	if (currTime > attackDelayTime)
	

		//UE_LOG(LogTemp, Warning, TEXT("attack delay"));


		if (IsWaitComplete(attackDelayTime))
		{
			// 플레어와의 거리
			float dist = FVector::Distance(target->GetActorLocation(), bossActor->GetActorLocation());
			// 그 거리가 공격범위- > 진짜 공격
			if (dist < attackRange)
			{
				// 3. 공격 상태로 가라
				ChangeState(EEnemyState::ATTACK);
				//UE_LOG(LogTemp, Warning, TEXT(" update attack delay change attak"));

			}

			// 인지범위 -> 이동 
			if(target == nullptr) return;
			else if (CanTrace())
			{
				//randSkill = 0;
				randSkill = FMath::RandRange(0, 1);

				if (randSkill == 0)
				{
					ChangeState(EEnemyState::MOVE);
					//UE_LOG(LogTemp, Warning, TEXT(" update attack delay change move"));
				}

				else
				{
					ChangeState(EEnemyState::ATTACK);
					//UE_LOG(LogTemp, Warning, TEXT(" update attack delay change Attack"));
				}
			}
			// 그 외는 -> 대기
			else
			{
				ChangeState(EEnemyState::IDLE);
				//bossActor->SetActorEnableCollision(ECollisionEnabled::NoCollision);
				//UE_LOG(LogTemp, Warning, TEXT(" update attack delay change idle"));

			}

			// 4. 현재시간 초기화
			currTime = 0;
		}
	/*else
	{
		ChangeState(EEnemyState::IDLE);
	}*/

}

void UBossFsmTest::UpdateDamaged(float deltaTime)
{
	if (IsWaitComplete(damageDelayTime))
	{
		// IDLE 상태로 전환
		ChangeState(EEnemyState::IDLE);
	}
}

void UBossFsmTest::UpdateDie()
{
	bossActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (isDie) return;
	MultiRPC_DieVoice();
	bossActor->PlayAnimMontage(montage, 1.0f, TEXT("die"));
	bossActor->DropItem();
	//UPlayerScoreComp
	target->playerScore->AddScore(100);
	
	isDie = true;
	if (IsWaitComplete(1))
	{
		
	
	}

}

void UBossFsmTest::UpdateReturn()
{
	// 내 위치와 처음 위치의 거리를 구한다.
	float dist = FVector::Distance(originPos, bossActor->GetActorLocation());
	//UE_LOG(LogTemp, Warning, TEXT("dist : %f"), dist);

	// 그 거리가 0이면
	if (dist < 50)
	{
		// IDLE 상태로 전환
		ChangeState(EEnemyState::IDLE);
	}
}

bool UBossFsmTest::IsWaitComplete(float delay)
{
	currTime += GetWorld()->DeltaTimeSeconds;
	if (currTime >= delay)
	{
		currTime = 0;
		return true;

	}

	return false;
}

bool UBossFsmTest::CanTrace()
{
	if (target == nullptr) return false;
	if (allTarget.Num() == 0) return false;


	//멀티플레이를 할때 플레이어 타겟의 위치를 구하자.
	MultiPlayTarget();
	//UE_LOG(LogTemp, Warning, TEXT(" cantarce MultiPlayTarget "));
	//UE_LOG(LogTemp, Warning, TEXT(" huntStart : %s "), huntStart);

	// 1. 싱글플레이어와의 거리를 구하자.
	FVector dir = target->GetActorLocation() - bossActor->GetActorLocation();
	float dist = dir.Length();
	//UE_LOG(LogTemp, Warning, TEXT(" trace dir %f"), dir.Length());

	//사냥시작이 참이면
	if (huntStart)
	{
		//트루를 반환, 무조건 추적
		return true;
	}


	// 2. 만약에 거리가 인지범위 2500 보다 작고
	// 보스체력이 0보다 크다면
	if (dist < traceRange && bossActor->currbossHP>=0)
	{
		
		// 나의 앞방향과 플레어를 향하는 방향을 내적
		float dot = FVector::DotProduct(dir.GetSafeNormal(), bossActor->GetActorForwardVector());

		FVector bossStarLocation = GetOwner()->GetActorLocation();
		FColor DebugColor;

		//#include "DrawDebugHelpers.h"
		//DrawDebugLine(GetWorld(), bossStarLocation, bossStarLocation + dir, DebugColor.Red, false, 5.0f, 0, 1.0f);


		if (dot > viewAngle)
		{

			UE_LOG(LogTemp, Warning, TEXT(" dot > view "));

			// 플레이어를 향하는 방향에 장애물이 있는지 판단 (LineTrace)
			FVector start = bossActor->GetActorLocation();
			FVector end = target->GetActorLocation();
			//FVector end = start + bossActor->GetActorForwardVector() * 3000;
			
			
			FHitResult hitInfo;
			FCollisionQueryParams param;
			param.AddIgnoredActor(bossActor);

			bool hit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_Visibility, param);

			if (hit)
			{
			if (hitInfo.GetActor()->GetName().Contains(TEXT("Player")) == true)
				{

					UE_LOG(LogTemp, Warning, TEXT(" crace huntStart = true "));
					//사냥시작을 참으로 이후부터 계속 추적합니다.
					
					MultiRPC_HuntStartVoice();
					huntStart = true;
					return true;

					
				}
			}

			//target = 가장가까운 Player를 넣자.
			return false;
		}

		// 내적한 값을 acos 하자
		float radianAngle = FMath::Acos(dot);
		// degree 값으로 변경하자
		float degreeAngle = FMath::RadiansToDegrees(radianAngle);
		// 만약에 플레이어가 시약각(60)에 들어왔다면 (degreeAngle <= 60 / 2) 
		if (degreeAngle <= 180 * 0.5f)
		{
			return true;
		}
	}

	return false;
	
	

}

//멀티플레이를 할때 플레이어 타겟의 위치를 구하자.
void UBossFsmTest::MultiPlayTarget()
{
	

	//거리를 비교하기 위한 최소값을 비교할 최대값
	float targetMaxDistance = 100000;
	//플레이어 인덱스를 저장할 변수
	targetplayerIndex = 0;
	otherplayerIndex = 0;

	
	//멀티플레이어의 거리를 구하는 for문, 구한값을 target에 넣자.
	for (int32 i = 0; i < allTarget.Num(); i++)
	{
		
		//보스와 올타겟의 거리를 구하자.
		float targetDistance = FVector::Distance(allTarget[i]->GetActorLocation(), bossActor->GetActorLocation());
		//UE_LOG(LogTemp, Warning, TEXT(" min : %f"), targetDistance);

		//현재거리 < 최대거리
		if (targetDistance < targetMaxDistance)
		{	

			
			//이전거리< 현재거리
			if (targetDistance < currTargetDistance)
			{
				//현재거리가 이전거리보다 작으면 타겟에 넣자.
				targetplayerIndex = i;
				//이전거리 갱신
				currTargetDistance = targetDistance;
			}
			else if (currTargetDistance < targetDistance)
			{
				otherplayerIndex = i;
				currTargetDistance = targetDistance;
			}
		}

		//for 종료
	}
	
	//타겟 값에 가장가까운 플레어를 인덱스를 넣자.
	target = allTarget[targetplayerIndex];
	//UE_LOG(LogTemp, Warning, TEXT(" targetidx : %d, otheridx : %d "), targetplayerIndex, otherplayerIndex);
	//UE_LOG(LogTemp, Warning, TEXT("target hp : %f"), target->statment.healthPoint);

	if (target->statment.healthPoint <= 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("target die"));
		allTarget.RemoveAt(targetplayerIndex);
		//target = allTarget[otherplayerIndex];
		
	}



	
}

void UBossFsmTest::MultiRPC_HuntStartVoice_Implementation()
{
	bossSound->playbossBattleStartSound();
	bossSound->playbossBattleStartMusic();
}

//bool UBossFsmTest::HuntStart()
//                                                                            {
//	return;	
//}

void UBossFsmTest::BossViewAngle()
{
	//MultiRPC_BossViewAngle();
	FVector playerPos = target->GetActorLocation();
	FVector bossPos = bossActor->GetActorLocation();
	//플레이어 방향을 구하자.
	FVector dir = playerPos - bossPos;

	bossActor->SetActorRotation(dir.Rotation());
}

void UBossFsmTest::ChasePlayer()
{
	//플레이어의 위치
	FVector playerPos = target->GetActorLocation() - bossActor->GetActorLocation();
	//플레이어의 방향값
	float dist = playerPos.Length();

	currTime += GetWorld()->GetDeltaSeconds();
	if (currTime < 2)
	{
		//0이면 계속 이동
		if (randSkill == 0)
		{
			bossActor->AddMovementInput(playerPos.GetSafeNormal());
			UE_LOG(LogTemp, Warning, TEXT(" ChasePlayer "));

			if (currTime > 2)
			{
				isChase = false;
				UE_LOG(LogTemp, Warning, TEXT("chase false"));
			}
		}

		//1이면 공격
		else
		{
			ChangeState(EEnemyState::ATTACK);
			UE_LOG(LogTemp, Warning, TEXT(" chaseskill change attack  "));

		}
	}
	isChase = false;
	UE_LOG(LogTemp, Warning, TEXT("chase false"));



}

void UBossFsmTest::ChangeAttackType()
{

}

void UBossFsmTest::PunchSkill()
{
	//UE_LOG(LogTemp, Warning, TEXT("punchskill"));
	FVector playerPos = target->GetActorLocation();
	FVector AttackDirstnace = playerPos - bossActor->GetActorLocation();
	float damageZone = AttackDirstnace.Length();

	currTime += GetWorld()->DeltaTimeSeconds;

	if (currTime > 1.35)
	{
		
		//UE_LOG(LogTemp, Warning, TEXT(" boss fsm punchskill "));

		//UE_LOG(LogTemp, Warning, TEXT(" fsm punchskill 0.5 time "));

		MultiRPC_PunchSkillEffect();

		//데미지존<punchDagmageZone
		if (damageZone < punchDagmageZone)
		{
			//UE_LOG(LogTemp, Warning, TEXT(" fsm punchskill damagezone "));

			PlayerDamageProcess(50);
			//MultiRPC_PlayerDamageProcess(50);
			//ChangeState(EEnemyState::MOVE);
		}
		currTime = 0;
		isPunch = false;
	}
}

void UBossFsmTest::MultiRPC_PunchSkillVoice_Implementation()
{
	//UBossSound
	bossSound->PlaybossPunchVoice();
}

void UBossFsmTest::MultiRPC_PunchSkillEffect_Implementation()
{	
	
	//ATest_Boss* bossActor = Cast<ATest_Boss>(GetOwner());

	USceneComponent* meshChild = bossActor->GetMesh()->GetChildComponent(0);
	UBoxComponent* rightHandBox = Cast<UBoxComponent>(meshChild);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), punchEffect, rightHandBox->GetComponentLocation(), rightHandBox->GetComponentRotation(), FVector(4));
	bossSound->PlaybossPunchSound();
	



}

void UBossFsmTest::ChargeSkill()
{
	
	//플레이어 위치 구하자.
	FVector playerPos = target->GetActorLocation();
	//플레이어 위치 방향값을 저장하자.
	FVector bossPos = playerPos - bossActor->GetActorLocation();
	bossPos.Normalize();

	if (randSkill == 0)
	{
		ChargeForward();

	}
	else
	{
		ChargeChase();
	}

	currTime += GetWorld()->GetDeltaSeconds();
	if (currTime > 2)
	{
		isCharge = false;
		UE_LOG(LogTemp, Warning, TEXT("charge end"));
	}
	
	//예제코드
	//플레이어 위치 구하기
	//FVector playerPos = target->GetActorLocation();
	//보스 위치 구하기
	//FVector bossPos = myActor->GetActorLocation();
	//플레이어에서 보스위치를 방향을 빼자
	//FVector p0 = playerPos - bossPos;
	//방향값을 노말라이즈하자.
	//p0.Normalize();
	//플레이어 방향, 속도, 시간
	//FVector p1 = p0 * chargeSpeed * GetWorld()->GetDeltaSeconds();
	//보스의 위치에 방향,속도,시간을 더하자.
	//FVector p2 = bossPos + p1;
	//내위치를 p2로 이동시키자.
	//myActor->SetActorLocation(p2);
	//myActor->AddMovementInput(p0);

	//DrawDebugLine(GetWorld(), bossPos, playerPos , FColor::Red);


}

void UBossFsmTest::ChargeForward()
{
	// 보스의 포워드 벡터 방향으로 움직이자.

	//플레이어 위치 구하자.
	//FVector playerPos = target->GetActorLocation();
	//플레이어 위치 방향값을 저장하자.
	//FVector bossPos = playerPos - bossActor->GetActorLocation();
	//보스방향을 노말라이즈
	//bossPos.Normalize();


	//플레이어 위치에서 보스위치를 방향을 빼자
	FVector p0 = bossActor->GetActorForwardVector();
	//방향값을 노말라이즈하자.
	p0.Normalize();
	//보스의 포워드 벡터에서 플레이어 방향으로 돌이 움직이게 하자, 속도, 시간
	FVector p1 = p0 * chargeForwardSpeed * GetWorld()->GetDeltaSeconds();
	//내 위치에 p1를 더하자.
	FVector p2 = bossActor->GetActorLocation() + p1;
	//내위치를 p2로 이동시키자.
	bossActor->SetActorLocation(p2);

	//DrawDebugLine(GetWorld(), p0, p2, FColor::Red);


	//SkillOverlap(UPrimitiveComponent * abc, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	//bossActor->GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &UBossFsmTest::SkillOverlap);

	//bossActor->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &UBossFsmTest::SkillOverlap);

	//UE_LOG(LogTemp, Warning, TEXT(" charge skill forward"));
	//chargeAttackDelay = 0.2;
	if(currTime > chargeAttackDelay)
	{
		ChargeSkillLinetrace();
	}

}

void UBossFsmTest::ChargeChase()
{

	//플레이어 위치 구하자.
	FVector playerPos = target->GetActorLocation();
	//플레이어 위치 방향값을 저장하자.
	FVector bossPos = playerPos - bossActor->GetActorLocation();
	bossPos.Normalize();

	// 플레이어를 추적하자.

	FVector p0 = bossActor->GetActorLocation();
	FVector vt = bossPos * chargeChaseSpeed * GetWorld()->DeltaTimeSeconds;
	FVector p = p0 + vt;
	bossActor->SetActorLocation(p);
	bossActor->SetActorRotation(bossPos.Rotation());
	//DrawDebugLine(GetWorld(), bossPos, playerPos, FColor::Red);
	//UE_LOG(LogTemp, Warning, TEXT(" charge skill chage"));
	
	//chargeAttackDelay= 0.2;
	if (currTime > chargeAttackDelay)
	{
		ChargeSkillLinetrace();
	}
}

void UBossFsmTest::MultiRPC_ChargeSkillVoice_Implementation()
{
	bossSound->playbossChargeVoice();
}

void UBossFsmTest::ThorwSkill()
{
	// 총알공장(BP_Bullet) 에서 총알을 하나 생성한다.
	AStoneActor* spawnStone = GetWorld()->SpawnActor<AStoneActor>(bossActor->stoneFactory);
	// 생성된 총알을 나의 위치에 놓는다.
	spawnStone->SetActorLocation(bossActor->GetActorLocation());
	spawnStone->isThorw = true;
	//스폰의 타겟을 현재타겟으로 갱신
	spawnStone->target = target;

	//	spawnStone->meshComp->OnComponentBeginOverlap.AddDynamic(this, &UBossFsmTest::SkillOverlap);

		//UE_LOG(LogTemp, Warning, TEXT("bossfsm Throw Skill"));

		//spawnStone->ThrowSkillLinetrace();



}

void UBossFsmTest::MultiRPC_ThorwSkillVoice_Implementation()
{
	bossSound->playbossStoneThrowVoice();
}

void UBossFsmTest::SmashSkill()
{
	//1초후에 대미지 범위내에 있으면 피해를 줍니다.
	isSmash = true;
	//MultiRPC_SmashSkillEffect();
	
}

void UBossFsmTest::MultiRPC_SmashSkillVoice_Implementation()
{
	bossSound->playbossSmashVoice();
}

void UBossFsmTest::JumpSmashSkill()
{
	//UCharacterMovementComponent
	//크래비티 스케일을 0으로
	//bossActor->GetCharacterMovement()->GravityScale = 0.0f;
	currTime += GetWorld()->DeltaTimeSeconds;
	
	if (currTime < 2)
	{
		bossActor->GetCharacterMovement()->DoJump(true);
		//isJumpSmash = true;
		UE_LOG(LogTemp, Warning, TEXT(" JumpSmashSkill"));
		//제자리 점프와 이동
		FVector p0 = bossActor->GetActorUpVector();
		//방향값을 노말라이즈하자.
		p0.Normalize();
		//보스의 포워드 벡터에서 플레이어 방향으로 돌이 움직이게 하자, 속도, 시간
		FVector p1 = p0 * chargeForwardSpeed * GetWorld()->GetDeltaSeconds();
		//현재 보스위치에 p1를 더하자.
		FVector p2 = bossActor->GetActorLocation() + p1;
		//방향값을 노말라이즈하자.
		//p2.Normalize();
		//내위치를 p2로 이동시키자.
		bossActor->SetActorLocation(p2);
		//바닥에 도착후 효과 출력, 범위 내라면 피격
		UE_LOG(LogTemp, Warning, TEXT(" JumpSmashSkill : %f "), p2.Length());
	}
	
	 if (currTime > 5)
	 {
		 isJumpSmash = false;
		currTime=0;
		
	 }
	 
	//예제코드
	//플레이어 위치 구하기
	//FVector playerPos = target->GetActorLocation();
	//보스 위치 구하기
	//FVector bossPos = myActor->GetActorLocation();
	//플레이어에서 보스위치를 방향을 빼자
	//FVector p0 = playerPos - bossPos;
	//방향값을 노말라이즈하자.
	//p0.Normalize();
	//플레이어 방향, 속도, 시간
	//FVector p1 = p0 * chargeSpeed * GetWorld()->GetDeltaSeconds();
	//보스의 위치에 방향,속도,시간을 더하자.
	//FVector p2 = bossPos + p1;
	//내위치를 p2로 이동시키자.
	//myActor->SetActorLocation(p2);
	//myActor->AddMovementInput(p0);

	//DrawDebugLine(GetWorld(), bossPos, playerPos , FColor::Red);

}

void UBossFsmTest::ChaseJumpSmashSkill()
{
	//시간을 누적하자
	currTime += GetWorld()->DeltaTimeSeconds;
	
	//1초후에 플레이어 방향으로 점프
	if (currTime > 1)
	{
	bossActor->Jump();
	
	//플레이어의 위치를 구하자.
	FVector playerPos = target->GetActorLocation();
	//보스 위치를 구하자.
	FVector bossPos = bossActor->GetActorLocation();
	//내 위치를 빼자.
	FVector dir = playerPos - bossActor->GetActorLocation();
	//방향값을 노말라이즈
	dir.Normalize();

	//보스의 포워드백터를 구한다.
	FVector p0 = bossActor->GetActorForwardVector();
	p0.Normalize();
	//방향,속도,시간
	FVector vt = p0 * 600 * GetWorld()->GetDeltaSeconds();
	//내위치에,방항,속도,시간을 보스의 포워드에 더하자.
	FVector p = bossPos + vt;
	bossActor->SetActorLocation(p);
	
	
	}
	
	if (currTime > 2.5)
	{
		//bossActor->Falling();
		bossActor->StopJumping();
	}

	if(currTime > 4)
	{	
		MultiRPC_SmashSkillEffect();
		currTime = 0;
		isChaseJumpSmash = false;
	}


}

void UBossFsmTest::MultiRPC_SmashSkillEffect_Implementation()
{
	USphereComponent* sr = bossActor->smashRange;

	FVector smashLocation = FVector(0, 0, -100);
	//범위공격 효과 출력
	//UParticleSystemComponent
	// myActor->smashRange->GetRelativeScale3D() 또는 FVector(10)
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), smashEffect, bossActor->GetActorLocation() + smashLocation, bossActor->GetActorRotation(), FVector(2)); //bossActor->smashRange->GetRelativeScale3D());

	//UE_LOG(LogTemp, Warning, TEXT("smash skill false"));

	//sr->OnComponentBeginOverlap.AddDynamic(this, &UBossFsmTest::OnOverlap);
}

bool UBossFsmTest::skillDelay(float delay)
{
	currTime += GetWorld()->DeltaTimeSeconds;

	UE_LOG(LogTemp, Warning, TEXT("skill delay : %f"), smashTime);

	if (smashTime >= delay)
	{
		currTime = 0;
		return true;
	}
	return false;
}

void UBossFsmTest::DamageDelayToSkill(float delay)
{
	currTime += GetWorld()->DeltaTimeSeconds;
	//UE_LOG(LogTemp, Warning, TEXT("smash skill currTime: %f"), currTime);

	//attackDelayTime = 1.5
	if (currTime > smashDamageDelay)
	{
		//목소리 동기화
		MultiRPC_DamageDelayToSkill();
		//플레이어 위치
		FVector playerPos = target->GetActorLocation();
		FVector AttackDirstnace = playerPos - bossActor->GetActorLocation();
		float damageZone = AttackDirstnace.Length();
		//UE_LOG(LogTemp, Warning, TEXT("smash skill"));

		//이펙트효과 멀티Rpc
		MultiRPC_SmashSkillEffect();
		
		
		for (int32 i = 0; i < allTarget.Num(); i++)
		{
				
			//보스와 올타겟의 거리를 구하자.
			float targetDistance = FVector::Distance(allTarget[i]->GetActorLocation(), bossActor->GetActorLocation());

			 
			if (targetDistance < smashDamageZone)
			{
				target = allTarget[i];
				//targetDistance < smashDamageZone 작을때
				PlayerDamageProcess(100);
			
			}
			
		
		}
		currTime = 0;
		isSmash = false;

	}

	//MultiRPC_DamageDelayToSkill();
}

void UBossFsmTest::MultiRPC_DamageDelayToSkill_Implementation()
{
	//스매쉬 소리
	bossSound->playbossSmashSound();

}


void UBossFsmTest::ChargeSkillLinetrace()
{
	//UE_LOG(LogTemp, Warning, TEXT("boss fsm SkillLinetrace "));
	//충돌결과 변수
	FHitResult hitInfo;
	//시작점
	FVector startPos = bossActor->GetActorLocation();
	//끝점
	FVector endPos = bossActor->GetActorLocation() + bossActor->GetActorForwardVector() * chargeDamageZone;
	FCollisionQueryParams params;
	params.AddIgnoredActor(bossActor);

	bool isHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECollisionChannel::ECC_Visibility, params);

	MultiRPC_ChargeSkillEffect();

	if (isHit)
	{
		// 만약에 맞은 Player 라면
		AActor* player = Cast<AGunPlayer>(hitInfo.GetActor());
		if (player)
		{
			// 데미지 주자
			PlayerDamageProcess(50);
			
		}		
		currTime = 0;
		isCharge = false;
		ChangeState(EEnemyState::ATTACK_DELAY);
	}

	//DrawDebugLine(GetWorld(), startPos, startPos + endPos, FColor::Red);


}

void UBossFsmTest::MultiRPC_ChargeSkillEffect_Implementation()
{
	if(!bossActor) return;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), chargeEffect, bossActor->GetActorLocation(), bossActor->GetActorRotation(), FVector(3));
	//차지사운드를 멀티로
	bossSound->playbossChargeSound();
}


void UBossFsmTest::MultiRPC_AttackDelayVoice_Implementation()
{
	bossSound->playbossDelayVoice();
}

void UBossFsmTest::MultiRPC_DieVoice_Implementation()
{
	bossSound->playbossDieVoice();
	bossActor->PlayAnimMontage(montage, 1.0f, TEXT("die"));
}

void UBossFsmTest::SkillOverlap(UPrimitiveComponent* abc, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	UE_LOG(LogTemp, Warning, TEXT("boss fsm skill overlap"));

	AActor* otherActor = target;
	//UGameplayStatics
	//UParticleSystemComponent
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), smashEffect, myActor->GetActorLocation(), myActor->GetActorRotation());


	AStoneActor* spawnStone = GetWorld()->SpawnActor<AStoneActor>(bossActor->stoneFactory);

	if (OtherActor->GetName().Contains(TEXT("player")))
	{
		PlayerDamageProcess(50);
		
		spawnStone->Destroy();
	}
}

void UBossFsmTest::PlayerDamageProcess(float damage)
{
	//피해량에서 방어도 보정
	damage -= target->statment.defancePoint;

	if (damage < 5)
	{
		damage = 5;
	}
	//플레이어 헬스 가져와서 데미지 넣어주기
	target->statment.healthPoint -= damage;
	target->bIsAttacked = true;
}



void UBossFsmTest::MultiRPC_PlayerDamageProcess_Implementation(float damage)
{
	damage -= target->statment.defancePoint;

	if(damage < 5)
	{	
		damage = 5;		
	}
	target->statment.healthPoint -= damage;
	target->bIsAttacked = true;
}

void UBossFsmTest::TestTarget()
{
	//타겟 체력이 0이면
	if (target->statment.healthPoint <= 0)
	{
		//모든타겟에서 인덱스 번호를 삭제
		allTarget.RemoveAt(targetplayerIndex);

		//타겟이 null이 아니면
		if (target != nullptr)
		{
			//타겟은 다른 idx
			target = allTarget[otherplayerIndex];
			//UE_LOG(LogTemp, Warning, TEXT(" other idx : %d"), otherplayerIndex);
		}
	}
}

