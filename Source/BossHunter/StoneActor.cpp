// Fill out your copyright notice in the Description page of Project Settings.


#include "StoneActor.h"
#include <Components/BoxComponent.h>
#include "TestPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "GunPlayer.h"
#include "Test_Boss.h"
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>

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
	
}

// Called when the game starts or when spawned
void AStoneActor::BeginPlay()
{
	Super::BeginPlay();


	// ������ �����ϸ� �÷��̾ ã��, UGameplayStatics�� ���� �����Ҷ��� ��밡��
	//UGameplayStatics ��Ŭ���
	//ATestPlayer ��Ŭ���
	//AGunPlayer ��Ŭ���
	
	//���ӿ��� �÷��̸� ã�ƶ�.
	AActor* findPlayer = UGameplayStatics::GetActorOfClass(GetWorld(), AGunPlayer::StaticClass());
	target = Cast<AGunPlayer>(findPlayer);

	//���ӿ��� ������ ã�ƶ�.
	AActor* findBoss = UGameplayStatics::GetActorOfClass(GetWorld(), ATest_Boss::StaticClass());
	bossActor = Cast<ATest_Boss>(findBoss);
	
	//�Ž��� �Ⱥ��̰� ����.
	meshComp->SetActive(false);

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
}

void AStoneActor::StoneFire()
{	
	FVector bossPos = bossActor->GetActorLocation();
	// Overlap �浹�� �Ǿ��� �� ȣ��Ǵ� �Լ� ��� (Delegate = �Լ��� ������ �ִ� ����)
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AStoneActor::OnOverlap);

}


void AStoneActor::StoneMove()
{
	meshComp->SetActive(true);
	meshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	//�÷��̾��� ��ġ�� ������.
	FVector playerPos = target->GetActorLocation();
	//���� ��ġ�� ������.
	FVector bossPos = bossActor->GetActorLocation();
	//�� ��ġ�� ����.
	dir = playerPos - GetActorLocation();
	dir.Normalize();

	// �÷��̾ �����ϴ� ������
	//����ġ+ ���Ⱚ(Ÿ�� ���� * �̵��ӵ� * �ð���)
	/*FVector p0 = GetActorLocation();
	FVector vt = dir * moveSpeed * GetWorld()->GetDeltaSeconds();
	FVector p = p0 + vt;
	SetActorLocation(p);*/

	//�α׸� ����.
	//UE_LOG(LogTemp, Warning, TEXT("stone actor StoneFire chase"));

	//�÷��̾� �������� �����ϴ� ������
	
	//�÷��̾� ��ġ���� ������ġ�� ������ ����
	FVector p0 = playerPos - bossPos;
	//���Ⱚ�� �븻����������.
	p0.Normalize();
	//������ ������ ���Ϳ��� �÷��̾� �������� ���� �����̰� ����, �ӵ�, �ð�
	FVector p1 = p0 * moveSpeed * GetWorld()->GetDeltaSeconds();
	//�� ��ġ�� p1�� ������.
	FVector p2 = GetActorLocation() + p1;
	//����ġ�� p2�� �̵���Ű��.
	SetActorLocation(p2);

	//����ϱ�
	//UE_LOG(LogTemp, Warning, TEXT("stone actor StoneFire foward"));

	//����� ���� �׸���
	DrawDebugLine(GetWorld(), bossPos, playerPos, FColor::Red);

	//�ð�����
	currTime += GetWorld()->GetDeltaSeconds();
	//2�������� ���� �ı�
	if (currTime > 2)
	{
		currTime = 0;
		isThorw = false;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), exploEffect, GetActorLocation(), GetActorRotation());
		Destroy();
		
	}


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


