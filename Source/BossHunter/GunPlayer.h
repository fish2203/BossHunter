// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossHunterCharacter.h"
#include "GunPlayer.generated.h"

/**
 * 
 */
UCLASS()
class BOSSHUNTER_API AGunPlayer : public ABossHunterCharacter
{
	GENERATED_BODY()
	
public:
	AGunPlayer();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Ability_Q_Action() override;
	virtual void Ability_E_Action() override;
	virtual void Ability_R_Action() override;
	virtual void Ability_R_End_Action() override;
	virtual void Ability_F_Action() override;
	virtual void Ability_Ev_Action() override;
	virtual void Ability_Normal_Action() override;

	// skill Q - change weapon
	UFUNCTION(Server, Reliable)
	void ServerRPC_Q_Action();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Q_Action();
	virtual void Q_Action() override;

	// skill E - lazer
	UFUNCTION(Server, Reliable)
	void ServerRPC_E_Action(FVector endLocation);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_E_Action(FVector endLocation);
	virtual void E_Action() override;
	void LazerShot(FHitResult hitInfo, FVector startPosition, FCollisionQueryParams params, FTimerHandle delayHandle);

	// skill R - Big Bomb
	virtual void R_Action() override;
	UFUNCTION(Server, Reliable)
	void ServerRPC_R_End_Action(FVector endLocation);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_R_End_Action(FVector endLocation);
	void R_End_Action();
	void CallBomb(FTimerHandle delayHandle, FVector bombLocation);

	// skill F - bomb
	UFUNCTION(Server, Reliable)
	void ServerRPC_F_Action(FVector endLocation);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_F_Action(FVector endLocation);
	virtual void F_Action() override;
	void ThrowBomb(FTimerHandle delayHandle);
	virtual void Ev_Action() override;

	// skill normal - gun
	UFUNCTION(Server, Reliable)
	void ServerRPC_Normal_Action(FVector endLocation);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Normal_Action(FVector endLocation);
	virtual void Normal_Action() override;

public:
	// skill distance
	UPROPERTY(EditAnywhere)
	float normalLength = 20000;

	// normal skill particle
	UPROPERTY(EditAnywhere)
	class UParticleSystem* normalParticleSystem;

	// Lazer Start
	UPROPERTY(EditAnywhere)
	class UParticleSystem* lazerStartParticleSystem;

	// Lazer End
	UPROPERTY(EditAnywhere)
	class UParticleSystem* lazerEndParticleSystem;

	// player montage
	UPROPERTY(EditAnywhere)
	class UAnimMontage* animMontage;

	// if now attacking, can't use other skill - check boolean
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttack;

	// skill E bomb factory
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class AGunPlayer_Bomb> bombFactory;

	// skill R Big Bomb factory
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class AGunPlayer_CallBomb> callBombFactory;

	// check changed gun
	bool bIsSlowGun = false;

	// check mouse position
	UPROPERTY(EditAnywhere)
	FVector endPosition;

	// check mouse position hit information
	FHitResult endInfo;

	// pasive skill cool time
	UPROPERTY(Replicated)
	float timeLimit = 10;

	// pasive skill current time
	float speedyTime = 10;

	// pasive check count
	float attackCount = 0;

	// pasive check boolean
	UPROPERTY(Replicated)
	bool bIsPasiveActive = false;

	// lazer delay count time
	UPROPERTY(Replicated)
	float lazerTime = 0;

	// skill cool time
	float cool_Q = 0; // 2
	float cool_E = 0; // 10
	float cool_F = 0; // 15
	float cool_R = 0; // 60

	// skill current time
	float curr_Q;
	float curr_E;
	float curr_F;
	float curr_R;

	// if buy item, change cooltime function
	virtual void CoolTimePointChange() override;

	// if die, true
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDie = false;

	// chase mouse point arrow
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* positionComp;

	// if R down, turn on widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* bombWidget;

public:
	// Attacking boss
	UFUNCTION(Server, Reliable)
	void ServerRPC_AttackBoss(float damagePercent);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_AttackBoss(float damagePercent);
	void AttackBoss(float damagePercent);


};
