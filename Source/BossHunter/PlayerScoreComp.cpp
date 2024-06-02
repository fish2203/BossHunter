// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerScoreComp.h"
#include <Kismet/GameplayStatics.h>
#include "SavePlayerScore.h"
#include "GunPlayer.h"
#include "PlayerScoreWidget.h"
#include "NetGameInstance.h"

// Sets default values for this component's properties
UPlayerScoreComp::UPlayerScoreComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// UPlayerScoreWidget 클래스 가져오자
	ConstructorHelpers::FClassFinder<UPlayerScoreWidget> tempPlayerScoreWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BulePrint/Cheezebee/Widget/WBP_PlayerScoreWidget.WBP_PlayerScoreWidget_C'"));
	if (tempPlayerScoreWidget.Succeeded())
	{
		playerScorePakage = tempPlayerScoreWidget.Class;
	}

	




}


// Called when the game starts
void UPlayerScoreComp::BeginPlay()
{
	Super::BeginPlay();

	//AActor* findPlayer = UGameplayStatics::GetActorOfClass(GetWorld(), AGunPlayer::StaticClass());
	
	gunPlayer = Cast<AGunPlayer>(GetOwner());
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();


	//위젯을 생성
	//UPlayerScoreWidget
	playerScoreUI = CreateWidget<UPlayerScoreWidget>(GetWorld(), playerScorePakage);
	//위젯을 붙인다.
	
	if(gunPlayer->IsLocallyControlled())
	{
		playerScoreUI->AddToViewport(0);

	}
	
	// 최고 점수 불러오자
	LoadData();

	//playerScoreUI->SetOwningLocalPlayer(LocalPlayer);
	//UE_LOG(LogTemp, Warning, TEXT(" 111111111111111111"));
	//playerScoreUI->SetVisibility(ESlateVisibility::Hidden);
	
	/*{
			playerScoreUI->RemoveFromViewport();
			UE_LOG(LogTemp, Warning, TEXT(" 2222222222222"));
	}*/
	
	

	//
	/*AActor* findPlayer = UGameplayStatics::GetActorOfClass(GetWorld(), AGunPlayer::StaticClass());
	gunPlayer = Cast<AGunPlayer>(findPlayer);*/

	//UGameInstance* findInst = UGameplayStatics::GetGameInstance(GetWorld());
	//netGameInstance= Cast<UNetGameInstance>(findInst);
}


// Called every frame
void UPlayerScoreComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerScoreComp::AddScore(int32 value)
{
	// 현재점수를 누적시키다
	currScore += value;
	
	if (currScore <= bestSocre)
	{
		//만약 최고점수가 더 크거나 같다면 현재점수를 최고점수에 더함.
		bestSocre += currScore;
	}

	// 만약에 현재점수가 최고점수보다 크다면
	else if (currScore >= bestSocre) 
	{
			// 최고점수를 현재점수로 갱신
			bestSocre = currScore;

	}

		//베스트점수와 현재점수 출력
		UE_LOG(LogTemp, Warning, TEXT("best%d = curr%d"), bestSocre, currScore);

		// 현재점수 UI를 갱신
		playerScoreUI->UpdateCurrScoreUI(currScore);
		// 최고점수 UI 를 갱신
		playerScoreUI->UpdateBestScoreUI(bestSocre);
		// 최고점수 저장
		SaveData();
	
}

void UPlayerScoreComp::SaveData()
{
	// bestScore -> CSaveData 에 있는 saveBestScore 변수에 넣는다.

	//UGameplayStatics
	//USavePlayerScore
	// 1. USavePlayerScore 를 하나 생성한다. (USaveGame*)
	USaveGame* saveGame = UGameplayStatics::CreateSaveGameObject(USavePlayerScore::StaticClass());
	// 2. 생성한 놈을 CSaveData 로 형변환
	USavePlayerScore* saveData = Cast<USavePlayerScore>(saveGame);
	// 3. saveBestScore = bestScore
	saveData->saveBestScore = bestSocre;
	// 4. USavePlayerScore 을 저장
	UGameplayStatics::SaveGameToSlot(saveData, TEXT("BEST_SCORE"), 0);
}

void UPlayerScoreComp::LoadData()
{
	// 1. "BEST_SCORE" 으로 되어 있는 데이터를 불러온다. (USaveGame*)
	USaveGame* loadGame = UGameplayStatics::LoadGameFromSlot(TEXT("BEST_SCORE"), 0);
	// 2. 만약에 잘 읽어왔다면 (저장한 값이 있었다면)
	if (loadGame != nullptr)
	{
		// 3. 불러온 객체를 UCSaveData 로 형변환
		USavePlayerScore* loadData = Cast<USavePlayerScore>(loadGame);
		// 4. bestScore = saveBestScore
		bestSocre = loadData->saveBestScore;
		// 5. 최고 점수 UI 갱신
		playerScoreUI->UpdateBestScoreUI(bestSocre);
		UE_LOG(LogTemp, Warning, TEXT("Load Best %d"), bestSocre);

	}
}

void UPlayerScoreComp::ShowSCoreUI()
{

}

