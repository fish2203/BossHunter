// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GateMoveLobby.generated.h"

UCLASS()
class BOSSHUNTER_API AGateMoveLobby : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGateMoveLobby();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* compBox;

	UPROPERTY()
	class UMoveLoobyWidget* movelobbyWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMoveLoobyWidget> movelobbyWidgetPackage;

public:

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* abc, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
