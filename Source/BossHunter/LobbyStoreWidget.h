// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyStoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOSSHUNTER_API ULobbyStoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	ULobbyStoreWidget(const FObjectInitializer& ObjectInitializer);


public:
	virtual void NativeConstruct() override;

public:
	int32 playerScore = 0;
	int32 widgetSkinIdx = 0;
	int32 onclickNormalSkin = 1;
	int32 onclickAmorSkin = 1;

	int32 ishaveNomalSkin = 0;
	int32 ishaveAmorSkin = 1;
	
	//변수를 플레이어 스킨으로 컴포넌트로 이동
// 	bool isnormalSkin = false;
// 	bool isamorSkin = false;

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* text_PlayerScore;
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_Cancel;
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_buy;
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_SkinEquip;
	
	//위젯 스위처
	UPROPERTY()
	class UWidgetSwitcher* WidgetSwitcher_LobbyStore;
	
	//삭제대기
	UPROPERTY()
	class UWidgetSwitcher* WidgetSwitcher_MakeupCategory;

	//꾸미기
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_MakeUp; 
	//꾸미기 기본
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_NormalSkin;
	//꾸미기 확장
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_AmorSkin;



	//삭제대기
	UPROPERTY()
	class UButton* btn_BossUp;
	UPROPERTY()
	class UButton* btn_BattleItem;
	
	//스코어 증감
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_Add;
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_Sub;

	
	//삭제대기
	//스킨위젯 블루프린트
	UPROPERTY()
	class ULobbyStoreMakeUpSkinWidget* wbp_skinWidgetEX;
	UPROPERTY()
	class ULobbyStoreMakeUpSkinWidget* wbp_skinWidgetNormal;

	UPROPERTY()
	TArray<class USkeletalMesh*> compMesh;


	UPROPERTY()
	TArray<class UTexture2D*> texture2D;
	/*	UPROPERTY(meta = (BindWidget))
	   class UImage* image_MakeUp;*/
	
	UPROPERTY(meta = (BindWidget))
	class UImage* image_SkinPreView;
	UPROPERTY(meta = (BindWidget))
	class UImage* image_ClickNormal;
	UPROPERTY(meta = (BindWidget))
	class UImage* image_ClickAmor;
	

public:
	UPROPERTY()
	class AGunPlayer* player;
	UPROPERTY()
	class UPlayerScoreComp* playerScoreComp;
	UPROPERTY()
	class UPlayerScoreWidget* playerScoreWidget;


public:
	void SetTextureImage();

	//스코어 로드하기
	void LoadPlayerScore();
	//위젯 닫기
	UFUNCTION()
	void OnClickCloseWidget();
	//구매하기
	UFUNCTION()
	void OnClickBuyItem();
	
	UFUNCTION()
	void OnClickSkinEquip();
	UFUNCTION(Server, Reliable)
	void ServerRPC_OnClickSkinEquip(int32 a, int32 b, int32 c);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_OnClickSkinEquip(int32 a, int32 b, int32 c);


	//구해시 스코어 증감
	void AddScore(int32 value);
	//스킨 인덱스
	void AddSkinIndex(int32 value);
	
	
	//꾸미기
	UFUNCTION()
	void OnClickMakeUp();
	UFUNCTION()
	void OnClickMakeUPNormalSkin();
	void ChangePlayerMeshNormal();
	
	
	UFUNCTION()
	void OnClickMakeUPAmorSkin();
	void ChangePlayerMeshAmor();


	//보스업
	UFUNCTION()
	void OnClickBossUp();
	//배틀아이템
	UFUNCTION()
	void OnClickBattleItem();
	//스코어증감함수
	UFUNCTION()
	void OnClickIncrease();
	UFUNCTION()
	void OnClickDecrease();


};
