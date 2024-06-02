// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GateBattleBoss.generated.h"

UCLASS()
class BOSSHUNTER_API AGateBattleBoss : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGateBattleBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY()
	class AGunPlayer* target; 

	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* abc, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void URLTravel();
	UFUNCTION(Server, Reliable)
	void ServerRPC_URLTravel();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_URLTravel();

	class UNetGameInstance* gi;

	//#include "Interfaces/OnlineSessionInterface.h"
	//void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result);

	//보스게이트에 위젯을 담아놓자.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMoveBossZoneWidget> moveWidgetPackage;
	UPROPERTY()
	class UMoveBossZoneWidget* movewidgetPointer;

	//다시하기 위젯을 담아놓자.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class URestartBossZone> restartbosszonePackage;
	UPROPERTY()
	class URestartBossZone* restartBossZone;
	//보스존을 다시시작하자.
	UFUNCTION()
	void viewRestartWidget();

	class ABossRoomGameStateBase* gamestate;

};
