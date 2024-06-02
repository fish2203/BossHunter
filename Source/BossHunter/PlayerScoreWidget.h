// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOSSHUNTER_API UPlayerScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* text_PlayerScore;

	UFUNCTION()
	void UpdateCurrScoreUI(int32 score);

	UFUNCTION()
	void UpdateBestScoreUI(int32 bestScore);
	
};
