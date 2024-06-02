// Fill out your copyright notice in the Description page of Project Settings.


#include "GateMoveLobby.h"
#include <Components/BoxComponent.h>
#include "MoveLoobyWidget.h"
#include "BossRoomGameStateBase.h"
#include <Kismet/GameplayStatics.h>
#include "BossHunterCharacter.h"

// Sets default values
AGateMoveLobby::AGateMoveLobby()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("box comp"));
	compBox -> SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void AGateMoveLobby::BeginPlay()
{
	Super::BeginPlay();

	//�������ϸ� �Լ�ȣ��
	compBox->OnComponentBeginOverlap.AddDynamic(this, &AGateMoveLobby::OnOverlap);


	movelobbyWidget = CreateWidget<UMoveLoobyWidget>(GetWorld(), movelobbyWidgetPackage);
	//�ٸ� ������ ��ġ�� ��ư�� �ȴ�����, zoder�� �ø���.
	movelobbyWidget->AddToViewport(1);
	//movewidgetPointer->SetVisibility(ESlateVisibility::Visible);
	movelobbyWidget->SetVisibility(ESlateVisibility::Hidden);
	
}

// Called every frame
void AGateMoveLobby::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ABossRoomGameStateBase* gamestate = ABossRoomGameStateBase::Get();
	TArray<AActor*> countArray;
	ABossHunterCharacter* player = Cast<ABossHunterCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	UGameplayStatics::GetAllActorsOfClass(this, player->GetClass(), countArray);
	gamestate->playerCount = countArray.Num();
}

void AGateMoveLobby::OnOverlap(UPrimitiveComponent* abc, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->HasAuthority() == false) return;
	//���� ���̰�����.
	//UMoveLoobyWidget
	movelobbyWidget->SetVisibility(ESlateVisibility::Visible);



}