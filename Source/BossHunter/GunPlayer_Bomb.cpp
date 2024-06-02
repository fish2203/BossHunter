// Fill out your copyright notice in the Description page of Project Settings.


#include "GunPlayer_Bomb.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/PrimitiveComponent.h>
#include "GeometryCollection/GeometryCollectionComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h>
#include "GunPlayer.h"
#include <../../../../../../../Source/Runtime/Engine/Public/KismetTraceUtils.h>

// Sets default values
AGunPlayer_Bomb::AGunPlayer_Bomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compBomb = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bomb"));
	RootComponent = compBomb;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	ProjectileMovementComponent->InitialSpeed = 3000;
	ProjectileMovementComponent->MaxSpeed = 3000;
	/** �ӵ��� ���� �����̼� ��ȭ */
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	/** �ٿ X */
	ProjectileMovementComponent->bShouldBounce = false;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AGunPlayer_Bomb::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("boss"), ActorsWithTag);
	for (AActor* Actor : ActorsWithTag)
	{
		if (Actor)
		{
			monster = Cast<ATest_Boss>(Actor);
		}
	}
}

// Called every frame
void AGunPlayer_Bomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult hitInfo;
	FCollisionQueryParams param;
	param.AddIgnoredActor(this);
	param.AddIgnoredActor(GetOwner());
	FVector startPosition = GetActorLocation();
	FVector EndPosition = GetActorLocation() + GetActorForwardVector();

	bool bIsHit = GetWorld()->SweepSingleByChannel(hitInfo, startPosition, EndPosition, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(50), param);

	if (bIsHit)
	{
		bounceCount++;
		if (hitInfo.GetActor())
		{
			if (hitInfo.GetActor()->ActorHasTag("boss") || bounceCount >= 3) // ���� ������
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particleSystem, GetActorLocation());
				AGunPlayer* player = Cast<AGunPlayer>(GetOwner());
				player->ServerRPC_AttackBoss(3);
			}
			else // ���� ���н�
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), failParticleSystem, GetActorLocation());
			}
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), F_Active_Sound, GetActorLocation());
			Destroy();
		}
	}
}

void AGunPlayer_Bomb::Fire(FVector direction)
{
	ProjectileMovementComponent->Velocity = direction * ProjectileMovementComponent->InitialSpeed;
}

