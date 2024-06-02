// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOSSHUNTER_API ULoadingWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	int32 count;
	class ABossRoomGameStateBase* gamestate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsReady = false;
};
