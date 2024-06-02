// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyStoreWidget.h"
#include <Kismet/GameplayStatics.h>
#include "SavePlayerScore.h"
#include "PlayerScoreComp.h"
#include "PlayerScoreWidget.h"
#include "GunPlayer.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include "LobbyStore/LobbyStoreActor.h"
#include <Components/WidgetSwitcher.h>
#include <Components/Image.h>
#include <Engine/Texture2D.h>
#include "PlayerSkinComp.h"






ULobbyStoreWidget::ULobbyStoreWidget(const FObjectInitializer& ObjectInitializer):UUserWidget(ObjectInitializer)
{
	//UE_LOG(LogTemp, Warning, TEXT(" Lobby sotre widget Create"));

	//이미지 세팅
	SetTextureImage();

	
	//스켈레톤 매쉬를 생성해서 들고 있자.
	ConstructorHelpers::FObjectFinder<USkeletalMesh>nomalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonLtBelica/Characters/Heroes/Belica/Meshes/Belica.Belica'"));
	if (nomalMesh.Succeeded())
	{
		compMesh.Add(nomalMesh.Object);
	}
	ConstructorHelpers::FObjectFinder<USkeletalMesh>amorMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonLtBelica/Characters/Heroes/Belica/Skins/HeavyArmor/Meshes/Belica_HeavyArmor.Belica_HeavyArmor'"));
	if (amorMesh.Succeeded())
	{
		compMesh.Add(amorMesh.Object);
	}


}


void ULobbyStoreWidget::NativeConstruct()
{
	//오버라이드함수
	Super::NativeConstruct();

	//클릭해서 위젯을 닫자
	//UButton
	btn_Cancel->OnClicked.AddDynamic(this, &ULobbyStoreWidget::OnClickCloseWidget);
	btn_buy->OnClicked.AddDynamic(this, &ULobbyStoreWidget::OnClickBuyItem);
	btn_MakeUp->OnClicked.AddDynamic(this, &ULobbyStoreWidget::OnClickMakeUp);
	//btn_BossUp->OnClicked.AddDynamic(this, &ULobbyStoreWidget::OnClickBossUp);
	//btn_BattleItem->OnClicked.AddDynamic(this, &ULobbyStoreWidget::OnClickBattleItem);
	btn_Add->OnClicked.AddDynamic(this, &ULobbyStoreWidget::OnClickIncrease);
	btn_Sub->OnClicked.AddDynamic(this, &ULobbyStoreWidget::OnClickDecrease);
	btn_NormalSkin->OnClicked.AddDynamic(this, &ULobbyStoreWidget::OnClickMakeUPNormalSkin);
	btn_AmorSkin->OnClicked.AddDynamic(this, &ULobbyStoreWidget::OnClickMakeUPAmorSkin);
	btn_SkinEquip->OnClicked.AddDynamic(this, &ULobbyStoreWidget::OnClickSkinEquip);


	//위젯에 점수등록
	LoadPlayerScore();

	//플레이어
	AActor* findPlayer = UGameplayStatics::GetActorOfClass(GetWorld(), AGunPlayer::StaticClass()); 
	player = Cast<AGunPlayer>(findPlayer);

}

void ULobbyStoreWidget::SetTextureImage()
{
	//texture2D
	ConstructorHelpers::FObjectFinder<UTexture2D>temptexture2D_00(TEXT("/Script/Engine.Texture2D'/Game/BulePrint/Cheezebee/Widget/Image/NormalSkinImage.NormalSkinImage'"));
	if (temptexture2D_00.Succeeded())
	{
		texture2D.Add(temptexture2D_00.Object);

	}
	ConstructorHelpers::FObjectFinder<UTexture2D>temptexture2D_01(TEXT("/Script/Engine.Texture2D'/Game/BulePrint/Cheezebee/Widget/Image/AmorSkinImage.AmorSkinImage'"));
	if (temptexture2D_01.Succeeded())
	{
		texture2D.Add(temptexture2D_01.Object);

	}

}

