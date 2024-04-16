// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFsmTest.h"
#include <Kismet/GameplayStatics.h>
#include "TestPlayer.h"
#include "Test_Boss.h"
#include "AnimBossTest.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include <../../../../../../../Source/Runtime/AIModule/Classes/AIController.h>
#include "GunPlayer.h"
#include "StoneActor.h"
#include <Components/SphereComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>
#include "PlayerStat.h"
#include "BossHunterCharacter.h"


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

	//smash효과, 아이스 캐스트 불러오자.
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempEffect(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonRampage/FX/Particles/Rampage_v001_IceBlue/FX/P_Rampage_Ice_Enrage_Cast.P_Rampage_Ice_Enrage_Cast'"));
	
	
	if (tempEffect.Succeeded())
	{
		smashEffect = tempEffect.Object;
	}



}


// Called when the game starts
void UBossFsmTest::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어를 찾자
	//UGameplayStatics 인클루드
	//ATestPlayer 인클루드
	AActor* findActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGunPlayer::StaticClass());
	target = Cast<AGunPlayer>(findActor);
	UE_LOG(LogTemp, Warning, TEXT("find player"));

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
}


// Called every frame
void UBossFsmTest::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 만약에 plater(target) 이 없다면 함수를 나가자
	if (target == nullptr) return;

	switch (currState)
	{
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

	if (isCharge == true)
	{
		ChargeSkill();

		currTime += GetWorld()->GetDeltaSeconds();

		if (currTime < 2)
		{
			isCharge = false;
			currTime = 0;
			UE_LOG(LogTemp, Warning, TEXT(" tick charge end"));
		}
		
	}

}

void UBossFsmTest::ChangeState(EEnemyState s)
{
	// 바뀌는 상태를 출력하자
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EBossState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -------> %s"),
		*enumPtr->GetNameStringByIndex((int32)currState),
		*enumPtr->GetNameStringByIndex((int32)s));

	}

	// 네비게이션 동작 멈춰
	//AAIController 인클루드
	//ai->StopMovement();


	// 현재 상태를 갱신
	currState = s;

	// 애니메이션 상태 갱신
	anim->state = currState;

	// 현재 시간을 초기화
	currTime = 0;

	//애니메이션 갱신
	switch (currState)
	{
		case EEnemyState::IDLE:
			break;
		case EEnemyState::MOVE:
			break;
		
		case EEnemyState::ATTACK:
		{
		// 4가지중 랜덤 공격할지 설정
		int32 rand = FMath::RandRange(0,3);
		anim->attackType = (EAttackType)rand;

			//보스 방향을 플레이어 방향으로 돌립니다.
			BossViewAngle();

			//PUNCH 0, Charge 1, THROW 2, SMASH 3
			
			if(rand == 0) 
			{
				PunchSkill();
				/*ThorwSkill();
				SmashSkill();*/
				UE_LOG(LogTemp, Warning, TEXT("state attack no0 punch Skill"));
				break;			
			}
			else if(rand == 1)
			{	
				ChargeSkill();
				UE_LOG(LogTemp, Warning, TEXT("state attack no1 charge Skill"));
				break;
			}
			else if (rand == 2)
			{	
				ThorwSkill();
				UE_LOG(LogTemp, Warning, TEXT("state attack no2 ThorwSkill"));
				break;
			}
			else (rand == 3);
			{
				SmashSkill();
				UE_LOG(LogTemp, Warning, TEXT("state attack no3 SmashSkill"));
				break;
			}
			break;
		}	



		break;
		
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
		{// 죽는 애니메이션 플레이
			bossActor->PlayAnimMontage(montage, 1.0f, TEXT("Die"));
		// 충돌 처리 되지 않게 하자	
		//UCapsuleComponent
			bossActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// myActor->Destroy();
		}
		break;

		case EEnemyState::RETURN:
			bossActor->SetActorLocation(originPos);
		break;
		
		default:
		break;

	}
		
}
		

