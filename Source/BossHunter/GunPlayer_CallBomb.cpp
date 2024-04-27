// Fill out your copyright notice in the Description page of Project Settings.


#include "GunPlayer_CallBomb.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Particles/ParticleSystem.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "GunPlayer.h"
#include "Test_Boss.h"

// Sets default values
AGunPlayer_CallBomb::AGunPlayer_CallBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	floorComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("floorComp"));
	RootComponent = floorComp;

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("boss"), ActorsWithTag);
	for (AActor* Actor : ActorsWithTag)
	{
		if (Actor)
		{
			monster = Cast<ATest_Boss>(Actor);
		}
	}
	bReplicates = true;
}

// Called when the game starts or when spawned
void AGunPlayer_CallBomb::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particle, GetActorLocation());
	player = Cast<AGunPlayer>(GetOwner());
}

// Called every frame
void AGunPlayer_CallBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(player == nullptr)
		player = Cast<AGunPlayer>(GetOwner());

	if(IsOverlappingActor(monster))
		player->ServerRPC_AttackBoss(DeltaTime);

	time += DeltaTime;
	if (time >= 5.0f)
		Destroy();
}

