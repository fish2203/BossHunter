// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossWidgetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOSSHUNTER_API UBossWidgetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossWidgetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	bool iscreate = false;


public:
	UPROPERTY()
	class UMoveLoobyWidget* movelobbyWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMoveLoobyWidget> movelobbyWidgetPackage;

	


public:
	UFUNCTION()
	void ViewBossClearWidget();

		
};
