// Fill out your copyright notice in the Description page of Project Settings.


#include "BossWidgetComponent.h"
#include "MoveLoobyWidget.h"

// Sets default values for this component's properties
UBossWidgetComponent::UBossWidgetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UBossWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	//UMoveLoobyWidget
	//보스의 BP로 가서 movelobbyWidgetPackage를 BP_clearwidget으로 설정
	movelobbyWidget = CreateWidget<UMoveLoobyWidget>(GetWorld(), movelobbyWidgetPackage);
	movelobbyWidget->AddToViewport(1);
	movelobbyWidget->SetVisibility(ESlateVisibility::Hidden);
	//movelobbyWidget->SetVisibility(ESlateVisibility::Visible);



}


// Called every frame
void UBossWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBossWidgetComponent::ViewBossClearWidget()
{
	movelobbyWidget->SetVisibility(ESlateVisibility::Visible);
}