void UBossFsmTest::UpdateIdle()
{
	//UE_LOG(LogTemp, Warning, TEXT(" update idle"));

	// 만약에 플레이어를 쫓아갈 수 있다면
	if (CanTrace())
	{
		
		UE_LOG(LogTemp, Warning, TEXT(" can trace"));
		
		int32 rand = FMath::RandRange(0, 1);
		//anim->attackType = (EAttackType)rand;

		if(rand == 0)
		{
		// 상태를 Move 로 바꿔라
		ChangeState(EEnemyState::MOVE);

		UE_LOG(LogTemp, Warning, TEXT(" update move"));
		}
		else

		{
			ChangeState(EEnemyState::ATTACK);

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

void UBossFsmTest::UpdateMove()
{
	//플레이어의 위치 - 
	FVector dir = target->GetActorLocation() - bossActor->GetActorLocation();
	
	//UE_LOG(LogTemp, Warning, TEXT("UpdateMove %f "), dir.Length());

	// 처음 위치와 나의 위치의 거리
	float distance = FVector::Distance(originPos, bossActor->GetActorLocation());
	
	// 만약에 distance 가 moveRange 보다 커지면 
	if (distance > moveRange)
	{
		// IDLE 상태로 전환
		ChangeState(EEnemyState::RETURN);
		UE_LOG(LogTemp, Warning, TEXT(" update move change RETURN "));
	}
	else
	{
		// 2. target 위치로 움직이자 (Navigation  기능을 통해서)
		//ai->MoveToLocation(target->GetActorLocation());
		
		//// 2. 그 방향으로 움직이자. 
		bossActor->AddMovementInput(dir.GetSafeNormal());

		// 3. 플레이어와의 거리가 공격 범위보다 작으면
		float dist = dir.Length();
		if (dist < attackRange)
		{
			// 4. 현재 상태를 ATTACK 로 바꾸자
			ChangeState(EEnemyState::ATTACK);
			UE_LOG(LogTemp, Warning, TEXT(" upmove change Attack "));
		}
	}
}

void UBossFsmTest::UpdateAttack()
{
	currTime += GetWorld()->DeltaTimeSeconds;
	
	if (currTime > 1)
	{
		currTime = 0;
		ChangeState(EEnemyState::ATTACK_DELAY);
	}
	
}

void UBossFsmTest::UpdateAttackDelay()
{

	// 1. 시간을 흐르게 한다.
	currTime += GetWorld()->DeltaTimeSeconds;
	// 2. 만약에 Attack Delay 시간이 지나면
	if (currTime > attackDelayTime)

	//UE_LOG(LogTemp, Warning, TEXT("update delay"));

	if (IsWaitComplete(attackDelayTime))
	{
		// 플레어와의 거리
		float dist = FVector::Distance(target->GetActorLocation(), bossActor->GetActorLocation());
		// 그 거리가 공격범위- > 진짜 공격
		if (dist < attackRange)
		{
			// 3. 공격 상태로 가라
			ChangeState(EEnemyState::ATTACK);
			UE_LOG(LogTemp, Warning, TEXT(" update attack delay change attak"));

		}
	
		// 인지범위 -> 이동 
		else if (CanTrace())
		{	
			int32 rand = FMath::RandRange(0, 1);

			if (rand == 0)
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
			UE_LOG(LogTemp, Warning, TEXT(" update attack delay change idle"));

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
	if (IsWaitComplete(100))
	{
		bossActor->PlayAnimMontage(montage, 1.0f, TEXT("die"));
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
	// 1. 플레이어와의 거리를 구하자.
	// 
	FVector dir = target->GetActorLocation() - bossActor->GetActorLocation();
	float dist = dir.Length();

	//UE_LOG(LogTemp, Warning, TEXT(" trace dir %f"), dir.Length());
	
	if (huntStart == true) 
	{
		return true;

	}
	// 2. 만약에 거리가 인지범위 보다 작으면
	if (dist < traceRange)
	{
		// 나의 앞방향과 플레어를 향하는 방향을 내적
		float dot = FVector::DotProduct(dir.GetSafeNormal(), bossActor->GetActorForwardVector());

		FVector bossStarLocation = GetOwner()->GetActorLocation();
		FColor DebugColor;

		//#include "DrawDebugHelpers.h"
		// Persistent (not persistent in this case)
		// Thickness
		// Depth priority
		// Duration
		//DrawDebugCone(GetWorld(),bossStarLocation,bossStarLocation + dir,180, 1, 100, 0, DebugColor.Red, false, 5.0f, 0, 1.0f);

		DrawDebugLine(GetWorld(),bossStarLocation,bossStarLocation + dir,DebugColor.Red,false,5.0f,0,1.0f);

		

		huntStart = true;
		if (dot > viewAngle)
		{

			UE_LOG(LogTemp, Warning, TEXT(" dot > view ") );
			
			// 플레이어를 향하는 방향에 장애물이 있는지 판단 (LineTrace)
			FVector start = bossActor->GetActorLocation();
			FVector end = target->GetActorLocation();
			FHitResult hitInfo;
			FCollisionQueryParams param;
			param.AddIgnoredActor(bossActor);


			bool hit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_Visibility, param);

			if (hit)
			{
				if (hitInfo.GetActor()->GetName().Contains(TEXT("Player")) == false)
				{
					

					UE_LOG(LogTemp, Warning, TEXT(" crace huntStart = true "));
					return false;
				}
			}

			return true;
		}
		
		//// 내적한 값을 acos 하자
		//float radianAngle = FMath::Acos(dot);
		//// degree 값으로 변경하자
		//float degreeAngle = FMath::RadiansToDegrees(radianAngle);
		//// 만약에 플레이어가 시약각(60)에 들어왔다면 (degreeAngle <= 60 / 2) 
		//if (degreeAngle <= 180 * 0.5f)
		//{
		//	return true;
		//}
	}

	return false;
	
}

void UBossFsmTest::BossViewAngle()
{
	FVector playerPos = target->GetActorLocation();
	FVector bossPos = bossActor->GetActorLocation();
	//플레이어 방향을 구하자.
	FVector dir = playerPos - bossPos;

	bossActor->SetActorRotation(dir.Rotation());
}

void UBossFsmTest::ChangeAttackType()
{

}

void UBossFsmTest::PunchSkill()
{
	FVector playerPos = target->GetActorLocation();
	FVector AttackDirstnace = playerPos - bossActor->GetActorLocation();
	float damageZone = AttackDirstnace.Length();

	if(damageZone < 200)
	{ 
		PlayerDamageProcess(50);
	}
}

void UBossFsmTest::ChargeSkill()
{
	
	//타겟의 위치를 구하자.
	FVector playerPos = target->GetActorLocation();
	posCheck = playerPos - bossActor->GetActorLocation();
	posCheck.Normalize();

	FVector p0 = bossActor->GetActorLocation();
	FVector vt = posCheck * chargeSpeed * GetWorld()->DeltaTimeSeconds;
	FVector p = p0 + vt;
	//myActor->AddMovementInput(p);
	//myActor->SetActorLocation(p);
	
	//UE_LOG(LogTemp, Warning, TEXT("%s"), p.Length());
	
	//플레이어 위치에서 보스위치를 방향을 빼자
	FVector pp0 = playerPos - bossActor->GetActorLocation();
	//방향값을 노말라이즈하자.
	pp0.Normalize();
	//보스의 포워드 벡터에서 플레이어 방향으로 돌이 움직이게 하자, 속도, 시간
	FVector pp1 = pp0 * chargeSpeed * GetWorld()->GetDeltaSeconds();
	//내 위치에 p1를 더하자.
	FVector pp2 = bossActor->GetActorLocation() + pp1;
	//내위치를 p2로 이동시키자.
	bossActor->SetActorLocation(pp2);

	//FVector playerPos = target->GetActorLocation();
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

	//UE_LOG(LogTemp, Warning, TEXT(" charge skill Active"));

}

void UBossFsmTest::ThorwSkill()
{	
	// 총알공장(BP_Bullet) 에서 총알을 하나 생성한다.
	AStoneActor* spawnStone = GetWorld()->SpawnActor<AStoneActor>(bossActor->stoneFactory);
	// 생성된 총알을 나의 위치에 놓는다.
	spawnStone->SetActorLocation(bossActor->GetActorLocation());
	spawnStone->isThorw = true;
	
	spawnStone->meshComp->OnComponentBeginOverlap.AddDynamic(this, &UBossFsmTest::ThrowSkillOverlap);

	//UE_LOG(LogTemp, Warning, TEXT("bossfsm Throw Skill"));
}

void UBossFsmTest::SmashSkill()
{
	USphereComponent* sr = bossActor->smashRange;

	//범위공격 효과 출력
	//UParticleSystemComponent
	// myActor->smashRange->GetRelativeScale3D() 또는 FVector(10)
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), smashEffect, bossActor->GetActorLocation(), bossActor->GetActorRotation(), bossActor->smashRange->GetRelativeScale3D());

	FVector playerPos = target->GetActorLocation();
	FVector AttackDirstnace = playerPos - bossActor->GetActorLocation();
	float damageZone = AttackDirstnace.Length();
	
		if (damageZone < 400)
		{
				PlayerDamageProcess(100);
		}
		

	
	//sr->OnComponentBeginOverlap.AddDynamic(this, &UBossFsmTest::OnOverlap);
}

bool UBossFsmTest::skillDelay(float delay)
{
	smashTime += GetWorld()->DeltaTimeSeconds;
	
	UE_LOG(LogTemp, Warning, TEXT("skill delay : %f"), smashTime);
	
	if(smashTime >= delay)
	{
		smashTime = 0;
		return true;
	}
	return false;
}

void UBossFsmTest::ThrowSkillOverlap(UPrimitiveComponent* abc, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
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
	//플레이어 헬스 가져와서 데미지 넣어주기
	target->statment.healthPoint -= damage;

}

