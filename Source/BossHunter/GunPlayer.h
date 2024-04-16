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

	virtual void Q_Action() override;
	virtual void E_Action() override;
	void LazerShot(FHitResult hitInfo, FVector startPosition, FCollisionQueryParams params, FTimerHandle delayHandle);
	virtual void R_Action() override;
	void R_End_Action();
	void CallBomb(FTimerHandle delayHandle);
	virtual void F_Action() override;
	void ThrowBomb(FTimerHandle delayHandle);
	virtual void Ev_Action() override;
	virtual void Normal_Action() override;

public:
	UPROPERTY(EditAnywhere)
	float normalLength = 20000;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* particleSystem;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* animMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class AGunPlayer_Bomb> bombFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class AGunPlayer_CallBomb> callBombFactory;

	bool bIsSlowGun = false;
	FVector endPosition;

	float timeLimit = 10;
	float speedyTime = 10;
	float attackCount = 0;
	float lazerTime = 0;
	bool bIsPasiveActive = false;

	float cool_Q = 2;
	float curr_Q;
	float cool_E = 10;
	float curr_E;
	float cool_F = 15;
	float curr_F;
	float cool_R = 60;
	float curr_R;
	void CoolTimePointChange();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDie = false;

	TSubclassOf<UUserWidget> gunPlayerWidgetInstance;
	class UGunPlayerWidget* gunPlayerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* positionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* bombWidget;

public:
	UFUNCTION()
	void AttackBoss(float damagePercent);
};
