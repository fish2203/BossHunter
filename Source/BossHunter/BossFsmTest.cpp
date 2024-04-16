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

	// montage �о����
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("/Script/Engine.AnimMontage'/Game/BulePrint/Cheezebee/AM_TestBoss.AM_TestBoss'"));
	if (tempMontage.Succeeded())
	{
		montage = tempMontage.Object;
	}

	//smashȿ��, ���̽� ĳ��Ʈ �ҷ�����.
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

	// �÷��̾ ã��
	//UGameplayStatics ��Ŭ���
	//ATestPlayer ��Ŭ���
	AActor* findActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGunPlayer::StaticClass());
	target = Cast<AGunPlayer>(findActor);
	UE_LOG(LogTemp, Warning, TEXT("find player"));

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
}


// Called every frame
void UBossFsmTest::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ���࿡ plater(target) �� ���ٸ� �Լ��� ������
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
	// �ٲ�� ���¸� �������
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EBossState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -------> %s"),
		*enumPtr->GetNameStringByIndex((int32)currState),
		*enumPtr->GetNameStringByIndex((int32)s));

	}

	// �׺���̼� ���� ����
	//AAIController ��Ŭ���
	//ai->StopMovement();


	// ���� ���¸� ����
	currState = s;

	// �ִϸ��̼� ���� ����
	anim->state = currState;

	// ���� �ð��� �ʱ�ȭ
	currTime = 0;

	//�ִϸ��̼� ����
	switch (currState)
	{
		case EEnemyState::IDLE:
			break;
		case EEnemyState::MOVE:
			break;
		
		case EEnemyState::ATTACK:
		{
		// 4������ ���� �������� ����
		int32 rand = FMath::RandRange(0,3);
		anim->attackType = (EAttackType)rand;

			//���� ������ �÷��̾� �������� �����ϴ�.
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
		
}
		

void UBossFsmTest::UpdateIdle()
{
	//UE_LOG(LogTemp, Warning, TEXT(" update idle"));

	// ���࿡ �÷��̾ �Ѿư� �� �ִٸ�
	if (CanTrace())
	{
		
		UE_LOG(LogTemp, Warning, TEXT(" can trace"));
		
		int32 rand = FMath::RandRange(0, 1);
		//anim->attackType = (EAttackType)rand;

		if(rand == 0)
		{
		// ���¸� Move �� �ٲ��
		ChangeState(EEnemyState::MOVE);

		UE_LOG(LogTemp, Warning, TEXT(" update move"));
		}
		else

		{
			ChangeState(EEnemyState::ATTACK);

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

void UBossFsmTest::UpdateMove()
{
	//�÷��̾��� ��ġ - 
	FVector dir = target->GetActorLocation() - bossActor->GetActorLocation();
	
	//UE_LOG(LogTemp, Warning, TEXT("UpdateMove %f "), dir.Length());

	// ó�� ��ġ�� ���� ��ġ�� �Ÿ�
	float distance = FVector::Distance(originPos, bossActor->GetActorLocation());
	
	// ���࿡ distance �� moveRange ���� Ŀ���� 
	if (distance > moveRange)
	{
		// IDLE ���·� ��ȯ
		ChangeState(EEnemyState::RETURN);
		UE_LOG(LogTemp, Warning, TEXT(" update move change RETURN "));
	}
	else
	{
		// 2. target ��ġ�� �������� (Navigation  ����� ���ؼ�)
		//ai->MoveToLocation(target->GetActorLocation());
		
		//// 2. �� �������� ��������. 
		bossActor->AddMovementInput(dir.GetSafeNormal());

		// 3. �÷��̾���� �Ÿ��� ���� �������� ������
		float dist = dir.Length();
		if (dist < attackRange)
		{
			// 4. ���� ���¸� ATTACK �� �ٲ���
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

	// 1. �ð��� �帣�� �Ѵ�.
	currTime += GetWorld()->DeltaTimeSeconds;
	// 2. ���࿡ Attack Delay �ð��� ������
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
			UE_LOG(LogTemp, Warning, TEXT(" update attack delay change attak"));

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
			UE_LOG(LogTemp, Warning, TEXT(" update attack delay change idle"));

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
	// 1. �÷��̾���� �Ÿ��� ������.
	// 
	FVector dir = target->GetActorLocation() - bossActor->GetActorLocation();
	float dist = dir.Length();

	//UE_LOG(LogTemp, Warning, TEXT(" trace dir %f"), dir.Length());
	
	if (huntStart == true) 
	{
		return true;

	}
	// 2. ���࿡ �Ÿ��� �������� ���� ������
	if (dist < traceRange)
	{
		// ���� �չ���� �÷�� ���ϴ� ������ ����
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

			return true;
		}
		
		//// ������ ���� acos ����
		//float radianAngle = FMath::Acos(dot);
		//// degree ������ ��������
		//float degreeAngle = FMath::RadiansToDegrees(radianAngle);
		//// ���࿡ �÷��̾ �þఢ(60)�� ���Դٸ� (degreeAngle <= 60 / 2) 
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
	//�÷��̾� ������ ������.
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
	
	//Ÿ���� ��ġ�� ������.
	FVector playerPos = target->GetActorLocation();
	posCheck = playerPos - bossActor->GetActorLocation();
	posCheck.Normalize();

	FVector p0 = bossActor->GetActorLocation();
	FVector vt = posCheck * chargeSpeed * GetWorld()->DeltaTimeSeconds;
	FVector p = p0 + vt;
	//myActor->AddMovementInput(p);
	//myActor->SetActorLocation(p);
	
	//UE_LOG(LogTemp, Warning, TEXT("%s"), p.Length());
	
	//�÷��̾� ��ġ���� ������ġ�� ������ ����
	FVector pp0 = playerPos - bossActor->GetActorLocation();
	//���Ⱚ�� �븻����������.
	pp0.Normalize();
	//������ ������ ���Ϳ��� �÷��̾� �������� ���� �����̰� ����, �ӵ�, �ð�
	FVector pp1 = pp0 * chargeSpeed * GetWorld()->GetDeltaSeconds();
	//�� ��ġ�� p1�� ������.
	FVector pp2 = bossActor->GetActorLocation() + pp1;
	//����ġ�� p2�� �̵���Ű��.
	bossActor->SetActorLocation(pp2);

	//FVector playerPos = target->GetActorLocation();
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

	//UE_LOG(LogTemp, Warning, TEXT(" charge skill Active"));

}

void UBossFsmTest::ThorwSkill()
{	
	// �Ѿ˰���(BP_Bullet) ���� �Ѿ��� �ϳ� �����Ѵ�.
	AStoneActor* spawnStone = GetWorld()->SpawnActor<AStoneActor>(bossActor->stoneFactory);
	// ������ �Ѿ��� ���� ��ġ�� ���´�.
	spawnStone->SetActorLocation(bossActor->GetActorLocation());
	spawnStone->isThorw = true;
	
	spawnStone->meshComp->OnComponentBeginOverlap.AddDynamic(this, &UBossFsmTest::ThrowSkillOverlap);

	//UE_LOG(LogTemp, Warning, TEXT("bossfsm Throw Skill"));
}

void UBossFsmTest::SmashSkill()
{
	USphereComponent* sr = bossActor->smashRange;

	//�������� ȿ�� ���
	//UParticleSystemComponent
	// myActor->smashRange->GetRelativeScale3D() �Ǵ� FVector(10)
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
	//�÷��̾� �ｺ �����ͼ� ������ �־��ֱ�
	target->statment.healthPoint -= damage;

}

