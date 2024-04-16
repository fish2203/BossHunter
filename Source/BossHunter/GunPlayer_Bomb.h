// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunPlayer_Bomb.generated.h"

UCLASS()
class BOSSHUNTER_API AGunPlayer_Bomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunPlayer_Bomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* compBomb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* particleSystem;
	class ATest_Boss* monster;

	int32 bounceCount = 0;
	FVector dir;

	UFUNCTION()
	void Fire(FVector direction);
};
