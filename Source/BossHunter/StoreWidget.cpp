// Fill out your copyright notice in the Description page of Project Settings.


#include "StoreWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include "BossHunterCharacter.h"
#include "BossHunterGameMode.h"
#include "BossRoomGameStateBase.h"
#include "PlayerWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetSwitcher.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CanvasPanel.h>

void UStoreWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// item click function
	ItemButton->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_ItemButton);
	Item1->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_Item1);
	Item2->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_Item2);
	Item3->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_Item4);
	Item4->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_Item5);
	Item5->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_Item6);
	Item6->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_Item7);
	Item7->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_Item8);

	// active item function
	ActiveItemButton->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_ActiveItemButton);
	ActiveItem1->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_Item3);
	ActiveItem2->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_Item9);
	ActiveItem3->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_Item10);
	ActiveItem4->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_Item11);

	// Passive Item Function
	PassiveButton->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_PassiveItemButton);
	PassiveItem1->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_PassiveItem1);
	PassiveItem2->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_PassiveItem2);
	PassiveItem3->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_PassiveItem3);
	PassiveItem4->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_PassiveItem4);

	PurchaseButton->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_PurchaseItem);
	ExitButton->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_ExitButton);
	gamestate = ABossRoomGameStateBase::Get();

	player = Cast<ABossHunterCharacter>(GetOwningPlayer()->GetPawn());
	tempItemDataTable = gamestate->itemDataTable;

	// 이미지 셋팅
	Item1_Img->SetBrushFromTexture(tempItemDataTable->FindRow<FItem>(*FString::FromInt(1), TEXT(""))->image);
	Item2_Img->SetBrushFromTexture(tempItemDataTable->FindRow<FItem>(*FString::FromInt(2), TEXT(""))->image);
	Item3_Img->SetBrushFromTexture(tempItemDataTable->FindRow<FItem>(*FString::FromInt(4), TEXT(""))->image);
	Item4_Img->SetBrushFromTexture(tempItemDataTable->FindRow<FItem>(*FString::FromInt(5), TEXT(""))->image);
	Item5_Img->SetBrushFromTexture(tempItemDataTable->FindRow<FItem>(*FString::FromInt(6), TEXT(""))->image);
	Item6_Img->SetBrushFromTexture(tempItemDataTable->FindRow<FItem>(*FString::FromInt(7), TEXT(""))->image);
	Item7_Img->SetBrushFromTexture(tempItemDataTable->FindRow<FItem>(*FString::FromInt(8), TEXT(""))->image);

	ActiveItem1_Img->SetBrushFromTexture(tempItemDataTable->FindRow<FItem>(*FString::FromInt(3), TEXT(""))->image);
	ActiveItem2_Img->SetBrushFromTexture(tempItemDataTable->FindRow<FItem>(*FString::FromInt(9), TEXT(""))->image);
	ActiveItem3_Img->SetBrushFromTexture(tempItemDataTable->FindRow<FItem>(*FString::FromInt(10), TEXT(""))->image);
	ActiveItem4_Img->SetBrushFromTexture(tempItemDataTable->FindRow<FItem>(*FString::FromInt(11), TEXT(""))->image);


	// 가격 셋팅
	Item1_Text->SetText(FText::AsNumber(tempItemDataTable->FindRow<FItem>(*FString::FromInt(1), TEXT(""))->price));
	Item2_Text->SetText(FText::AsNumber(tempItemDataTable->FindRow<FItem>(*FString::FromInt(2), TEXT(""))->price));
	Item3_Text->SetText(FText::AsNumber(tempItemDataTable->FindRow<FItem>(*FString::FromInt(4), TEXT(""))->price));
	Item4_Text->SetText(FText::AsNumber(tempItemDataTable->FindRow<FItem>(*FString::FromInt(5), TEXT(""))->price));
	Item5_Text->SetText(FText::AsNumber(tempItemDataTable->FindRow<FItem>(*FString::FromInt(6), TEXT(""))->price));
	Item6_Text->SetText(FText::AsNumber(tempItemDataTable->FindRow<FItem>(*FString::FromInt(7), TEXT(""))->price));
	Item7_Text->SetText(FText::AsNumber(tempItemDataTable->FindRow<FItem>(*FString::FromInt(8), TEXT(""))->price));

	ActiveItem1_Text->SetText(FText::AsNumber(tempItemDataTable->FindRow<FItem>(*FString::FromInt(3), TEXT(""))->price));
	ActiveItem2_Text->SetText(FText::AsNumber(tempItemDataTable->FindRow<FItem>(*FString::FromInt(9), TEXT(""))->price));
	ActiveItem3_Text->SetText(FText::AsNumber(tempItemDataTable->FindRow<FItem>(*FString::FromInt(10), TEXT(""))->price));
	ActiveItem4_Text->SetText(FText::AsNumber(tempItemDataTable->FindRow<FItem>(*FString::FromInt(11), TEXT(""))->price));

	// 액티브 아이템은 처음에 안보이게
	ActiveItemPannel->SetVisibility(ESlateVisibility::Hidden);
	PassiveItemPannel->SetVisibility(ESlateVisibility::Hidden);
}

void UStoreWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (player) {
		HasGold->SetText(FText::FromString(FString::Printf(TEXT("보유 골드 : %d"), player->gold)));
	}

	MenuButtonHovered();
}

void UStoreWidget::MenuButtonHovered()
{
	if (ActiveItemButton->IsHovered())
	{
		ActiveItemButton->SetBackgroundColor(FLinearColor(0.7f, 0.6f, 0.08f, 0.4f));
		ItemButton->SetBackgroundColor(FLinearColor(0.7f, 0.6f, 0.08f, 0.0f));
		PassiveButton->SetBackgroundColor(FLinearColor(0.7f, 0.6f, 0.08f, 0.0f));

	}
	else if (PassiveButton->IsHovered())
	{
		PassiveButton->SetBackgroundColor(FLinearColor(0.7f, 0.6f, 0.08f, 0.4f));
		ItemButton->SetBackgroundColor(FLinearColor(0.7f, 0.6f, 0.08f, 0.0f));
		ActiveItemButton->SetBackgroundColor(FLinearColor(0.7f, 0.6f, 0.08f, 0.0f));


	}
	else if (ItemButton->IsHovered())
	{
		ItemButton->SetBackgroundColor(FLinearColor(0.7f, 0.6f, 0.08f, 0.4f));
		PassiveButton->SetBackgroundColor(FLinearColor(0.7f, 0.6f, 0.08f, 0.0f));
		ActiveItemButton->SetBackgroundColor(FLinearColor(0.7f, 0.6f, 0.08f, 0.0f));


	}
	else
	{
		ItemButton->SetBackgroundColor(FLinearColor(0.7f, 0.6f, 0.08f, 0.0f));
		ActiveItemButton->SetBackgroundColor(FLinearColor(0.7f, 0.6f, 0.08f, 0.0f));
		PassiveButton->SetBackgroundColor(FLinearColor(0.7f, 0.6f, 0.08f, 0.0f));
	}
}

// item pannel
void UStoreWidget::OnClick_ItemButton()
{
	ItemPannel->SetVisibility(ESlateVisibility::Visible);
	ActiveItemPannel->SetVisibility(ESlateVisibility::Hidden);
	PassiveItemPannel->SetVisibility(ESlateVisibility::Hidden);

}
void UStoreWidget::OnClick_Item1()
{
	itemindex = 1;
	ChangeImage();
}

void UStoreWidget::OnClick_Item2()
{
	itemindex = 2;
	ChangeImage();
}

void UStoreWidget::OnClick_Item4()
{
	itemindex = 4;
	ChangeImage();
}

void UStoreWidget::OnClick_Item5()
{
	itemindex = 5;
	ChangeImage();
}

void UStoreWidget::OnClick_Item6()
{
	itemindex = 6;
	ChangeImage();
}

void UStoreWidget::OnClick_Item7()
{
	itemindex = 7;
	ChangeImage();
}

void UStoreWidget::OnClick_Item8()
{
	itemindex = 8;
	ChangeImage();
}


// active item pannel
void UStoreWidget::OnClick_ActiveItemButton()
{
	ItemPannel->SetVisibility(ESlateVisibility::Hidden);
	ActiveItemPannel->SetVisibility(ESlateVisibility::Visible);
	PassiveItemPannel->SetVisibility(ESlateVisibility::Hidden);

}

void UStoreWidget::OnClick_Item3()
{
	itemindex = 3;
	ChangeImage();
}

void UStoreWidget::OnClick_Item9()
{
	itemindex = 9;
	ChangeImage();
}

void UStoreWidget::OnClick_Item10()
{
	itemindex = 10;
	ChangeImage();
}

void UStoreWidget::OnClick_Item11()
{
	itemindex = 11;
	ChangeImage();
}


