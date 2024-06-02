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
	// 위젯에 띄울때 필요한 캐릭터들
	class ABossHunterCharacter* player;
	class AGunPlayer* gunPlayer;
	class ATest_Boss* monster;
	class UBossFsmTest* monsterFsm;
	class ABossHunterGameMode* gamemode;
	// 인벤토리에 필요한 객체들
	UPROPERTY()
	class ABossRoomGameStateBase* gamestate;
	UPROPERTY()
	class ABossHunterGameMode* bhGameMode;

	// 체력 및 마나
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* ManaBar;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* HPBarText;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* MPBarText;

	// 스탯 출력 텍스트
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* statText;

	//보스의 체력 프로그레스바
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* MonsterHP;

	//보스의 체력 텍스트
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* text_MonsterHP;

	// 스킬 버튼들
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


	// 스킬 설명을 띄우기 위해 hover option에 필요한 텍스트
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

	//다시하기 버튼
	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_restart;

	// 피 나오는 효과
	UPROPERTY(meta=(BindWidget))
	class UImage* BloodImg;

	// 피 나오는 효과에 필요한 opacity 값
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

	//다시하기 버튼
	UFUNCTION()
	void OnClickRestarGame();
	UPROPERTY()
	class AGateBattleBoss* gateBossZone;

};
