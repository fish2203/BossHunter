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
#include <../../../../../../../Source/Runtime/CoreUObject/Public/UObject/ObjectMacros.h>
#include <Components/BoxComponent.h>


// Sets default values for this component's properties
UBossFsmTest::UBossFsmTest()
{
	PrimaryComponentTick.bCanEverTick = true;

	// montage �о����
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("/Script/Engine.AnimMontage'/Game/BulePrint/Cheezebee/AM_TestBoss.AM_TestBoss'"));
	if (tempMontage.Succeeded())
	{
		montage = tempMontage.Object;
	}

	//punchȿ��, 
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempPunchEffect(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonRampage/FX/Particles/Rampage_v001_IceBlue/FX/P_Rampage_Ice_Melee_Impact.P_Rampage_Ice_Melee_Impact'"));

	if (tempPunchEffect.Succeeded())
	{
		punchEffect = tempPunchEffect.Object;
	}
	//chargeEffectȿ��, ���Ž���ũ
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempChargeEffect(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonRampage/FX/Particles/Rampage_v001_IceBlue/FX/P_Rampage_Ice_SmashArc.P_Rampage_Ice_SmashArc'"));

	if (tempChargeEffect.Succeeded())
	{
		chargeEffect = tempChargeEffect.Object;
	}
	//throwEffectȿ��
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempThrowEffect(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonRampage/FX/Particles/Rampage_v001_IceBlue/FX/P_Rampage_Ice_Melee_Impact.P_Rampage_Ice_Melee_Impact'"));

	if (tempThrowEffect.Succeeded())
	{
		throwEffect = tempThrowEffect.Object;
	}
	//smashEffectȿ��, ���̽� ĳ��Ʈ �ҷ�����.
	//ConstructorHelpers::FObjectFinder<UParticleSystem> tempEffect(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonRampage/FX/Particles/Rampage_v001_IceBlue/FX/P_Rampage_Ice_Enrage_Cast.P_Rampage_Ice_Enrage_Cast'"));

	//smashȿ�� ��׶��� ���� �ҷ�����.
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempSmashEffect(TEXT("/ Script / Engine.ParticleSystem'/Game/GroundAttacks/Fx/Earth/P_EarthGroundAttack.P_EarthGroundAttack'"));

	if (tempSmashEffect.Succeeded())
	{
		smashEffect = tempSmashEffect.Object;
	}

}


// Called when the game starts
void UBossFsmTest::BeginPlay()
{
	Super::BeginPlay();

	// �÷��̾ ã��
	//UGameplayStatics ��Ŭ���
	//ATestPlayer ��Ŭ���
	//�÷��̾ ã��.
	AActor* findActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGunPlayer::StaticClass());
	target = Cast<AGunPlayer>(findActor);
	UE_LOG(LogTemp, Warning, TEXT("beginplay find player"));

	//�迭 ������ �ּҰ��� �����ϴ� ��������
	TArray<AActor*> findallPlayer;
	//���ӿ��� ���Ŭ������ ã�Ƽ�, findallPlayer ����.
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGunPlayer::StaticClass(), findallPlayer);
	//UE_LOG(LogTemp, Warning, TEXT("find player"));

	//�÷��̾ AActor�� �����͵� ������ Gunplayer�� ���� ������ �Լ��� ȣ���ϱ� ���ؼ��� Gunplayer �ڷ����� ����մϴ�. ex) �÷��̾��� ü���� �����ö�

	//ã�� �÷��̾��� ���ڸ�ŭ ����
	
	for (int32 i = 0; i < findallPlayer.Num(); i++)
	{
		//��Ÿ����(���)�� ĳ������ �÷��̾� �迭 �ڷ��� �ֱ�
		//allTarget.Add(Cast<AGunPlayer>(findallPlayer[i]));

	}
	UE_LOG(LogTemp, Warning, TEXT("find player : %d"), findallPlayer.Num());

	// ���� ���͸� ã��
	//ATest_Boss ��Ŭ���
	bossActor = Cast<ATest_Boss>(GetOwner());
	UE_LOG(LogTemp, Warning, TEXT("find Boss"));

	// ������ �����Ǿ� �ִ� Anim Class ��������
	// ��Ŭ��� #include "AnimBossTest.h"
	USkeletalMeshComponent* mesh = bossActor->GetMesh();
	UAnimInstance* animInstance = mesh->GetAnimInstance();
	anim = Cast<UAnimBossTest>(animInstance);

	// �þ߰��� cos(�þ߰�) ���� ����
	float radianViewAngle = FMath::DegreesToRadians(viewAngle * 0.5f);
	viewAngle = FMath::Cos(radianViewAngle);

	// ���� ó�� ��ġ�� ��Ƶ���
	originPos = bossActor->GetActorLocation();

	//�浹�� ȣ��Ǵ� �Լ��� ���
	bossActor->GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &UBossFsmTest::SkillOverlap);

}


