// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerScoreWidget.h"
#include <Components/TextBlock.h>

void UPlayerScoreWidget::NativeConstruct()
{

}

void UPlayerScoreWidget::UpdateCurrScoreUI(int32 score)
{
	//현재 점수 : score
	FString strScore = FString::Printf(TEXT("현재 점수 : %d"), score);

	//UTextBlock
	//text_PlayerScore->SetText(FText::FromString(strScore));
}

void UPlayerScoreWidget::UpdateBestScoreUI(int32 bestScore)
{
	//UE_LOG(LogTemp, Warning, TEXT("2222"));

	//최고 점수 : bestScore
	FString strBestScore = FString::Printf(TEXT("%d"), bestScore);

	text_PlayerScore->SetText(FText::FromString(strBestScore));
}
