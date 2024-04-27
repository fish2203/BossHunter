// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOSSHUNTER_API USessionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	// game instance ���� ����
	UPROPERTY()
	class UNetGameInstance* gi;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* widgetSwitcher;

	// ����ȭ��
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_MoveCreateSession;
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_MoveSearchSession;


	UFUNCTION()
	void OnClickMoveCreateSession();
	UFUNCTION()
	void OnClickMoveSearchSession();

	// ���� ���� ȭ��
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* edit_SessionName;
	UPROPERTY(meta = (BindWidget))
	class USlider* slider_PlayerCount;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* text_PlayerCount;
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_CreateSession;
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_Exit_CreateSession;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* text_CreateSession;

	UFUNCTION()
	void OnValueChanged(float Value);
	UFUNCTION()
	void OnClickCreateSession();

	
	//���� �˻� // ���� ���� ȭ��
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_FindSession;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* scroll_RoomList;
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_Exit_RoomList;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* text_FindSession;

	//���������� ������ ���� ���丮 �Լ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class USessionInfoWidget> sessionInfoWidgetFactory;
	

public:
	UFUNCTION()
	void OnClickFindSession();

	// ���� �˻� �Ϸ�Ǹ� ȣ��Ǵ� �Լ�
	UFUNCTION()
	void OnSearchComplete(int32 idx, FString info);

	// ����ȭ������
	UFUNCTION()
	void OnClickExit();

	
};
