// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameInstance.h"
#include <OnlineSubsystem.h>
//#include <Interfaces/OnlineSessionInterface.h>
#include <OnlineSessionSettings.h>
#include <Online/OnlineSessionNames.h>
#include "MoveBossZoneWidget.h"
#include "PlayerScoreWidget.h"

UNetGameInstance::UNetGameInstance()
{
	//�÷��̾� ���ھ� ������ ����
	//UPlayerScoreWidget
	/*ConstructorHelpers::FClassFinder<UPlayerScoreWidget> tempPlayerScoreWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BulePrint/Cheezebee/Widget/WBP_PlayerScoreWidget.WBP_PlayerScoreWidget_C'"));
	if (tempPlayerScoreWidget.Succeeded())
	{
		playerScoreUIPakage = tempPlayerScoreWidget.Class;
	}*/

}

// OnlineSessionInterface ���ؼ� �Ѵ�.
// ������ �����. 
// ������ �˻�
// ������ ����


void UNetGameInstance::Init()
{
	Super::Init();

	// �¶��� ���� �ý��� ��������
	//#include <OnlineSubsystem.h>
	IOnlineSubsystem* subsys = IOnlineSubsystem::Get();
	if (subsys)
	{
		// ���� �������̽� ��������
		sessionInterface = subsys->GetSessionInterface();
		//#include <Interfaces/OnlineSessionInterface.h>
		//���������
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnCreateSessionComplete);
		//���������
		sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnDestroySessionComplete);
		//����ã��
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNetGameInstance::OnFindSessionComplete);
		//��������
		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnJoinSessionComplete);
	}

	//���������� �̵��ϴ� ����
	movewidgetPointer = CreateWidget<UMoveBossZoneWidget>(GetWorld(), moveWidgetPackage);
	movewidgetPointer->AddToViewport(1);


	//movewidgetPointer->SetVisibility(true);
	//movebosszoneWidget->SetVisibility(false);

	//�÷��̾� ���ھ� ������ ������
	//UPlayerScoreWidget
	/*playerScoreUI = CreateWidget<UPlayerScoreWidget>(GetWorld(), playerScoreUIPakage);
	playerScoreUI->AddToViewport(1);
	playerScoreUI->SetVisibility(ESlateVisibility::Visible);*/
}

void UNetGameInstance::CreateMySession(FString roomName, int32 maxPlayer)
{
	//#include <OnlineSessionSettings.h>
	FOnlineSessionSettings sessionSettings;

	// true ������ �˻� �ȴ�.
	sessionSettings.bShouldAdvertise = true;

	// steam ����ϸ� �ش� �ɼ��� true ������ ���� �� �ִ�.
	sessionSettings.bUseLobbiesIfAvailable = true;

	// ���� �������� �ƴ����� �����ٰ���
	sessionSettings.bUsesPresence = true;
	// ���� �÷��� �߿� ������ �� �ְ�
	sessionSettings.bAllowJoinInProgress = true;
	sessionSettings.bAllowJoinViaPresence = true;

	// �ο� �� 
	//sessionSettings.NumPublicConnections = 10;
	sessionSettings.NumPublicConnections = maxPlayer;
	UE_LOG(LogTemp, Warning, TEXT("origin : %s"), *roomName);

	// base64�� Encode
	roomName = StringBase64Encode(roomName);
	sessionSettings.Set(FName("ROOM_NAME"), roomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	

	// ���� ���� ��û
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	//???
	int32 rand = FMath::RandRange(1, 100000);
	mySessionName += FString::Printf(TEXT("%d"), rand);
	//��������, �ݾ��̵�, �������̸�
	sessionInterface->CreateSession(*netID, FName(mySessionName), sessionSettings);
}

void UNetGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete Success -- %s"), *SessionName.ToString());

		//GetWorld()->ServerTravel(TEXT("/Game/ThirdPerson/Maps/BattleMap?listen"));
		//�κ������ �̵�
		GetWorld()->ServerTravel(TEXT("/Game/BulePrint/Cheezebee/Map/BossHunter/LobbyRoom?listen"));
	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete Fail"));
	}
}

