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

void UStoreWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Item1->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_Item1);
	Item2->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_Item2);
	PurchaseButton->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_PurchaseItem);
	ExitButton->OnClicked.AddDynamic(this, &UStoreWidget::OnClick_ExitButton);
	gamestate = Cast<ABossRoomGameStateBase>(GetWorld()->GetGameState());

	player = Cast<ABossHunterCharacter>(GetOwningPlayer()->GetPawn());
	tempItemDataTable = gamestate->itemDataTable;
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
	}
	else if (PassiveButton->IsHovered())
	{
		PassiveButton->SetBackgroundColor(FLinearColor(0.7f, 0.6f, 0.08f, 0.4f));
	}
	else
	{
		ActiveItemButton->SetBackgroundColor(FLinearColor(0.7f, 0.6f, 0.08f, 0.0f));
		PassiveButton->SetBackgroundColor(FLinearColor(0.7f, 0.6f, 0.08f, 0.0f));
	}
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
	if (player->gold < item->price || player->itemIndexArray.Num() > 8)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("%d"), item->itemIndex);
	player->gold -= item->price;
	player->itemIndexArray.Add(itemindex);
	player->allPlayerWidget->ChangeInventory();
	player->ChangeStat();
}

void UStoreWidget::OnClick_ExitButton()
{
	WidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
}
