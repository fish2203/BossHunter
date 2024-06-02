// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	
	// Item Function
	UFUNCTION()
	void OnClick_ItemButton();
	UFUNCTION()
	void OnClick_Item1();
	UFUNCTION()
	void OnClick_Item2();
	UFUNCTION()
	void OnClick_Item4();
	UFUNCTION()
	void OnClick_Item5();
	UFUNCTION()
	void OnClick_Item6();
	UFUNCTION()
	void OnClick_Item7();
	UFUNCTION()
	void OnClick_Item8();

	// Active Item Function
	UFUNCTION()
	void OnClick_ActiveItemButton();
	UFUNCTION()
	void OnClick_Item3();
	UFUNCTION()
	void OnClick_Item9();
	UFUNCTION()
	void OnClick_Item10();
	UFUNCTION()
	void OnClick_Item11();

	// Passive Item Function
	UFUNCTION()
	void OnClick_PassiveItemButton();
	UFUNCTION()
	void OnClick_PassiveItem1();
	UFUNCTION()
	void OnClick_PassiveItem2();
	UFUNCTION()
	void OnClick_PassiveItem3();
	UFUNCTION()
	void OnClick_PassiveItem4();



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

	// Item Panel
	UPROPERTY(meta=(BindWidget))
	class UButton* ItemButton;
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* ItemPannel;

	// Item
	UPROPERTY(meta=(BindWidget))
	class UButton* Item1;
	UPROPERTY(meta=(BindWidget))
	class UImage* Item1_Img;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Item1_Text;

	UPROPERTY(meta=(BindWidget))
	class UButton* Item2;
	UPROPERTY(meta=(BindWidget))
	class UImage* Item2_Img;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Item2_Text;

	UPROPERTY(meta=(BindWidget))
	class UButton* Item3;
	UPROPERTY(meta=(BindWidget))
	class UImage* Item3_Img;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Item3_Text;

	UPROPERTY(meta=(BindWidget))
	class UButton* Item4;
	UPROPERTY(meta=(BindWidget))
	class UImage* Item4_Img;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Item4_Text;

	UPROPERTY(meta=(BindWidget))
	class UButton* Item5;
	UPROPERTY(meta=(BindWidget))
	class UImage* Item5_Img;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Item5_Text;

	UPROPERTY(meta=(BindWidget))
	class UButton* Item6;
	UPROPERTY(meta=(BindWidget))
	class UImage* Item6_Img;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Item6_Text;

	UPROPERTY(meta=(BindWidget))
	class UButton* Item7;
	UPROPERTY(meta=(BindWidget))
	class UImage* Item7_Img;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Item7_Text;

	// Active Item Panel
	UPROPERTY(meta=(BindWidget))
	class UButton* ActiveItemButton;
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* ActiveItemPannel;

	// Active Item
	UPROPERTY(meta=(BindWidget))
	class UButton* ActiveItem1;
	UPROPERTY(meta=(BindWidget))
	class UImage* ActiveItem1_Img;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ActiveItem1_Text;

	UPROPERTY(meta=(BindWidget))
	class UButton* ActiveItem2;
	UPROPERTY(meta=(BindWidget))
	class UImage* ActiveItem2_Img;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ActiveItem2_Text;

	UPROPERTY(meta=(BindWidget))
	class UButton* ActiveItem3;
	UPROPERTY(meta=(BindWidget))
	class UImage* ActiveItem3_Img;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ActiveItem3_Text;

	UPROPERTY(meta=(BindWidget))
	class UButton* ActiveItem4;
	UPROPERTY(meta=(BindWidget))
	class UImage* ActiveItem4_Img;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ActiveItem4_Text;


	// Passive Item Pannel
	UPROPERTY(meta=(BindWidget))
	class UButton* PassiveButton;
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* PassiveItemPannel;


	// Passive Item
	UPROPERTY(meta=(BindWidget))
	class UButton* PassiveItem1;
	UPROPERTY(meta=(BindWidget))
	class UImage* PassiveItem1_Img;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* PassiveItem1_Text;

	UPROPERTY(meta=(BindWidget))
	class UButton* PassiveItem2;
	UPROPERTY(meta=(BindWidget))
	class UImage* PassiveItem2_Img;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* PassiveItem2_Text;

	UPROPERTY(meta=(BindWidget))
	class UButton* PassiveItem3;
	UPROPERTY(meta=(BindWidget))
	class UImage* PassiveItem3_Img;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* PassiveItem3_Text;

	UPROPERTY(meta=(BindWidget))
	class UButton* PassiveItem4;
	UPROPERTY(meta=(BindWidget))
	class UImage* PassiveItem4_Img;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* PassiveItem4_Text;

public:
	int32 itemindex;
	UPROPERTY()
	class ABossRoomGameStateBase* gamestate;
	UPROPERTY()
	class UDataTable* tempItemDataTable;
};
