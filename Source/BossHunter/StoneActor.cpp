// Fill out your copyright notice in the Description page of Project Settings.


#include "StoneActor.h"
#include <Components/BoxComponent.h>
#include "TestPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "GunPlayer.h"
#include "Test_Boss.h"
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>
#include "Boss/BossFsmTest.h"
#include "BossSound.h"

// Sets default values
AStoneActor::AStoneActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box comp"));
	SetRootComponent(boxComp);
	//boxComp->SetupAttachment(RootComponent);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	meshComp->SetupAttachment(RootComponent);

	// �׽�Ʈ ���� ȿ�� ��������
	//ConstructorHelpers::FObjectFinder<UParticleSystem> tempEffect(TEXT("/Script/Engine.ParticleSystem'/Game/BulePrint/Cheezebee/Test_Particle.Test_Particle'"));

	//���� ȿ�� ���̽��� ��������
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempEffect(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonRampage/FX/Particles/Rampage_v001_IceBlue/FX/P_Rampage_Ice_Enraged_Impact.P_Rampage_Ice_Enraged_Impact'"));

	//���� ȿ���� ����
	if (tempEffect.Succeeded())
	{
		exploEffect = tempEffect.Object;
	}

	meshComp->SetWorldScale3D(FVector(0.5));

}

// Called when the game starts or when spawned
void AStoneActor::BeginPlay()
{
	Super::BeginPlay();


	// ������ �����ϸ� �÷��̾ ã��, UGameplayStatics�� ���� �����Ҷ��� ��밡��
	//UGameplayStatics ��Ŭ���
	//ATestPlayer ��Ŭ���
	//AGunPlayer ��Ŭ���

	////���ӿ��� �÷��̸� ã�ƶ�.
	//AActor* findPlayer = UGameplayStatics::GetActorOfClass(GetWorld(), AGunPlayer::StaticClass());

	//target = Cast<AGunPlayer>(findPlayer);

	//���ӿ��� ������ ã�ƶ�.
	AActor* findBoss = UGameplayStatics::GetActorOfClass(GetWorld(), ATest_Boss::StaticClass());
	bossActor = Cast<ATest_Boss>(findBoss);

	//�Ž��� �Ⱥ��̰� ����.
	meshComp->SetActive(false);
	//UE_LOG(LogTemp, Warning, TEXT("stoneactor beginplay mesh false "));
	
	//���� ���� ������Ʈ
	UActorComponent* findSoundComp = bossActor->FindComponentByClass<UBossSound>();
	bossSound = Cast<UBossSound>(findSoundComp);

}

// Called every frame
void AStoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isThorw == true)
	{
		StoneMove();
		
	}

}

void AStoneActor::StoneSpawn()
{
	// �Ѿ� ����
	/*AStoneActor* stoneBullet = GetWorld()->SpawnActor<AStoneActor>(bossActor->stoneFactory, bossActor->GetActorLocation(), FRotator::ZeroRotator);*/
	GetWorld()->SpawnActor<AStoneActor>(bossActor->stoneFactory, bossActor->GetActorLocation(), FRotator::ZeroRotator);
	//UBossSound
	
}

void AStoneActor::StoneFire()
{
	FVector bossPos = bossActor->GetActorLocation();
	// Overlap �浹�� �Ǿ��� �� ȣ��Ǵ� �Լ� ��� (Delegate = �Լ��� ������ �ִ� ����)
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AStoneActor::OnOverlap);

}


