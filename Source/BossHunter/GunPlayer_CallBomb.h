// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunPlayer_CallBomb.generated.h"

UCLASS()
class BOSSHUNTER_API AGunPlayer_CallBomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunPlayer_CallBomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* floorComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* particle;

	class AGunPlayer* player;
	class ATest_Boss* monster;
	float time = 0;
};