void UNetGameInstance::DestroyMySession()
{
	//sessionInterface->DestroySession(FName(mySessionName));
	//�����̸��� �����ͷ� ��������.
	sessionInterface->DestroySession(FName(mySessionName));

}

void UNetGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionComplete Success -- %s"), *SessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionComplete Fail"));
	}
}

void UNetGameInstance::FindOtherSession()
{

	sessionSearch = MakeShared<FOnlineSessionSearch>();

	//#include <Online/OnlineSessionNames.h>
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	//���� �ִ������ο�
	sessionSearch->MaxSearchResults = 10;

	// ���� �˻� ��û
	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
}

void UNetGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		auto results = sessionSearch->SearchResults;
		UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete Success - count : %d"), results.Num());


		for (int32 i = 0; i < results.Num(); i++)
		{
			FOnlineSessionSearchResult si = results[i];
			//���̸� ���� ����
			FString roomName;
			si.Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
			
			// ���� ���� ---> String ���� 
			// ������ �ִ� �ο�
			int32 maxPlayer = si.Session.SessionSettings.NumPublicConnections;
			// ������ ���� �ο� (�ִ� �ο� - ���� �ο�)
			int32 currPlayer = maxPlayer - si.Session.NumOpenPublicConnections;

			//���̸� ���ڵ�
			roomName = StringBase64Decode(roomName);
			
			// ���̸� ( 5 / 10 )
			FString sessionInfo = FString::Printf(
				TEXT("%s ( %d / %d )"),
				*roomName, currPlayer, maxPlayer);

			onSearchComplete.ExecuteIfBound(i, sessionInfo);

		}

		// idx �� -1 �����ؼ� �˻� �Ϸ� �˷�����
		//onSearchComplete.ExecuteIfBound(-1, TEXT(""));

		if (results.Num() > 0)
		{
			JoinOtherSession(0);
		}

		/*for (auto si : results)
		{
			FString roomName;
			si.Session.SessionSettings.Get(FName(TEXT("ROOM_NAME")), roomName);
		}*/

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete Fail"));
	}
}


void UNetGameInstance::JoinOtherSession(int32 idx)
{
	//TArray<FOnlineSessionSearchResult> 
	auto results = sessionSearch->SearchResults;
	
	if (sessionInterface == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("sessionInterface is null"));
	}
	if (results.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("results Zero"));

	}
	UE_LOG(LogTemp, Warning, TEXT("results count : %d, idx : %d"), results.Num(), idx);
	
	
	//���ǹ�ȣ
	sessionInterface->JoinSession(0, FName(mySessionName), results[idx]);

}

void UNetGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result)
{
	if (result == EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete Success : %s"), *SessionName.ToString());
		FString url;
		// �����ؾ� �ϴ� Listen ���� URL�� �޾� ����
		sessionInterface->GetResolvedConnectString(SessionName, url);
		UE_LOG(LogTemp, Warning, TEXT("Join session URL : %s"), *url);

		if (!url.IsEmpty())
		{
			// �ش� URL �� ��������
			APlayerController* pc = GetWorld()->GetFirstPlayerController();
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete Fail : %d"), result);
	}
}


void UNetGameInstance::EnterOtherMap()
{
	ServerRPC_EnterOtherMap();
}

void UNetGameInstance::ServerRPC_EnterOtherMap_Implementation()
{
	FString URL = TEXT("/Game/BulePrint/Cheezebee/Map/BossHunter/BossZone?listen");
	GetWorld()->ServerTravel(URL, ETravelType::TRAVEL_Absolute);
}

FString UNetGameInstance::StringBase64Encode(FString str)
{
	// Set �� �� : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 �� Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()), utf8String.length());
	return FBase64::Encode(arrayData);
}

FString UNetGameInstance::StringBase64Decode(FString str)
{
	// Get �� �� : base64 �� Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(str, arrayData);
	std::string ut8String((char*)(arrayData.GetData()), arrayData.Num());
	return UTF8_TO_TCHAR(ut8String.c_str());
}