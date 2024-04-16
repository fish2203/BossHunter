// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerFSM.generated.h"

UENUM(BlueprintType)
enum class playerSkillSet : uint8
{
	Idle,
	Plain,
	Q_Skill,
	E_Skill,
	R_Skill,
	F_Skill,
	Evasion,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOSSHUNTER_API UPlayerFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	playerSkillSet playerSkillState = playerSkillSet::Idle;
	UFUNCTION(BlueprintCallable)
	playerSkillSet GetPlayerSkillState();
	UFUNCTION(BlueprintCallable)
	void SetPlayerSkillState(playerSkillSet skillState);
		
};
