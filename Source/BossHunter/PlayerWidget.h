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
	class ABossHunterCharacter* player;
	class AGunPlayer* gunPlayer;
	class ATest_Boss* monster;
	class UBossFsmTest* monsterFsm;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* ManaBar;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* HPBarText;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* MPBarText;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* statText;
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* MonsterHP;
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* Q_Btn;
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* E_Btn;
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* F_Btn;
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* R_Btn;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ExplainText;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Q_Text;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* E_Text;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* F_Text;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* R_Text;

	UFUNCTION(BlueprintCallable)
	void DamageProgressBoss(float damage);


};
