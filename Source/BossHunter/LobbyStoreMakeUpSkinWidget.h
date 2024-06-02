// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyStoreMakeUpSkinWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOSSHUNTER_API ULobbyStoreMakeUpSkinWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	ULobbyStoreMakeUpSkinWidget(const FObjectInitializer& ObjectInitializer);

public:
	virtual void NativeConstruct() override;

	
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_WearSkin;
	UPROPERTY()
	TArray<UTexture2D*> texture2D;
	UPROPERTY(meta = (BindWidget))
	class UImage* image_MakeUp;
	UPROPERTY()
	TArray<class USkeletalMesh*> compMesh;
	UPROPERTY()
	class ULobbyStoreWidget* lobbyStoreWidget;

public:
	UFUNCTION()
	void OnClickWearSkin();

	//�⺻��Ų ����
	//����	int32
	//���뿩�� bool
	//���ſ��� bool
	//�̹��� UImage

	//�߰���Ų����
	//����	int32
	//���뿩�� bool
	//���ſ��� bool
	//�̹��� UImage


};
