// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkinComp.h"
#include "LobbyStoreWidget.h"
#include "GunPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "SavePlayerScore.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/SaveGame.h>

// Sets default values for this component's properties
UPlayerSkinComp::UPlayerSkinComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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


// Called when the game starts
void UPlayerSkinComp::BeginPlay()
{
	Super::BeginPlay();


	AActor * findplayer = UGameplayStatics::GetActorOfClass(GetWorld(), AGunPlayer::StaticClass());
	//player = Cast<AGunPlayer>(findplayer);

	player = Cast<AGunPlayer>(GetOwner());
	
	//LoadData();
	if(player->IsLocallyControlled())
	{

		LoadData();
		ServerRPC_LoadData();
		MultiRPC_LoadData();

	}

	if (haveAmorSkin == 0)
	{
		
		/*UButton* findbtn = lobbyStoreWidget->btn_buy();
		lobbyStoreWidget->btn_buy->SetVisibility(ESlateVisibility::Hidden);
		lobbyStoreWidget::btn_SkinEquip->SetVisibility(ESlateVisibility::Visible);*/
	}

}


// Called every frame
void UPlayerSkinComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerSkinComp::ChangePlayerMeshNormal()
{
	PlayerSkinIdx = 0;
	//AGunPlayer
	player = Cast<AGunPlayer>(GetOwner());
	if (player->IsLocallyControlled())
	{
		player->GetMesh()->SetSkeletalMesh(compMesh[0]);
		SaveData();
		ServerRPC_ChangePlayerSkin(0);
		//UE_LOG(LogTemp, Warning, TEXT(" 111111111"));
	}
	
	

}

void UPlayerSkinComp::ChangePlayerMeshAmor()
{
	PlayerSkinIdx = 1;
	player = Cast<AGunPlayer>(GetOwner());
	if (player->IsLocallyControlled())
	{
		player->GetMesh()->SetSkeletalMesh(compMesh[1]);
		
		SaveData();
		ServerRPC_ChangePlayerSkin(1);
		//UE_LOG(LogTemp, Warning, TEXT(" 2222222"));
	}
	
	
}

void UPlayerSkinComp::ServerRPC_ChangePlayerSkin_Implementation(int32 idx)
{
	if (idx == 0)
	{ 
		player->GetMesh()->SetSkeletalMesh(compMesh[idx]);
	}
	else
	{
		player->GetMesh()->SetSkeletalMesh(compMesh[idx]);
	
	}
	MultiRPC_ChangePlayerSkin(idx);

}


void UPlayerSkinComp::MultiRPC_ChangePlayerSkin_Implementation(int32 idx)
{
	if (idx == 0)
	{
		player->GetMesh()->SetSkeletalMesh(compMesh[idx]);
	}
	else
	{
		player->GetMesh()->SetSkeletalMesh(compMesh[idx]);

	}
}

void UPlayerSkinComp::SaveData()
{
	// 1. USavePlayerScore 를 하나 생성한다. 
	USaveGame* saveGame = UGameplayStatics::CreateSaveGameObject(USavePlayerScore::StaticClass());
	// 2. 생성한 놈을 CSaveData 로 형변환
	USavePlayerScore* saveData = Cast<USavePlayerScore>(saveGame);
	// 3. saveData->savePlayerSkinIdx = PlayerSkinIdx 
	saveData->savePlayerSkinIdx = PlayerSkinIdx;
	saveData->playerSkinDataArray = valuePlayerSkinCompArray;
	saveData->saveHaveAmorSkin = haveAmorSkin;
	// 4. USavePlayerScore 을 저장
	UGameplayStatics::SaveGameToSlot(saveData, TEXT("Skin_Index"), 0);
	UE_LOG(LogTemp, Warning, TEXT("Skin_Index : %d "), PlayerSkinIdx);

}

void UPlayerSkinComp::LoadData()
{
	// 1 데이터를 불러온다. (USaveGame*)
	USaveGame* loadGame = UGameplayStatics::LoadGameFromSlot(TEXT("Skin_Index"), 0);
	// 2. 만약에 잘 읽어왔다면 (저장한 값이 있었다면)
	if (loadGame != nullptr)
	{
		// 3. 불러온 객체를 UCSaveData 로 형변환
		//USavePlayerScore
		USavePlayerScore* loadData = Cast<USavePlayerScore>(loadGame);
		// 4. bestScore = saveBestScore
		PlayerSkinIdx = loadData->savePlayerSkinIdx;
		haveAmorSkin = loadData->saveHaveAmorSkin;
		//UE_LOG(LogTemp, Warning, TEXT("Load PlayerComp Load SkinIdx %d"), PlayerSkinIdx);

	}
}

void UPlayerSkinComp::ServerRPC_LoadData_Implementation()
{
	
	if (PlayerSkinIdx == 0)
	{
		ChangePlayerMeshNormal();
		//UE_LOG(LogTemp, Warning, TEXT("player comp nomal skin"));
	}

	if (PlayerSkinIdx == 1)
	{
		ChangePlayerMeshAmor();
		//UE_LOG(LogTemp, Warning, TEXT("player comp amor skin"));
	}
	
	MultiRPC_LoadData();
}

void UPlayerSkinComp::MultiRPC_LoadData_Implementation()
{
	if (PlayerSkinIdx == 0)
	{
		ChangePlayerMeshNormal();
		//UE_LOG(LogTemp, Warning, TEXT("player comp nomal skin"));
	}

	if (PlayerSkinIdx == 1)
	{
		ChangePlayerMeshAmor();
		//UE_LOG(LogTemp, Warning, TEXT("player comp amor skin"));
	}
}

