// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyStoreMakeUpSkinWidget.h"
#include "Components/Button.h"
#include "LobbyStoreWidget.h"
#include <Kismet/GameplayStatics.h>

ULobbyStoreMakeUpSkinWidget::ULobbyStoreMakeUpSkinWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UTexture2D>textureNomal(TEXT("/Script/Engine.Texture2D'/Game/BulePrint/Cheezebee/Widget/image/PlayerSkinNormal.PlayerSkinNormal'"));
	if (textureNomal.Succeeded())
	{
		texture2D.Add(textureNomal.Object);
	}

	ConstructorHelpers::FObjectFinder<UTexture2D>textureEX(TEXT("/Script/Engine.Texture2D'/Game/BulePrint/Cheezebee/Widget/image/PlayerSkinHeavyAmor.PlayerSkinHeavyAmor'"));
	if (textureEX.Succeeded())
	{
		texture2D.Add(textureEX.Object);
	}


}

void ULobbyStoreMakeUpSkinWidget::NativeConstruct()
{
	//오버라이드함수
	Super::NativeConstruct();

	//UUserWidget* findWidget = UGameplayStatics::Get
	//(GetWorld(), ULobbyStoreWidget::Statics);
	//lobbyStoreWidget = Cast<ULobbyStoreWidget>();

	//UButton
	Btn_WearSkin->OnClicked.AddDynamic(this, &ULobbyStoreMakeUpSkinWidget::OnClickWearSkin);

}

void ULobbyStoreMakeUpSkinWidget::OnClickWearSkin()
{
	UE_LOG(LogTemp, Warning, TEXT("lobby store makeup skin OnClickWearNomalSkin"))

	//노말스킨이 트루이면 매쉬변경
	//ULobbyStoreWidget
	lobbyStoreWidget-> ChangePlayerMeshNormal();
	lobbyStoreWidget-> AddSkinIndex(0);

}
