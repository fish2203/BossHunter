// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MoveBossZoneWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOSSHUNTER_API UMoveBossZoneWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_MoveBossZone;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Cancel;

public:
	UPROPERTY()
	class AGateBattleBoss* gatebattleBoss;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UMoveBossZoneWidget> movebosszoneWidget;*/

public:
	UFUNCTION()
	void OnClickMoveBossZone();
	UFUNCTION()
	void OnClickCancel();


	
};
