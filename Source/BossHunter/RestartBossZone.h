// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RestartBossZone.generated.h"

/**
 * 
 */
UCLASS()
class BOSSHUNTER_API URestartBossZone : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
//전역변수
public:
	//다시시작버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_restartBossZone;
	//취소버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Cancel;
	//보스존 게이트
	UPROPERTY()
	class AGateBattleBoss* gatebattleBoss;
	UPROPERTY()
	class ABossHunterGameMode* bhGameMode;

//함수
public:
	UFUNCTION()
	void OnClickRestartBossZone();
	UFUNCTION()
	void OnClickCancel();
	

	
};
