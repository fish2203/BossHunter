// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StoneActor.generated.h"

DECLARE_DELEGATE_OneParam(FInsertMagazine, AStoneActor*);

UCLASS()
class BOSSHUNTER_API AStoneActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStoneActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere)
	class AGunPlayer* target;
	
	UPROPERTY()
	class ATest_Boss* bossActor;

	FVector dir;

	UPROPERTY(EditAnywhere)
	float moveSpeed = 400;

	UPROPERTY()
	class USoundBase* exploSound;

	UPROPERTY()
	class UParticleSystem* exploEffect;

public:
	bool isThorw = false;
	bool isstoneHit = false;
	float currTime = 0.0f;


public:
	UFUNCTION()
	void StoneSpawn();
	UFUNCTION()
	void StoneFire();
	UFUNCTION()
	void StoneMove();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ShowStoneEffect();


	UFUNCTION()
	void SetAcitve(bool isActive);

	//���� ��ã���� ��� �Լ��� ���������� Ȯ���غ���.
	void ThrowSkillLinetrace();
	
	//Delegate �� ��ϵǴ� �Լ��� UFUNCTION �� �� ������!
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* abc, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FInsertMagazine insertMagazineDel;

};
