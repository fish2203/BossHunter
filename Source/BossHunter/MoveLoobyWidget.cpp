// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveLoobyWidget.h"
#include <Components/Button.h>
#include "BossDropItem.h"
#include <Kismet/GameplayStatics.h>
#include "GunPlayer.h"


void UMoveLoobyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//UButton
	Btn_MoveLooby->OnClicked.AddDynamic(this, &UMoveLoobyWidget::OnClickMoveLobby);
	Btn_Cancel->OnClicked.AddDynamic(this, &UMoveLoobyWidget::OnClickCancel);
}


void UMoveLoobyWidget::OnClickMoveLobby()
{
	//UGameplayStatics
	//ABossDropItem
	//UE_LOG(LogTemp, Warning, TEXT("OnClickMoveLobby"));

	AActor* findGate = UGameplayStatics::GetActorOfClass(GetWorld(), ABossDropItem::StaticClass());
	bossdropItem = Cast<ABossDropItem>(findGate);
	
	AActor* findPlayer = UGameplayStatics::GetActorOfClass(GetWorld(), AGunPlayer::StaticClass());;
	AGunPlayer* player = Cast<AGunPlayer>(findPlayer);

	if(player->HasAuthority())
	{
		bossdropItem->LobbyURLTravel();
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;

	}
}

void UMoveLoobyWidget::OnClickCancel()
{
	//위젯을 감추자
	SetVisibility(ESlateVisibility::Hidden);
}

