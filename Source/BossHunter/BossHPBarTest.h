// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossHPBarTest.generated.h"

/**
 * 
 */
UCLASS()
class BOSSHUNTER_API UBossHPBarTest : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,  meta = (BindWidget))
	class UProgressBar* HPBar;

};
