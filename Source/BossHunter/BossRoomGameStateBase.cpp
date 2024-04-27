// Fill out your copyright notice in the Description page of Project Settings.


#include "BossRoomGameStateBase.h"

ABossRoomGameStateBase::ABossRoomGameStateBase()
{
	ConstructorHelpers::FObjectFinder<UDataTable> tempTable(TEXT("/Script/Engine.DataTable'/Game/BulePrint/ItemDataTable.ItemDataTable'"));
	if (tempTable.Succeeded())
	{
		itemDataTable = tempTable.Object;
	}
}
