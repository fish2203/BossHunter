// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHunter/LobbyStore/LobbyStoreActor.h"
#include <BossHunter/LobbyStoreWidget.h>
#include <Components/BoxComponent.h>
#include <Components/SceneComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Sound/SoundBase.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "../GunPlayer.h"

// Sets default values
ALobbyStoreActor::ALobbyStoreActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//USceneComponent
	sceneComp=CreateDefaultSubobject<USceneComponent>(TEXT("Scene Comp"));
	SetRootComponent(sceneComp);
	
	//UBoxComponent
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("box comp"));
	boxComp -> SetupAttachment(sceneComp);

	staticMesh= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("static mesh"));
	staticMesh->SetupAttachment(sceneComp);

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/EnvironmentPack1/Meshes/EnvironmentPack_1/SM_HornedManAltar.SM_HornedManAltar'"));
	if(tempMesh.Succeeded())
	{
		staticMesh->SetStaticMesh(tempMesh.Object);
	}

	ConstructorHelpers::FClassFinder<ULobbyStoreWidget>tempWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BulePrint/Cheezebee/Widget/WBP_LobbyStoreWidget.WBP_LobbyStoreWidget_C'"));
	if (tempWidget.Succeeded())
	{
		lobbyStoreWidgetPackage = tempWidget.Class;
	}
	//USoundBase
	
	ConstructorHelpers::FObjectFinder<USoundBase>templobbyMusic(TEXT("/Script/Engine.SoundCue'/Game/BulePrint/Cheezebee/LobbySound/SafeZone__cut_180sec__Cue.SafeZone__cut_180sec__Cue'"));
	if (templobbyMusic.Succeeded())
	{
		lobbyMusic = templobbyMusic.Object;
	}


}

// Called when the game starts or when spawned
void ALobbyStoreActor::BeginPlay()
{
	Super::BeginPlay();
	
	//ULobbyStoreWidget
	lobbyStoreWidget = CreateWidget<ULobbyStoreWidget>(GetWorld(), lobbyStoreWidgetPackage);
	
	if (lobbyStoreWidget != nullptr)
	{
		lobbyStoreWidget->AddToViewport(1);
		lobbyStoreWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	

	//박스컴포넌트와 오버렙하면 함수 호출
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ALobbyStoreActor::OnOverlap);

	PlayLobbyMusic();

	//AGunPlayer
	AActor* findgunplayer = UGameplayStatics::GetActorOfClass(GetWorld(), AGunPlayer::StaticClass());
	gunPlayer = Cast<AGunPlayer>(findgunplayer);

	
	TArray<AActor*> findallPlayer;
	TArray<int32*> num;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGunPlayer::StaticClass(), findallPlayer);

	for (int32 i = 0; i< findallPlayer.Num(); i++)
	{
		gunPlayers.Add(Cast<AGunPlayer>(findallPlayer[i]));

	}

}

// Called every frame
void ALobbyStoreActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALobbyStoreActor::OnOverlap(UPrimitiveComponent* abc, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	APawn* PlayerPawn = PlayerController->GetPawn();
	
	//오버렙한 아더액터를 플레이어로 캐스팅
	AGunPlayer* player = Cast<AGunPlayer>(OtherActor);
	if (player && player->IsLocallyControlled())
	{
		//모든 플레이어
		//UE_LOG(LogTemp, Warning, TEXT("1111111"));

		if (lobbyStoreWidget != nullptr)
		{
			lobbyStoreWidget->SetVisibility(ESlateVisibility::Visible);
			//UE_LOG(LogTemp, Warning, TEXT("lobby store actor overlap"));
		}
	}
	//모든 플레이어가 오버렙 할때 서버플레이어만 
	/*if(gunPlayer->IsLocallyControlled())
	{
		

	}*/

	
}

void ALobbyStoreActor::PlayLobbyMusic()
{
	UGameplayStatics::PlaySound2D(GetWorld(), lobbyMusic); 
}

