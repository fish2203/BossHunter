// Fill out your copyright notice in the Description page of Project Settings.


#include "BossDropItem.h"
#include <Components/PointLightComponent.h>
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include <Components/BoxComponent.h>
#include "MoveLoobyWidget.h"
#include "BossWidgetComponent.h"
#include "Test_Boss.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "GunPlayer.h"

// Sets default values
ABossDropItem::ABossDropItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	clearChest = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClearChest"));
	clearChest ->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempChest(TEXT("/Script/Engine.StaticMesh'/Game/BulePrint/Cheezebee/testasset/SM_MerchantShip_Storagebox_05.SM_MerchantShip_Storagebox_05'"));
	if (tempChest.Succeeded())
	{
		clearChest->SetStaticMesh(tempChest.Object);
	}
	
	//����ũ�⸦ 5���
	clearChest->SetRelativeScale3D(FVector(5));
	
	/*pointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light Comp"));
	pointLight->SetupAttachment(clearChest);*/

	pointLightArray.SetNum(4);

	for (int i = 0; i < pointLightArray.Num(); i++)
	{
		
		//�̸� ��°�
		FString	lightcompName = FString::Printf(TEXT("Light_%d"), i);
		//UChildActorComponent* lightComp = CreateDefaultSubobject<UChildActorComponent>(FName(lightcompName));
		 pointLight = CreateDefaultSubobject<UPointLightComponent>(FName(lightcompName));
		//������ �̹� num���� ������������ add �ϸ� �����߻���.
		pointLightArray[i] = pointLight;
		//Ŭ���� ü��Ʈ �ڽ����� ������
		pointLight->SetupAttachment(clearChest);
		pointLight->SetLightColor(FLinearColor::Yellow);

	}
	//UNiagaraComponent
	compItemNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DropItemNiagara"));
	compItemNiagara->SetupAttachment(clearChest);

	ConstructorHelpers::FObjectFinder<UNiagaraSystem>tempItem(TEXT("/Script/Niagara.NiagaraSystem'/Game/BulePrint/Cheezebee/testasset/NA_BossDropItem.NA_BossDropItem'"));
	if (tempItem.Succeeded())
	{
		compItemNiagara->SetAsset(tempItem.Object);
	}
	
	//UBoxComponent
	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("clear widget"));
	compBox -> SetupAttachment(clearChest);


}

// Called when the game starts or when spawned
void ABossDropItem::BeginPlay()
{
	Super::BeginPlay();

	compBox->OnComponentBeginOverlap.AddDynamic(this, &ABossDropItem::OnOverlap);

	//UBossWidgetComponent
	//UGameplayStatics
	//ATest_Boss
	AActor* findBoss = UGameplayStatics::GetActorOfClass(GetWorld(), ATest_Boss::StaticClass());
	bossActor = Cast<ATest_Boss>(findBoss);
	
}

// Called every frame
void ABossDropItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossDropItem::OnOverlap(UPrimitiveComponent* abc, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
		AGunPlayer* player = Cast<AGunPlayer>(GetOwner());
		bossActor->bossWidget->ViewBossClearWidget();
		UE_LOG(LogTemp, Warning, TEXT(" overlap chest "));
		
	//UE_LOG(LogTemp, Warning, TEXT(" overlap return "));
	//if (player->HasAuthority())
	//IsLocallyControlled()
	//bossWidgetComp->movelobbyWidget->SetVisibility(ESlateVisibility::Visible);
	//LobbyURLTravel();
	//}
	

}

void ABossDropItem::LobbyURLTravel()
{
	//UE_LOG(LogTemp,Warning, TEXT("1111111111"));
	if (!HasAuthority())
	{
		return;

		//UE_LOG(LogTemp, Warning, TEXT("22222"));
	}
	else
	{
		ServerRPC_LobbyURLTravel();
	}
}

void ABossDropItem::ServerRPC_LobbyURLTravel_Implementation()
{
	if(HasAuthority() == false) return;
	FString URL = TEXT("/Game/BulePrint/Cheezebee/Map/BossHunter/LobbyRoom?listen");
	GetWorld()->ServerTravel(URL, ETravelType::TRAVEL_Absolute);

}

