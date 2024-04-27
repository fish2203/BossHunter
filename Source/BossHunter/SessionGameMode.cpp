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

	// lobby widget 만들어서 viewport 에 붙히자
	//#include "SessionWidget.h"
	//게임모드의 sessionWidgetFactory에 wbp_sessionWidget:
	
	sessionWidget = CreateWidget<USessionWidget>(GetWorld(), sessionWidgetFactory);
	sessionWidget->AddToViewport();

	// 마우스 나오게 하자
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	pc->SetShowMouseCursor(true);
}

