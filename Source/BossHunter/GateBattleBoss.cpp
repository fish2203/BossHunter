// Fill out your copyright notice in the Description page of Project Settings.


#include "GateBattleBoss.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include "GunPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "NetGameInstance.h"
#include "MoveBossZoneWidget.h"
#include "RestartBossZone.h"
#include "BossRoomGameStateBase.h"

// Sets default values
AGateBattleBoss::AGateBattleBoss()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//#include <Components/BoxComponent.h>
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("GateBox"));
	SetRootComponent(boxComp);

	meshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateMesh"));
	meshComp->SetupAttachment(RootComponent);

	//#include <Components/StaticMeshComponent.h>
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/EnvironmentPack1/Meshes/EnvironmentPack_1/SM_DungeonBaseDoorFrame.SM_DungeonBaseDoorFrame'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}
}

// Called when the game starts or when spawned
void AGateBattleBoss::BeginPlay()
{
	Super::BeginPlay();
	
	gi = GetGameInstance<UNetGameInstance>();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AGateBattleBoss::OnOverlap);

	bool visibility = true;
	//로비->보스존으로 이동하는 위젯
	movewidgetPointer = CreateWidget<UMoveBossZoneWidget>(GetWorld(), moveWidgetPackage);
	movewidgetPointer->AddToViewport(1);
	//movewidgetPointer->SetVisibility(ESlateVisibility::Visible);
	movewidgetPointer->SetVisibility(ESlateVisibility::Hidden);

	/*sessionInterface->GetResolvedConnectString(SessionName, url);
	UE_LOG(LogTemp, Warning, TEXT("Join session URL : %s"), *url);*/

	//보스존에서 재시작하는 위젯
	restartBossZone = CreateWidget<URestartBossZone>(GetWorld(), restartbosszonePackage, FName("restart"));
	restartBossZone->AddToViewport(1);
	//restartBossZone->SetVisibility(ESlateVisibility::Visible);
	restartBossZone->SetVisibility(ESlateVisibility::Hidden);





	
}

// Called every frame
void AGateBattleBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	gamestate = ABossRoomGameStateBase::Get();
	TArray<AActor*> countArray;
	//ABossHunterCharacter* player = Cast<ABossHunterCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	UGameplayStatics::GetAllActorsOfClass(this, AGunPlayer::StaticClass(), countArray);
	gamestate->playerCount = countArray.Num();
}

void AGateBattleBoss::OnOverlap(UPrimitiveComponent* abc, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT(" gatebattleboss overlap fail"));
	if (OtherActor->HasAuthority() == false) return;
	//위젯 보이게하자.
	movewidgetPointer->SetVisibility(ESlateVisibility::Visible);
	
	//UE_LOG(LogTemp, Warning, TEXT(" gatebattleboss overlap "));
	
	/*
	URLTravel();
	if (OtherActor->GetName().Contains(TEXT("player")))
	{
		gi->EnterOtherMap();
		
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("BossZone"));
		MultiRPC_URLTravel();
	}
	*/
}

void AGateBattleBoss::URLTravel()
{
	if (HasAuthority() == true)
	{
		ServerRPC_URLTravel();
	}
}

void AGateBattleBoss::ServerRPC_URLTravel_Implementation()
{
	
	//FString URL = TEXT("/Game/BulePrint/Cheezebee/Map/BossHunter/BossZone?listen");
	FString URL = TEXT("/Game/BulePrint/JINA/Map/LoadingMap?listen");
	GetWorld()->ServerTravel(URL, ETravelType::TRAVEL_Absolute);

	//MultiRPC_URLTravel();
}

//이동할 맵의 URL을 지정하자.
	/*FString URL = TEXT("/Game/Maps/BossZone?listen");
	APlayerController* pc = GetWorld()->GetFirstPlayerController();*/
	// 해당 URL 로 접속하자
	/*pc->ClientTravel(URL, ETravelType::TRAVEL_Absolute);

	GetWorld()->ServerTravel(URL, ETravelType::TRAVEL_Absolute);*/
	//GetWorld()->ServerTravel(TEXT("/Game/BulePrint/Cheezebee/Map/BossHunter/LobbyRoom?listen"));
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("BossZone"));


void AGateBattleBoss::MultiRPC_URLTravel_Implementation()
{
	if (HasAuthority() == true) return;

}

void AGateBattleBoss::viewRestartWidget()
{
	restartBossZone->SetVisibility(ESlateVisibility::Visible);
}

//FString URL = TEXT("");
	//APlayerController* pc = GetWorld()->GetFirstPlayerController();
	//// 해당 URL 로 접속하자
	//pc->ClientTravel(URL, ETravelType::TRAVEL_Absolute);

//void AGateBattleBoss::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result)
//{
//	if (result == EOnJoinSessionCompleteResult::Success)
//	{
//		FString url = TEXT("192.168.0.5:7777");
//		APlayerController* pc = GetWorld()->GetFirstPlayerController();
//		pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
//
//
//	}
//}
