// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <Interfaces/OnlineSessionInterface.h>
#include "NetGameInstance.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_TwoParams(FSearchComplete, int32, FString);


UCLASS()
class BOSSHUNTER_API UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMoveBossZoneWidget> moveWidgetPackage;
	UPROPERTY()
	class UMoveBossZoneWidget* movewidgetPointer;
	
	// ���� ����� �Լ�
	UFUNCTION(BlueprintCallable)
	//void CreateMySession();
	//�Ű����� �߰�, ���̸�, �ִ��ο�
	void CreateMySession(FString roomName, int32 maxPlayer);
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	// ���� �ı��ϴ� �Լ�
	UFUNCTION(BlueprintCallable)
	void DestroyMySession();
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	// ������ �˻� �Լ�
	UFUNCTION(BlueprintCallable)
	void FindOtherSession();
	void OnFindSessionComplete(bool bWasSuccessful);

	// ���� ���� �Լ�
	//#include <Interfaces/OnlineSessionInterface.h>
	UFUNCTION(BlueprintCallable)
	void JoinOtherSession(int32 idx);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result);

	UFUNCTION(BlueprintCallable)
	void EnterOtherMap();
	UFUNCTION(Server,Reliable)
	void ServerRPC_EnterOtherMap();


	//��Ʈ�� ���ڴ�
	FString StringBase64Encode(FString str);
	FString StringBase64Decode(FString str);


public:
	// �� �������� (���� �����, ���� �˻�, ���� ����)
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> sessionInterface;

	// ���� �˻��� ���̴� Ŭ����
	TSharedPtr<class FOnlineSessionSearch> sessionSearch;

	// ���� �̸�
	FString mySessionName = TEXT("TestPlayer");

	// ���� �˻��� �Ϸ�Ǹ� ȣ���ؾ� �ϴ� Delegate
	FSearchComplete onSearchComplete;

};
	