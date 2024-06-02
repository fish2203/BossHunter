// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossDropItem.generated.h"

UCLASS()
class BOSSHUNTER_API ABossDropItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossDropItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	bool iscreate = false;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* clearChest;

	UPROPERTY(EditAnywhere)
	class UPointLightComponent* pointLight;

	UPROPERTY(EditAnywhere)
	TArray <class UPointLightComponent*> pointLightArray;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* compItemNiagara;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* compBox;

	UPROPERTY()
	class ATest_Boss* bossActor;


	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* abc, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void LobbyURLTravel();
	UFUNCTION(Server, Reliable)
	void ServerRPC_LobbyURLTravel();
	

};
