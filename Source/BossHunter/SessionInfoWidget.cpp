// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionInfoWidget.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include "NetGameInstance.h"

void USessionInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//<Components/Button.h>
	btn_JoinSession->OnClicked.AddDynamic(this, &USessionInfoWidget::OnClickJoinSession);
}

void USessionInfoWidget::OnClickJoinSession()
{	//#include "NetGameInstance.h"
	//auto gi = Cast<UNetGameInstance>(GetGameInstance());
	UNetGameInstance* gi = Cast<UNetGameInstance>(GetGameInstance());
	gi->JoinOtherSession(sessionIdx);
}

void USessionInfoWidget::SetInfo(int32 idx, FString info)
{
	// �� ��° �������� ��Ƴ���
	sessionIdx = idx;
	// ȭ�鿡 ���̴� ���� ����
	text_SessionInfo->SetText(FText::FromString(info));
	//UTextBlock
	// ȭ�鿡 ���̴� ���� ����
	text_SessionInfo->SetText(FText::FromString(info));
}
