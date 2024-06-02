// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MoveLoobyWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOSSHUNTER_API UMoveLoobyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_MoveLooby;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Cancel;

public:
	UPROPERTY()
	class ABossDropItem* bossdropItem;

public:
	UFUNCTION()
	void OnClickMoveLobby();
	UFUNCTION()
	void OnClickCancel();

	
};
