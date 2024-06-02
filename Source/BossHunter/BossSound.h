// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossSound.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOSSHUNTER_API UBossSound : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossSound();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	UPROPERTY()
	class USoundBase* bossBattleStartSound;
	UPROPERTY()
	class USoundBase* bossBattleStartMusic;
	UPROPERTY()
	class USoundBase* bossWalkSound;
	UPROPERTY()
	class USoundBase* bossPunchSound;
	UPROPERTY()
	class USoundBase* bossPunchVoice;
	UPROPERTY()
	class USoundBase* bossChargeSound;
	UPROPERTY()
	class USoundBase* bossChargeVoice;
	
	
	UPROPERTY()
	class USoundBase* bossStoneThrowSound;
	UPROPERTY()
	class USoundBase* bossStoneThrowVoiece;

	UPROPERTY()
	class USoundBase* bossSmashSound;
	UPROPERTY()
	class USoundBase* bossSmashVoice;

	UPROPERTY()
	class USoundBase* bossDelayVoice;

	UPROPERTY()
	class USoundBase* bossDieVoice;

	void playbossBattleStartSound();
	void playbossBattleStartMusic();
	
	void playbossWalkSound();
	void PlaybossPunchSound();
	void PlaybossPunchVoice();

	void playbossStoneThrowSound();
	void playbossStoneThrowVoice();
	
	void playbossChargeSound();
	void playbossChargeVoice();
	
	void playbossSmashSound();
	void playbossSmashVoice();
	
	void playbossDelayVoice();
	void playbossDieVoice();
	

};