void ULobbyStoreWidget::LoadPlayerScore()
{
	//UGameplayStatics
	// 1. "BEST_SCORE" 으로 되어 있는 데이터를 불러온다. (USaveGame*)
	USaveGame* loadGame = UGameplayStatics::LoadGameFromSlot(TEXT("BEST_SCORE"), 0);
	// 2. 만약에 잘 읽어왔다면 (저장한 값이 있었다면)
	if (loadGame != nullptr)
	{
		//USavePlayerScore
		// 3. 불러온 객체를 UCSaveData 로 형변환
		USavePlayerScore* loadData = Cast<USavePlayerScore>(loadGame);
		//AGunPlayer
		//UPlayerScoreComp
		// 4. bestScore = saveBestScore
		playerScore = loadData->saveBestScore;
		//UPlayerScoreWidget
		// 5. 최고 점수 UI 갱신
		/*playerScoreWidget->UpdateBestScoreUI(playerScoreComp->bestSocre);*/

		FString strBestScore = FString::Printf(TEXT("%d"), playerScore);

		//UTextBlock
		text_PlayerScore->SetText(FText::FromString(strBestScore));

		UE_LOG(LogTemp, Warning, TEXT("Load playerScore %d"), playerScore);



	}
}

void ULobbyStoreWidget::OnClickCloseWidget()
{
	
	SetVisibility(ESlateVisibility::Hidden);
	//UE_LOG(LogTemp, Warning, TEXT("close widget"));

}

void ULobbyStoreWidget::OnClickBuyItem()
{
	//UE_LOG(LogTemp, Warning, TEXT("lobby store widget OnClickBuyItem"));
	
	if (playerScore < 1000) return;
	AddScore(1000);
	if(ishaveAmorSkin == 1)
	{ 
		ishaveAmorSkin = 0;
		player->playerSkin->isamorSkin = 0;
		btn_buy->SetVisibility(ESlateVisibility::Hidden);
		btn_SkinEquip->SetVisibility(ESlateVisibility::Visible);
		player->playerSkin->haveAmorSkin = 0;
		player->playerSkin->SaveData();
	}
	
}

void ULobbyStoreWidget::OnClickSkinEquip()
{
	if (onclickNormalSkin == 0)
	{
		ChangePlayerMeshNormal();

		//UE_LOG(LogTemp, Warning, TEXT("onclickNormalSkin : %d"), onclickNormalSkin);
	}

	if (onclickAmorSkin == 0 && ishaveAmorSkin == 0)
	{
		ChangePlayerMeshAmor();
		
		//UE_LOG(LogTemp, Warning, TEXT("onclickAmorSkin : %d ,  ishaveAmorSkin : %d"), onclickAmorSkin, ishaveAmorSkin);

	}
	
	//ServerRPC_OnClickSkinEquip(onclickNormalSkin, onclickAmorSkin, ishaveAmorSkin);
	//MultiRPC_OnClickSkinEquip(onclickNormalSkin, onclickAmorSkin, ishaveAmorSkin);
}

void ULobbyStoreWidget::ServerRPC_OnClickSkinEquip_Implementation(int32 a, int32 b, int32 c)
{
	a = onclickNormalSkin;
	b = onclickAmorSkin;
	c = ishaveAmorSkin;

	if (a)
	{
		ChangePlayerMeshNormal();

	}

	if (b && c)
	{
		ChangePlayerMeshAmor();

	}

	MultiRPC_OnClickSkinEquip(a, b, c);

}

void ULobbyStoreWidget::MultiRPC_OnClickSkinEquip_Implementation(int32 a, int32 b, int32 c)
{
	a = onclickNormalSkin;
	b = onclickAmorSkin;
	c = ishaveAmorSkin;

	if (a)
	{
		ChangePlayerMeshNormal();

	}

	if (b && c)
	{
		ChangePlayerMeshAmor();

	}
}

