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
	// game instance 담을 변수
	UPROPERTY()
	class UNetGameInstance* gi;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* widgetSwitcher;

	// 메인화면
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_MoveCreateSession;
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_MoveSearchSession;


	UFUNCTION()
	void OnClickMoveCreateSession();
	UFUNCTION()
	void OnClickMoveSearchSession();

	// 세션 생성 화면
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

	
	//세션 검색 // 세션 참여 화면
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_FindSession;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* scroll_RoomList;
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_Exit_RoomList;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* text_FindSession;

	//세션위젯의 정보를 담을 펙토리 함수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class USessionInfoWidget> sessionInfoWidgetFactory;
	

public:
	UFUNCTION()
	void OnClickFindSession();

	// 세션 검색 완료되면 호출되는 함수
	UFUNCTION()
	void OnSearchComplete(int32 idx, FString info);

	// 메인화면으로
	UFUNCTION()
	void OnClickExit();

	
};
