// Fill out your copyright notice in the Description page of Project Settings.


#include "BossRoomGameStateBase.h"

ABossRoomGameStateBase* ABossRoomGameStateBase::singleton = nullptr;

ABossRoomGameStateBase::ABossRoomGameStateBase()
{
	ConstructorHelpers::FObjectFinder<UDataTable> tempTable(TEXT("/Script/Engine.DataTable'/Game/BulePrint/ItemDataTable.ItemDataTable'"));
	if (tempTable.Succeeded())
	{
		itemDataTable = tempTable.Object;
	}

	singleton = nullptr;
}

ABossRoomGameStateBase* ABossRoomGameStateBase::Get()
{
	if(singleton == nullptr)
		singleton = CastChecked<ABossRoomGameStateBase>(GEngine->GameSingleton);
	
	return singleton;
}