void UStoreWidget::OnClick_PassiveItemButton()
{
	ItemPannel->SetVisibility(ESlateVisibility::Hidden);
	ActiveItemPannel->SetVisibility(ESlateVisibility::Hidden);
	PassiveItemPannel->SetVisibility(ESlateVisibility::Visible);
}

void UStoreWidget::OnClick_PassiveItem1()
{
	SelectItemName->SetText(FText::FromString(FString::Printf(TEXT("공격력"))));
	SelectItemExplain->SetText(FText::FromString(FString::Printf(TEXT("공격력을 3 올립니다.\n인벤토리에 들어가지 않습니다."))));
	SelectImage->SetOpacity(1.0f);
	SelectImage->SetBrushFromTexture(Cast<UTexture2D>(PassiveItem1_Img->GetBrush().GetResourceObject()));
	itemindex = 101;
}

void UStoreWidget::OnClick_PassiveItem2()
{
	SelectItemName->SetText(FText::FromString(FString::Printf(TEXT("방어력"))));
	SelectItemExplain->SetText(FText::FromString(FString::Printf(TEXT("방어력을 3 올립니다.\n인벤토리에 들어가지 않습니다."))));
	SelectImage->SetOpacity(1.0f);
	SelectImage->SetBrushFromTexture(Cast<UTexture2D>(PassiveItem2_Img->GetBrush().GetResourceObject()));
	itemindex = 102;
}

void UStoreWidget::OnClick_PassiveItem3()
{
	SelectItemName->SetText(FText::FromString(FString::Printf(TEXT("체력"))));
	SelectItemExplain->SetText(FText::FromString(FString::Printf(TEXT("전체 체력을 50 올립니다.\n인벤토리에 들어가지 않습니다."))));
	SelectImage->SetOpacity(1.0f);
	SelectImage->SetBrushFromTexture(Cast<UTexture2D>(PassiveItem3_Img->GetBrush().GetResourceObject()));
	itemindex = 103;
}

void UStoreWidget::OnClick_PassiveItem4()
{
	SelectItemName->SetText(FText::FromString(FString::Printf(TEXT("마나"))));
	SelectItemExplain->SetText(FText::FromString(FString::Printf(TEXT("전체 마나를 50 올립니다.\n인벤토리에 들어가지 않습니다."))));
	SelectImage->SetOpacity(1.0f);
	SelectImage->SetBrushFromTexture(Cast<UTexture2D>(PassiveItem4_Img->GetBrush().GetResourceObject()));
	itemindex = 104;
}

void UStoreWidget::ChangeImage()
{
	FItem* item = tempItemDataTable->FindRow<FItem>(*FString::FromInt(itemindex), TEXT(""));
	SelectItemName->SetText(FText::FromString(item->name));
	SelectItemExplain->SetText(FText::FromString(item->explain));
	SelectImage->SetOpacity(1.0f);
	SelectImage->SetBrushFromTexture(item->image);
}

void UStoreWidget::OnClick_PurchaseItem()
{
	FItem* item = tempItemDataTable->FindRow<FItem>(*FString::FromInt(itemindex), TEXT(""));
	if (item != nullptr) {
		if (player->gold < item->price || player->itemIndexArray.Num() > 7 )
		{
			return;
		}
		player->gold -= item->price;
		player->itemIndexArray.Add(itemindex);
		player->allPlayerWidget->ChangeInventory();
		player->ChangeStat();
	}
	else if(item == nullptr)
	{
		if (player->gold < 100) return;
		player->gold -= 100;
		if (itemindex == 101) player->origin_AttackPoint += 3;
		else if (itemindex == 102) player->origin_DefencePoint += 3;
		else if (itemindex == 103) {
			player->origin_fullHP += 50;
			player->statment.healthPoint += 50;
		}
		else if (itemindex == 104) {
			player->origin_fullMP += 50;
			player->statment.manaPoint += 50;
		}
		player->ChangeStat();
	}
}

void UStoreWidget::OnClick_ExitButton()
{
	if (player->itemIndexArray.Num() <= 0) return;
	int32 itemnumber = player->itemIndexArray[player->itemIndexArray.Num() - 1];
	FItem* item = tempItemDataTable->FindRow<FItem>(*FString::FromInt(itemnumber), TEXT(""));
	player->gold += item->price;
	player->itemIndexArray.Pop();
	player->allPlayerWidget->ChangeInventory();
	player->ChangeStat();

}