// Called every frame
void UBossFsmTest::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bossActor->HasAuthority() == false) return;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ���࿡ plater(target) �� ���ٸ� �Լ��� ������
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

void UBossFsmTest::MultiRPC_SetAnimation_Implementation(EEnemyState s, int attackType)
{
	// �ִϸ��̼� ���� ���� (����ȭ)
	anim->state = s;

	// ���� �ִϸ��̼� ����
	anim->attackType = (EAttackType)attackType;

	//�ǰ� �ִϸ��̼� ����
	if(s == EEnemyState::DAMAGE)
	{
		bossActor->PlayAnimMontage(montage, 1.0f, TEXT("hurt"));
	}
	//���� �ִϸ��̼� ����
	if (s == EEnemyState::DIE)
	{
		//bossActor->Chracter
		bossActor->PlayAnimMontage(montage, 1.0f, TEXT("die"));
	}
	
}

//�ѹ��� �����Ұ��� ���⿡ �߰��մϴ�.
void UBossFsmTest::ChangeState(EEnemyState s)
{
	// �ٲ�� ���¸� �������
	/*UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EBossState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -------> %s"),
			*enumPtr->GetNameStringByIndex((int32)currState),
			*enumPtr->GetNameStringByIndex((int32)s));
	}*/

	// �׺���̼� ���� ����
	//AAIController ��Ŭ���
	//ai->StopMovement();
	// ���� ���¸� ����
	//�ִϸ��̼� ����, currState = idle
	currState = s;

	// �ִϸ��̼� ���� ���� (����ȭ)
	anim->state = currState;

	// ���� �ð��� �ʱ�ȭ
	currTime = 0;

	
	switch (currState)
	{
	case EEnemyState::IDLE:
		break;
	case EEnemyState::MOVE:
		break;

	case EEnemyState::ATTACK:
	{

		//���� ������ �÷��̾� �������� �����ϴ�.
		BossViewAngle();
		// 4������ ���� �������� ����
		randAttackAnim = FMath::RandRange(0, 3);
		//2���� �Ӽ��� ���� ���� Ÿ���� ���⼭ ����
		randSkill = FMath::RandRange(0, 1);
		//�ִϸ��̼� ����
		anim->attackType = (EAttackType)randAttackAnim;

		//��� �÷��̾��.
		//MultiRPC_ChangeState(s);

		//PUNCH 0, Charge 1, THROW 2, SMASH 3
		if (randAttackAnim == 0)
		{
			//isChase = true;
			isPunch = true;
			//PunchSkill();
			
			//isCharge = true;
			//ChargeSkill();
			
			//ThorwSkill();

			//SmashSkill();

			//UE_LOG(LogTemp, Warning, TEXT("state attack no0 punch Skill"));
		}
		else if (randAttackAnim == 1)
		{
			isCharge = true;
			//ChargeSkill();
			//UE_LOG(LogTemp, Warning, TEXT("state attack no1 charge Skill"));
		}
		else if (randAttackAnim == 2)
		{
			ThorwSkill();
			//UE_LOG(LogTemp, Warning, TEXT("state attack no2 ThorwSkill"));
		}
		else
		{
			SmashSkill();
			//UE_LOG(LogTemp, Warning, TEXT("state attack no3 SmashSkill"));
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("state attack  break"));
	break;
	}
	switch (currState)
	{
	case EEnemyState::DAMAGE:
	{// 1. ������ ���� �̴´�. (1, 2)
		int32 rand = FMath::RandRange(1, 2);
		// 2. Damage01, Damage02 �� ���ڿ��� �����.
		FString sectionName = FString::Printf(TEXT("Damage0%d"), rand);
		// 3. Montage �÷���
		//myActor->PlayAnimMontage(montage, 1.0f, FName(*sectionName));
		bossActor->PlayAnimMontage(montage, 1.0f, TEXT("hurt"));
	}
	break;

	case EEnemyState::DIE:
	{// �״� �ִϸ��̼� �÷���
		bossActor->PlayAnimMontage(montage, 1.0f, TEXT("Die"));
		// �浹 ó�� ���� �ʰ� ����	
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

	// �ִϸ��̼� ����ȭ
	MultiRPC_SetAnimation(currState, randAttackAnim);
}

void UBossFsmTest::UpdateIdle()
{
	//UE_LOG(LogTemp, Warning, TEXT(" update idle"));

	// ���࿡ �÷��̾ �Ѿư� �� �ִٸ�
	if (CanTrace())
	{

		//UE_LOG(LogTemp, Warning, TEXT(" can trace"));

		int32 rand = 0;//FMath::RandRange(0, 1);
		//anim->attackType = (EAttackType)rand;

		if (rand == 0)
		{
			// ���¸� Move �� �ٲ��
			ChangeState(EEnemyState::MOVE);
			UE_LOG(LogTemp, Warning, TEXT(" updateidle change move"));
		}
		else

		{
			ChangeState(EEnemyState::ATTACK);
			UE_LOG(LogTemp, Warning, TEXT(" updateidle change attack"));

		}
	}
	// �׷��� �ʰ� idleDelayTime �� �����ٸ�
	else if (IsWaitComplete(idleDelayTime))
	{
		// ���¸� PATROL �� �ٲ��
		ChangeState(EEnemyState::IDLE);

		UE_LOG(LogTemp, Warning, TEXT(" no trace "));

	}
}

// ƽ���� �����մϴ�.
void UBossFsmTest::UpdateMove()
{
	//�÷��̾��� ��ġ
	FVector playerPos = target->GetActorLocation() - bossActor->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("UpdateMove %f "), dir.Length());

	// ó�� ��ġ�� ���� ��ġ�� �Ÿ�
	float distance = FVector::Distance(originPos, bossActor->GetActorLocation());

	// ���࿡ distance �� moveRange ���� Ŀ���� 
	if (distance > moveRange)
	{
		// Return ���·� ��ȯ
		ChangeState(EEnemyState::RETURN);
		//UE_LOG(LogTemp, Warning, TEXT(" update move change RETURN "));
	}

	else
	{
		// 2. target ��ġ�� �������� (Navigation  ����� ���ؼ�)
		//ai->MoveToLocation(target->GetActorLocation());

		//// 2. �� �������� ��������. 
		bossActor->AddMovementInput(playerPos.GetSafeNormal());
		//UE_LOG(LogTemp, Warning, TEXT("UpdateMove  AddMovementInput"));

		// 3. �÷��̾���� �Ÿ��� ���� �������� ������
		float dist = playerPos.Length();
		//UE_LOG(LogTemp, Warning, TEXT("dist : %f"), dist);

		currTime += GetWorld()->DeltaTimeSeconds;
		//UE_LOG(LogTemp, Warning, TEXT(" update move time %f"), currTime);


		//�÷��̾���� �Ÿ��� playerReChase���� ������
		if (dist < playerReChase)
		{

			//�Ÿ��� attackRange ������ ����
			if (dist < attackRange)
			{

				ChangeState(EEnemyState::ATTACK);
				//UE_LOG(LogTemp, Warning, TEXT(" update move ischase"));
				currTime = 0;
			}

			//2�ʰ� ������ ����
			else if (currTime > 2)
			{

				// 4. ���� ���¸� ATTACK �� �ٲ���
				ChangeState(EEnemyState::ATTACK);
				//UE_LOG(LogTemp, Warning, TEXT(" upmove change Attack "));
				currTime = 0;
			}

		}


	}

}

//ƽ���� ��� �����Ұ̴ϴ�.
void UBossFsmTest::UpdateAttack()
{
	if (isChase)
	{
		//������ų�� ƽ���� ����
		ChasePlayer();
	}

	else if (isPunch)
	{
		//��ġ ��ų�� ƽ���� ����
		PunchSkill();
	}

	else if (isCharge)
	{
		//������ų�� ƽ���� ����
		ChargeSkill();
	}
	else if (isSmash)
	{
		DamageDelayToSkill(1);
	}
	//1�ʴ�� �� ���� �����̷�
	else
	{
		currTime += GetWorld()->DeltaTimeSeconds;


		if (currTime > 1)
		{
			currTime = 0;
			UE_LOG(LogTemp, Warning, TEXT("UpdateAttack chansge attack delay after 1 second "));
			ChangeState(EEnemyState::ATTACK_DELAY);

		}
	}
}

void UBossFsmTest::UpdateAttackDelay()
{

	// 1. �ð��� �帣�� �Ѵ�.
	currTime += GetWorld()->DeltaTimeSeconds;
	// 2. ���࿡ Attack Delay �ð��� ������
	//attackDelayTime = 1
	if (currTime > attackDelayTime)

		//UE_LOG(LogTemp, Warning, TEXT("update delay"));


		if (IsWaitComplete(attackDelayTime))
		{
			// �÷������ �Ÿ�
			float dist = FVector::Distance(target->GetActorLocation(), bossActor->GetActorLocation());
			// �� �Ÿ��� ���ݹ���- > ��¥ ����
			if (dist < attackRange)
			{
				// 3. ���� ���·� ����
				ChangeState(EEnemyState::ATTACK);
				//UE_LOG(LogTemp, Warning, TEXT(" update attack delay change attak"));

			}

			// �������� -> �̵� 
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
			// �� �ܴ� -> ���
			else
			{
				ChangeState(EEnemyState::IDLE);
				//UE_LOG(LogTemp, Warning, TEXT(" update attack delay change idle"));

			}

			// 4. ����ð� �ʱ�ȭ
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
		// IDLE ���·� ��ȯ
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
	// �� ��ġ�� ó�� ��ġ�� �Ÿ��� ���Ѵ�.
	float dist = FVector::Distance(originPos, bossActor->GetActorLocation());
	//UE_LOG(LogTemp, Warning, TEXT("dist : %f"), dist);

	// �� �Ÿ��� 0�̸�
	if (dist < 50)
	{
		// IDLE ���·� ��ȯ
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

	MultiPlayTarget();
	//UE_LOG(LogTemp, Warning, TEXT(" cantarce MultiPlayTarget "));

	// 1. �̱��÷��̾���� �Ÿ��� ������.
	FVector dir = target->GetActorLocation() - bossActor->GetActorLocation();
	float dist = dir.Length();
	//UE_LOG(LogTemp, Warning, TEXT(" trace dir %f"), dir.Length());

		// 2. ���࿡ �Ÿ��� �������� ���� ������
	if (dist < traceRange || bossActor->currbossHP<1000)
	{

		//��ɽ����� ������
		huntStart = true;
		//��ɽ����� ���̸�
		if (huntStart)
		{
			return true;
		}

		// ���� �չ���� �÷�� ���ϴ� ������ ����
		float dot = FVector::DotProduct(dir.GetSafeNormal(), bossActor->GetActorForwardVector());

		FVector bossStarLocation = GetOwner()->GetActorLocation();
		FColor DebugColor;

		//#include "DrawDebugHelpers.h"
		DrawDebugLine(GetWorld(), bossStarLocation, bossStarLocation + dir, DebugColor.Red, false, 5.0f, 0, 1.0f);


		if (dot > viewAngle)
		{

			UE_LOG(LogTemp, Warning, TEXT(" dot > view "));

			// �÷��̾ ���ϴ� ���⿡ ��ֹ��� �ִ��� �Ǵ� (LineTrace)
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

			//target = ���尡��� Player�� ����.
			return true;
		}

		// ������ ���� acos ����
		float radianAngle = FMath::Acos(dot);
		// degree ������ ��������
		float degreeAngle = FMath::RadiansToDegrees(radianAngle);
		// ���࿡ �÷��̾ �þఢ(60)�� ���Դٸ� (degreeAngle <= 60 / 2) 
		if (degreeAngle <= 180 * 0.5f)
		{
			return true;
		}
	}

	return false;
	//return true;
	//HuntStart();
	//return false;
	//


}

void UBossFsmTest::MultiPlayTarget()
{
	//�Ÿ��� ���ϱ� ���� �ּҰ��� ���� �ִ밪
	float targetChase = 100000;
	//�÷��̾� �ε����� ������ ����
	int32 targetplayerIndex = 0;
	int32 otherplayerIndex = 0;

	//��Ƽ�÷��̾��� �Ÿ��� ���ϴ� for��, ���Ѱ��� target�� ����.
	for (int32 i = 0; i < allTarget.Num(); i++)
	{
		//������ ��Ÿ���� �Ÿ��� ������.
		float targetDistance = FVector::Distance(allTarget[i]->GetActorLocation(), bossActor->GetActorLocation());
		//UE_LOG(LogTemp, Warning, TEXT(" min : %f"), targetDistance);

		//���� �Ÿ��� targetDistance ũ�ٸ�
		if (targetDistance < targetChase)
		{
			//���� �Ÿ��� ����
			targetChase = targetDistance;
			//index ���ڸ� �÷��̾� ������ ����.
			targetplayerIndex = i;

		}
		else
		{
			otherplayerIndex = i;
		}

	}

	//Ÿ�� ���� ���尡��� �÷�� �ε����� ����.
	target = allTarget[targetplayerIndex];

	/*if (target->statment.healthPoint >= 0)
	{
		target = allTarget[otherplayerIndex];
	}*/

	//�÷��̾� ü���� ����մϴ�.
	//	UE_LOG(LogTemp, Warning, TEXT(" target hp: %f"), target->statment.healthPoint);

	//UE_LOG(LogTemp, Warning, TEXT(" minplayer : %d"), targetplayerIndex);

	//UE_LOG(LogTemp, Warning, TEXT(" otherplayer : %d"), otherplayerIndex);
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
	//�÷��̾� ������ ������.
	FVector dir = playerPos - bossPos;

	bossActor->SetActorRotation(dir.Rotation());
}

void UBossFsmTest::ChasePlayer()
{
	//�÷��̾��� ��ġ
	FVector playerPos = target->GetActorLocation() - bossActor->GetActorLocation();
	//�÷��̾��� ���Ⱚ
	float dist = playerPos.Length();

	currTime += GetWorld()->GetDeltaSeconds();
	if (currTime < 2)
	{
		//0�̸� ��� �̵�
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

		//1�̸� ����
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
	FVector playerPos = target->GetActorLocation();
	FVector AttackDirstnace = playerPos - bossActor->GetActorLocation();
	float damageZone = AttackDirstnace.Length();

	//UE_LOG(LogTemp, Warning, TEXT(" fsm punchskill "));
	currTime += GetWorld()->DeltaTimeSeconds;

	if (currTime > 0.36)
	{
		//UE_LOG(LogTemp, Warning, TEXT(" fsm punchskill 0.5 time "));

		MultiRPC_PunchSkillEffect();

		//��������<punchDagmageZone
		if (damageZone < punchDagmageZone)
		{
			//UE_LOG(LogTemp, Warning, TEXT(" fsm punchskill damagezone "));

			PlayerDamageProcess(50);
			MultiRPC_PlayerDamageProcess(50);
			//ChangeState(EEnemyState::MOVE);
		}
		currTime = 0;
		isPunch = false;
	}
}

void UBossFsmTest::MultiRPC_PunchSkillEffect_Implementation()
{
	//ATest_Boss* bossActor = Cast<ATest_Boss>(GetOwner());

	USceneComponent* meshChild = bossActor->GetMesh()->GetChildComponent(0);
	UBoxComponent* rightHandBox = Cast<UBoxComponent>(meshChild);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), punchEffect, rightHandBox->GetComponentLocation(), rightHandBox->GetComponentRotation(), FVector(4));
}

void UBossFsmTest::ChargeSkill()
{

	//�÷��̾� ��ġ ������.
	FVector playerPos = target->GetActorLocation();
	//�÷��̾� ��ġ ���Ⱚ�� ��������.
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
		UE_LOG(LogTemp, Warning, TEXT("charge false"));
	}

	//�����ڵ�
	//�÷��̾� ��ġ ���ϱ�
	//FVector playerPos = target->GetActorLocation();
	//���� ��ġ ���ϱ�
	//FVector bossPos = myActor->GetActorLocation();
	//�÷��̾�� ������ġ�� ������ ����
	//FVector p0 = playerPos - bossPos;
	//���Ⱚ�� �븻����������.
	//p0.Normalize();
	//�÷��̾� ����, �ӵ�, �ð�
	//FVector p1 = p0 * chargeSpeed * GetWorld()->GetDeltaSeconds();
	//������ ��ġ�� ����,�ӵ�,�ð��� ������.
	//FVector p2 = bossPos + p1;
	//����ġ�� p2�� �̵���Ű��.
	//myActor->SetActorLocation(p2);
	//myActor->AddMovementInput(p0);

	//DrawDebugLine(GetWorld(), bossPos, playerPos , FColor::Red);


}

void UBossFsmTest::ChargeForward()
{
	// ������ ������ ���� �������� ��������.

	//�÷��̾� ��ġ ������.
	FVector playerPos = target->GetActorLocation();
	//�÷��̾� ��ġ ���Ⱚ�� ��������.
	FVector bossPos = playerPos - bossActor->GetActorLocation();
	bossPos.Normalize();


	//�÷��̾� ��ġ���� ������ġ�� ������ ����
	FVector p0 = bossActor->GetActorForwardVector();
	//���Ⱚ�� �븻����������.
	p0.Normalize();
	//������ ������ ���Ϳ��� �÷��̾� �������� ���� �����̰� ����, �ӵ�, �ð�
	FVector p1 = p0 * chargeForwardSpeed * GetWorld()->GetDeltaSeconds();
	//�� ��ġ�� p1�� ������.
	FVector p2 = bossActor->GetActorLocation() + p1;
	//����ġ�� p2�� �̵���Ű��.
	bossActor->SetActorLocation(p2);

	//DrawDebugLine(GetWorld(), p0, p2, FColor::Red);
	//UE_LOG(LogTemp, Warning, TEXT(" charge skill forward"));

	//SkillOverlap(UPrimitiveComponent * abc, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	//bossActor->GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &UBossFsmTest::SkillOverlap);

	//bossActor->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &UBossFsmTest::SkillOverlap);

	ChargeSkillLinetrace();


}

void UBossFsmTest::ChargeChase()
{

	//�÷��̾� ��ġ ������.
	FVector playerPos = target->GetActorLocation();
	//�÷��̾� ��ġ ���Ⱚ�� ��������.
	FVector bossPos = playerPos - bossActor->GetActorLocation();
	bossPos.Normalize();

	// �÷��̾ ��������.

	FVector p0 = bossActor->GetActorLocation();
	FVector vt = bossPos * chargeChaseSpeed * GetWorld()->DeltaTimeSeconds;
	FVector p = p0 + vt;
	bossActor->SetActorLocation(p);
	bossActor->SetActorRotation(bossPos.Rotation());
	//DrawDebugLine(GetWorld(), bossPos, playerPos, FColor::Red);
	//UE_LOG(LogTemp, Warning, TEXT(" charge skill chage"));

	ChargeSkillLinetrace();

}

void UBossFsmTest::ThorwSkill()
{
	// �Ѿ˰���(BP_Bullet) ���� �Ѿ��� �ϳ� �����Ѵ�.
	AStoneActor* spawnStone = GetWorld()->SpawnActor<AStoneActor>(bossActor->stoneFactory);
	// ������ �Ѿ��� ���� ��ġ�� ���´�.
	spawnStone->SetActorLocation(bossActor->GetActorLocation());
	spawnStone->isThorw = true;
	spawnStone->target = target;

	//	spawnStone->meshComp->OnComponentBeginOverlap.AddDynamic(this, &UBossFsmTest::SkillOverlap);

		//UE_LOG(LogTemp, Warning, TEXT("bossfsm Throw Skill"));

		//spawnStone->ThrowSkillLinetrace();



}

void UBossFsmTest::SmashSkill()
{
	//1���Ŀ� ����� �������� ������ ���ظ� �ݴϴ�.
	isSmash = true;
	MultiRPC_SmashSkillEffect();
	
}

void UBossFsmTest::MultiRPC_SmashSkillEffect_Implementation()
{
	USphereComponent* sr = bossActor->smashRange;

	FVector smashLocation = FVector(0, 0, -70);
	//�������� ȿ�� ���
	//UParticleSystemComponent
	// myActor->smashRange->GetRelativeScale3D() �Ǵ� FVector(10)
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
	MultiRPC_DamageDelayToSkill();
}

void UBossFsmTest::MultiRPC_DamageDelayToSkill_Implementation()
{
	currTime += GetWorld()->DeltaTimeSeconds;
	//UE_LOG(LogTemp, Warning, TEXT("smash skill currTime: %f"), currTime);

	//attackDelayTime = 1
	if (currTime > smashDamageDelay)
	{

		FVector playerPos = target->GetActorLocation();
		FVector AttackDirstnace = playerPos - bossActor->GetActorLocation();
		float damageZone = AttackDirstnace.Length();
		//UE_LOG(LogTemp, Warning, TEXT("smash skill"));

		//�Ÿ��� < smashDamageZone ������
		if (damageZone < smashDamageZone)
		{
			PlayerDamageProcess(100);
		}
		currTime = 0;
		isSmash = false;
	}
}

void UBossFsmTest::ChargeSkillLinetrace()
{
	//UE_LOG(LogTemp, Warning, TEXT("boss fsm SkillLinetrace "));
	//�浹��� ����
	FHitResult hitInfo;
	//������
	FVector startPos = bossActor->GetActorLocation();
	//����
	FVector endPos = bossActor->GetActorLocation() + bossActor->GetActorForwardVector() * chargeDamageZone;
	FCollisionQueryParams params;
	params.AddIgnoredActor(bossActor);

	bool isHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECollisionChannel::ECC_Visibility, params);

	MultiRPC_ChargeSkillEffect();

	if (isHit)
	{
		// ���࿡ ���� Player ���
		AActor* player = Cast<AGunPlayer>(hitInfo.GetActor());
		if (player)
		{
			// ������ ����
			PlayerDamageProcess(50);
			ChangeState(EEnemyState::ATTACK_DELAY);
		}
		else
		{

			ChangeState(EEnemyState::ATTACK_DELAY);
		}

	}

	DrawDebugLine(GetWorld(), startPos, startPos + endPos, FColor::Red);


}

void UBossFsmTest::MultiRPC_ChargeSkillEffect_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), chargeEffect, bossActor->GetActorLocation(), bossActor->GetActorRotation(), FVector(3));
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
	//���ط����� �� ����
	damage -= target->statment.defancePoint;

	if (damage < 5)
	{
		damage = 5;
	}
	//�÷��̾� �ｺ �����ͼ� ������ �־��ֱ�
	target->statment.healthPoint -= damage;
}



void UBossFsmTest::MultiRPC_PlayerDamageProcess_Implementation(float damage)
{
	damage -= target->statment.defancePoint;

	if(damage < 5)
	{	
		damage = 5;		
	}
	target->statment.healthPoint -= damage;
}