void ULobbyStoreWidget::AddScore(int32 value)
{
	//벨류가 플레이어 스코어보다 크다면 리턴
	if(playerScore < value) return;
	//플레이어 스코어에서 벨류를 빼자.
	playerScore -= value;


	UE_LOG(LogTemp, Warning, TEXT("lobby store widget AddScore : %d"), playerScore);

	//세이브게임을 가져오자.
	USaveGame* saveGame = UGameplayStatics::CreateSaveGameObject(USavePlayerScore::StaticClass());
	//세이브형변환
	USavePlayerScore* saveData = Cast<USavePlayerScore>(saveGame);

	//세이브 데이터가 가진 값을 현재 플레이 스코어로 갱신
	saveData->saveBestScore = playerScore;
	
	//결과값 데이터를 저장하자.
	UGameplayStatics::SaveGameToSlot(saveData, TEXT("BEST_SCORE"), 0);

	//플레이어 스코어를 세이브 데이터로 갱신
	//playerScore = saveData->saveBestScore;
	
	//플레이어 스코어의 값으로  UI를 갱신
	FString strStorePlayerScore = FString::Printf(TEXT("%d"), playerScore);
	//로비 스토어 위젯의 스코어 텍스트를 갱신
	text_PlayerScore->SetText(FText::FromString(strStorePlayerScore));
	
	AActor* findActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGunPlayer::StaticClass());
	player = Cast<AGunPlayer>(findActor);
	
	//그걸 가져오자
	FString strPlayerScoreWidghet = FString::Printf(TEXT("%d"), playerScore);
	//플레이어 스코어 위젯의 텍스트를 갱신
	player->playerScore->playerScoreUI->text_PlayerScore->SetText(FText::FromString(strPlayerScoreWidghet));

}

void ULobbyStoreWidget::AddSkinIndex(int32 value)
{

	//플레이어 스코어에서 벨류를 빼자.
	widgetSkinIdx = value;

	UE_LOG(LogTemp, Warning, TEXT("lobby store widget AddSkinIndex : %d"), widgetSkinIdx);

	//세이브게임을 가져오자.
	//UGameplayStatics
	USaveGame* saveGame = UGameplayStatics::CreateSaveGameObject(USavePlayerScore::StaticClass());
	
	//세이브형변환
	USavePlayerScore* saveData = Cast<USavePlayerScore>(saveGame);

	//세이브 데이터가 가진 값을 현재 플레이 스코어로 갱신
	saveData->savePlayerSkinIdx = widgetSkinIdx;

	//위젯스킨 인덱스 값을 세이브 값으로 갱신
	widgetSkinIdx = saveData->savePlayerSkinIdx;

	//결과값 데이터를 1번에 저장하자.
	UGameplayStatics::SaveGameToSlot(saveData, TEXT("Skin_Index"), 0);
	UE_LOG(LogTemp, Warning, TEXT("add save skin idx : %d"), saveData->savePlayerSkinIdx);
}


void ULobbyStoreWidget::OnClickMakeUp()
{
	UE_LOG(LogTemp, Warning, TEXT("lobby store widghet OnClickMakeUp"));
	
	//삭제대기
	//WidgetSwitcher_LobbyStore->SetActiveWidgetIndex(0);
	
	//UImage
	//UTexture2D* findImage = FindObject<UTexture2D>(ANY_PACKAGE, TEXT("/Script/Engine.Texture2D'/Game/BulePrint/Cheezebee/Widget/image/good_child.good_child'"));
	//image_MakeUp->SetBrushFromTexture(texture2D[0]);
}

void ULobbyStoreWidget::OnClickMakeUPNormalSkin()
{
	//삭제대기
	//WidgetSwitcher_MakeupCategory->SetActiveWidgetIndex(0);
	
	//구매버튼, 착용버튼 보이게하자.
	btn_buy->SetVisibility(ESlateVisibility::Hidden);
	btn_SkinEquip->SetVisibility(ESlateVisibility::Visible);

	//클릭했는지 검출
	onclickNormalSkin = 0;
	onclickAmorSkin = 1;

	//버튼 아웃라인 강조
	//버튼 아웃라인 컬러 색깔
	FLinearColor noneColor(0.0f, 0.0f, 0.0f, 0.0f);
	FLinearColor backColor(0.0f, 0.0f, 0.0f, 1.0f);
	//전체가 바뀜.
	//btn_AmorSkin->SetColorAndOpacity(backColor);
	btn_NormalSkin->WidgetStyle.FButtonStyle::Normal.TintColor = backColor;
	//아웃라인색상
	btn_NormalSkin->WidgetStyle.FButtonStyle::Normal.OutlineSettings.Color = FLinearColor::Green;
	//아웃라인두께
	btn_NormalSkin->WidgetStyle.FButtonStyle::Normal.OutlineSettings.Width = 3.0f;
	//위젯의 컬러를 끄자.
	btn_AmorSkin->WidgetStyle.FButtonStyle::Normal.TintColor = noneColor;


	//UPlayerSkinComp
	player->playerSkin->isnormalSkin = true;
	player->playerSkin->isamorSkin = false;

	//색깔지정
	image_ClickNormal->SetBrushTintColor(FLinearColor::Blue);
	FLinearColor newColor(0.0f , 0.0f, 0.0f, 0.0f);
	image_ClickAmor->SetBrushTintColor(newColor);

	//노말 스킨으로 이미지 변경
	image_SkinPreView->SetBrushFromTexture(texture2D[0]);

	

	//노말스킨이 트루이면 매쉬변경
	//ChangePlayerMeshNormal();
	//AddSkinIndex(0);

}

