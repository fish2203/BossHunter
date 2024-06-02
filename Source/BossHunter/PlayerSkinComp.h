// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerSkinComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOSSHUNTER_API UPlayerSkinComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerSkinComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

//OSH가 추가함
public:
	UPROPERTY()
	class ULobbyStoreWidget* lobbyStoreWidget;
	UPROPERTY()
	TArray<class USkeletalMesh*> compMesh;
	UPROPERTY()
	class AGunPlayer* player; 

public:
	bool isnormalSkin = true;
	bool isamorSkin = false;
	int32 haveAmorSkin = 1;
	int32 PlayerSkinIdx = 0;
	TArray <class USavePlayerScore*> valuePlayerSkinCompArray;
	
public:
	void ChangePlayerMeshNormal();
	void ChangePlayerMeshAmor();
	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangePlayerSkin(int32 idx);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ChangePlayerSkin(int32 idx);

	void SaveData();
	void LoadData();
	UFUNCTION(Server, Reliable)
	void ServerRPC_LoadData();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_LoadData();

		
};
