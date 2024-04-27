// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionGameMode.h"
#include "SessionWidget.h"

ASessionGameMode::ASessionGameMode()
{
 ConstructorHelpers::FClassFinder<USessionWidget>tempWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BulePrint/Cheezebee/Widget/WBP_SessionWidget.WBP_SessionWidget_C'"));

 if (tempWidget.Succeeded())
 {
	 sessionWidgetFactory = tempWidget.Class;
 }


}


void ASessionGameMode::BeginPlay()
{
	Super::BeginPlay();

	// lobby widget ���� viewport �� ������
	//#include "SessionWidget.h"
	//���Ӹ���� sessionWidgetFactory�� wbp_sessionWidget:
	
	sessionWidget = CreateWidget<USessionWidget>(GetWorld(), sessionWidgetFactory);
	sessionWidget->AddToViewport();

	// ���콺 ������ ����
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	pc->SetShowMouseCursor(true);
}

