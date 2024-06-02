// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOSSHUNTER_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	// ������ ��ﶧ �ʿ��� ĳ���͵�
	class ABossHunterCharacter* player;
	class AGunPlayer* gunPlayer;
	class ATest_Boss* monster;
	class UBossFsmTest* monsterFsm;
	class ABossHunterGameMode* gamemode;
	// �κ��丮�� �ʿ��� ��ü��
	UPROPERTY()
	class ABossRoomGameStateBase* gamestate;
	UPROPERTY()
	class ABossHunterGameMode* bhGameMode;

	// ü�� �� ����
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* ManaBar;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* HPBarText;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* MPBarText;

	// ���� ��� �ؽ�Ʈ
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* statText;

	//������ ü�� ���α׷�����
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* MonsterHP;

	//������ ü�� �ؽ�Ʈ
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* text_MonsterHP;

	// ��ų ��ư��
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* SkillNameText;
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* Passive_Btn;
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* Q_Btn;
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* E_Btn;
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* F_Btn;
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* R_Btn;
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* SP_Btn;


	// ��ų ������ ���� ���� hover option�� �ʿ��� �ؽ�Ʈ
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Q_Text;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* E_Text;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* F_Text;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* R_Text;
	//UPROPERTY(meta=(BindWidget))
	//class UTextBlock* SP_Text;

	//�ٽ��ϱ� ��ư
	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_restart;

	// �� ������ ȿ��
	UPROPERTY(meta=(BindWidget))
	class UImage* BloodImg;

	// �� ������ ȿ���� �ʿ��� opacity ��
	float bloodOpacity;

	// Inventory
	UPROPERTY(meta=(BindWidget))
	class UImage* slot_1;
	UPROPERTY(meta=(BindWidget))
	class UImage* slot_2;
	UPROPERTY(meta=(BindWidget))
	class UImage* slot_3;
	UPROPERTY(meta=(BindWidget))
	class UImage* slot_4;
	UPROPERTY(meta=(BindWidget))
	class UImage* slot_5;
	UPROPERTY(meta=(BindWidget))
	class UImage* slot_6;
	UPROPERTY(meta=(BindWidget))
	class UImage* slot_7;
	UPROPERTY(meta=(BindWidget))
	class UImage* slot_8;
	TArray<UImage*> inventory;
	UFUNCTION()
	void ChangeInventory();

	//�ٽ��ϱ� ��ư
	UFUNCTION()
	void OnClickRestarGame();
	UPROPERTY()
	class AGateBattleBoss* gateBossZone;

};
