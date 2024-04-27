// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemList.h"
#include "StoreWidget.generated.h"

/**
 * 
 */

UCLASS()
class BOSSHUNTER_API UStoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void MenuButtonHovered();
	UFUNCTION()
	void OnClick_Item1();
	UFUNCTION()
	void OnClick_Item2();
	void ChangeImage();
	UFUNCTION()
	void OnClick_PurchaseItem();
	UFUNCTION()
	void OnClick_ExitButton();

public:
	class ABossHunterCharacter* player;
	
public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* HasGold;
	UPROPERTY(meta=(BindWidget))
	class UButton* ItemButton;
	UPROPERTY(meta=(BindWidget))
	class UButton* ActiveItemButton;
	UPROPERTY(meta=(BindWidget))
	class UButton* PassiveButton;
	UPROPERTY(meta=(BindWidget))
	class UButton* PurchaseButton;
	UPROPERTY(meta=(BindWidget))
	class UImage* SelectImage;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* SelectItemName;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* SelectItemExplain;
	UPROPERTY(meta=(BindWidget))
	class UButton* ExitButton;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

public:
	UPROPERTY(meta=(BindWidget))
	class UButton* Item1;
	UPROPERTY(meta=(BindWidget))
	class UButton* Item2;
	int32 itemindex;
	UPROPERTY()
	class ABossRoomGameStateBase* gamestate;
	UPROPERTY()
	class UDataTable* tempItemDataTable;
};
