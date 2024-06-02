// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LobbyStoreActor.generated.h"

UCLASS()
class BOSSHUNTER_API ALobbyStoreActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALobbyStoreActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY()
	class USceneComponent* sceneComp;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* staticMesh;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ULobbyStoreWidget> lobbyStoreWidgetPackage;
	UPROPERTY()
	class ULobbyStoreWidget* lobbyStoreWidget;
	UPROPERTY()
	class USoundBase* lobbyMusic;
	
	UPROPERTY()
	class AGunPlayer* gunPlayer;
	UPROPERTY()
	TArray<class AGunPlayer*> gunPlayers;
public:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* abc, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void PlayLobbyMusic();


};
