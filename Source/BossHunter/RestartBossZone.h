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
//��������
public:
	//�ٽý��۹�ư
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_restartBossZone;
	//��ҹ�ư
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Cancel;
	//������ ����Ʈ
	UPROPERTY()
	class AGateBattleBoss* gatebattleBoss;
	UPROPERTY()
	class ABossHunterGameMode* bhGameMode;

//�Լ�
public:
	UFUNCTION()
	void OnClickRestartBossZone();
	UFUNCTION()
	void OnClickCancel();
	

	
};