void AStoneActor::StoneMove()
{
	if (this != nullptr)
	{
		MultiRPC_StoneSound();
	}
	//UStaticMeshComponent
	meshComp->SetActive(true);
	meshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	if (target == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT(" target is null"));

		return;
	}
	//�÷��̾��� ��ġ�� ������.
	FVector playerPos = target->GetActorLocation();

	//���� ��ġ�� ������.
	FVector bossPos = bossActor->GetActorLocation();
	//�� ��ġ�� ����.
	dir = playerPos - GetActorLocation();
	dir.Normalize();
	int32 randStone = bossActor->fsm->randSkill;
	FRotator moveRotator = FRotator(1);

	if (randStone == 0)
	{
		// �÷��̾ �����ϴ� ������
		//����ġ+ ���Ⱚ(Ÿ�� ���� * �̵��ӵ� * �ð���)
		FVector p0 = GetActorLocation();
		FVector vt = dir * moveSpeed * GetWorld()->GetDeltaSeconds();
		FVector p = p0 + vt;
		SetActorLocation(p);

		
		FRotator r0 = GetActorRotation();
		FRotator r1 = GetActorRotation() + FRotator(moveRotator) ;
		FRotator r2= r0+ r1;
		SetActorRotation(r2);

		//�α׸� ����.
		//UE_LOG(LogTemp, Warning, TEXT("stone actor StoneFire chase"));
		//����� ���� �׸���
		//DrawDebugLine(GetWorld(), bossPos, playerPos, FColor::Red);
		ThrowSkillLinetrace();

	}

	else
	{
		//�÷��̾� �������� �����ϴ� ������
		//�÷��̾� ��ġ���� ������ġ�� ������ ����
		FVector p0 = bossActor->GetActorForwardVector();
		//���Ⱚ�� �븻����������.
		p0.Normalize();
		//������ ������ ���Ϳ��� �÷��̾� �������� ���� �����̰� ����, �ӵ�, �ð�
		FVector p1 = p0 * moveSpeed * GetWorld()->GetDeltaSeconds();
		//�� ��ġ�� p1�� ������.
		FVector p2 = GetActorLocation() + p1;
		//����ġ�� p2�� �̵���Ű��.
		SetActorLocation(p2);

		
		FRotator r0 = GetActorRotation();
		FRotator r1 = GetActorRotation() + FRotator(moveRotator);
		FRotator r2 = r0 + r1;
		SetActorRotation(r2);


		//����ϱ�
		//UE_LOG(LogTemp, Warning, TEXT("stone actor StoneFire foward"));

		//����� ���� �׸���
		//DrawDebugLine(GetWorld(), bossPos, playerPos, FColor::Red);
		ThrowSkillLinetrace();

	}
	
	//�ð�����
	currTime += GetWorld()->DeltaTimeSeconds;
	//2�������� ���� �ı�
	if (currTime > 2)
	{
		//UE_LOG(LogTemp, Warning, TEXT(" StoneMove 2 seconds"));
		currTime = 0;
		isThorw = false;
		
		MultiRPC_ShowStoneEffect();

		Destroy();
		UE_LOG(LogTemp, Warning, TEXT(" 22222222222"));
	}
		
}


void AStoneActor::MultiRPC_ShowStoneEffect_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), exploEffect, GetActorLocation(), GetActorRotation());

}


void AStoneActor::SetAcitve(bool isActive)
{
	// ���࿡ isActive �� true ��
	if (isActive == true)
	{
		// ���(mesh) �� ���̰� ����
		meshComp->SetVisibility(isActive);
		// �浹 �� �� �ְ� ����
		meshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	// �׷��� ������
	else
	{
		// ���(mesh) �� �Ⱥ��̰� ����
		meshComp->SetVisibility(isActive);
		// �浹 ���� �ʰ� ����
		meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}

}

void AStoneActor::ThrowSkillLinetrace()
{
	
	//ServerRPC_ThrowSkillLinetrace();
	//AActor* findActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGunPlayer::StaticClass());

	UActorComponent* findFSM = bossActor->fsm;

	//UE_LOG(LogTemp, Warning, TEXT("boss fsm SkillLinetrace "));
	//�浹��� ����
	FHitResult hitInfo;
	//������
	FVector startPos = this->GetActorLocation();
	//����
	FVector endPos = this->GetActorLocation() + this->GetActorForwardVector() * 100;
	FCollisionQueryParams params;
	params.AddIgnoredActor(bossActor);

	bool isHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECollisionChannel::ECC_Visibility, params);

	if (isHit)
	{
		// ���࿡ ���� Player ���
		//hitInfo.GetActor()->GetName().Contains(TEXT("player"));
		//AActor* hitActor = Cast<AGunPlayer>(hitInfo.GetActor());
		AActor* hitActor = hitInfo.GetActor();
		//��Ʈ �����̸��� �÷��̾ �ִٸ�
		if (hitActor != nullptr)
		{
			if (hitActor->GetName().Contains(TEXT("player")))
			{
				// ������ ����
				bossActor->fsm->PlayerDamageProcess(50);
				bossActor->fsm->ChangeState(EEnemyState::ATTACK_DELAY);
				
				//ȿ���� ��������
				MultiRPC_ShowStoneEffect();
				
				Destroy();

			}

		}

		//DrawDebugLine(GetWorld(), startPos, startPos + endPos, FColor::Red);
		
	}
}

void AStoneActor::MultiRPC_StoneSound_Implementation()
{
	if(bossSound == nullptr)return;
	bossSound->playbossStoneThrowSound();
}

void AStoneActor::OnOverlap(UPrimitiveComponent* abc, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
	{

		if (OtherActor->GetName().Contains(TEXT("player")))
		{
			FVector bossPos = bossActor->GetActorLocation();
			SetActorLocation(bossPos);

			//ȿ���� ��������
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), exploEffect, GetActorLocation(), GetActorRotation());

			isstoneHit = false;

			//Destroy();



			/*UParticleSystemComponent* stoneEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), exploEffect, GetActorLocation(), GetActorRotation());

			AActor* OwnerActor = stoneEffect->GetOwner();
			*/
			UE_LOG(LogTemp, Warning, TEXT("stone actor, OnOverlap stoneEffect"));

			/*currTime += GetWorld()->GetDeltaSeconds();

			if (currTime > 2)
			{
				currTime = 0;
				OwnerActor->Destroy();
			}*/

		}
		else
		{
			isstoneHit = false;
		}
	}


