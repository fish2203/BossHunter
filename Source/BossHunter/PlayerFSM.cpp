// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFSM.h"

// Sets default values for this component's properties
UPlayerFSM::UPlayerFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	
}


// Called when the game starts
void UPlayerFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	playerSkillState = playerSkillSet::Idle;
}


// Called every frame
void UPlayerFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//UE_LOG(LogTemp, Warning, TEXT("player fsm tick component %d"), playerSkillState);
}

playerSkillSet UPlayerFSM::GetPlayerSkillState()
{
	return playerSkillState;
}

void UPlayerFSM::SetPlayerSkillState(playerSkillSet skillState)
{
	this->playerSkillState = skillState;
}