void ULobbyStoreWidget::ChangePlayerMeshNormal()
{
	//player->GetMesh()->SetSkeletalMesh(compMesh[0]);
	//UE_LOG(LogTemp, Warning, TEXT("11111111111"));
	player->playerSkin->ChangePlayerMeshNormal();
}

void ULobbyStoreWidget::OnClickMakeUPAmorSkin()
{
	//삭제대기
	//WidgetSwitcher_MakeupCategory->SetActiveWidgetIndex(1);
	
	//클릭했는지 검출
	onclickNormalSkin = 1;
	onclickAmorSkin = 0;

	if (ishaveAmorSkin == 1)
	{
		btn_buy->SetVisibility(ESlateVisibility::Visible);
		btn_SkinEquip->SetVisibility(ESlateVisibility::Hidden);
	}
	//구매버튼, 착용버튼 보이게하자.
	if (ishaveAmorSkin == 0)
	{
		btn_buy->SetVisibility(ESlateVisibility::Hidden);
		btn_SkinEquip->SetVisibility(ESlateVisibility::Visible);
	}
	

	//버튼 아웃라인 강조
	//버튼 아웃라인 컬러 색깔
	FLinearColor noneColor(0.0f, 0.0f, 0.0f, 0.0f);
	FLinearColor backColor(0.0f, 0.0f, 0.0f, 1.0f );
	
	//전체가 바뀜.
	//btn_AmorSkin->SetColorAndOpacity(backColor);
	
	//버튼 아웃라인조정
	btn_AmorSkin->WidgetStyle.FButtonStyle::Normal.TintColor = backColor;
	btn_AmorSkin->WidgetStyle.FButtonStyle::Normal.OutlineSettings.Color = FLinearColor::Green;
	btn_AmorSkin->WidgetStyle.FButtonStyle::Normal.OutlineSettings.Width = 3.0f;
	btn_NormalSkin->WidgetStyle.FButtonStyle::Normal.TintColor = noneColor;
	
	//선택 이미지 색깔
	image_ClickAmor->SetBrushTintColor(FLinearColor::Blue);
	FLinearColor newColor(0.0f, 0.0f, 0.0f, 0.0f);
	image_ClickNormal->SetBrushTintColor(newColor);
	
	player->playerSkin->isnormalSkin = false;
	player->playerSkin->isamorSkin = true;
	
	//아머스킨으로 이미지 변경
	image_SkinPreView->SetBrushFromTexture(texture2D[1]);

	

	//아머스킨이 트루이면 매쉬변경
	//ChangePlayerMeshAmor();
	//AddSkinIndex(1);

}

void ULobbyStoreWidget::ChangePlayerMeshAmor()
{
	
	//player->GetMesh()->SetSkeletalMesh(compMesh[1]);
	//UE_LOG(LogTemp, Warning, TEXT("2"));
	player->playerSkin->ChangePlayerMeshAmor();
}

void ULobbyStoreWidget::OnClickBossUp()
{
	//UE_LOG(LogTemp, Warning, TEXT("lobby store widghet OnClickBossUp"));
	//삭제대기
	//WidgetSwitcher_LobbyStore->SetActiveWidgetIndex(1);
	
	//image_MakeUp->SetBrushFromTexture(texture2D[1]);
}

void ULobbyStoreWidget::OnClickBattleItem()
{
	//UE_LOG(LogTemp, Warning, TEXT("lobby store widghet OnClickBattleItem"));
	
	//삭제대기
	//WidgetSwitcher_LobbyStore->SetActiveWidgetIndex(2);
	//image_MakeUp->SetBrushFromTexture(texture2D[2]);
}

void ULobbyStoreWidget::OnClickIncrease()
{
	AddScore(-1000);
}

void ULobbyStoreWidget::OnClickDecrease()
{
	
	AddScore(1000);
}
