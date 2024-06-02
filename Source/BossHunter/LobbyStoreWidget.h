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
	
	//������ �÷��̾� ��Ų���� ������Ʈ�� �̵�
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
	
	//���� ����ó
	UPROPERTY()
	class UWidgetSwitcher* WidgetSwitcher_LobbyStore;
	
	//�������
	UPROPERTY()
	class UWidgetSwitcher* WidgetSwitcher_MakeupCategory;

	//�ٹ̱�
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_MakeUp; 
	//�ٹ̱� �⺻
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_NormalSkin;
	//�ٹ̱� Ȯ��
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_AmorSkin;



	//�������
	UPROPERTY()
	class UButton* btn_BossUp;
	UPROPERTY()
	class UButton* btn_BattleItem;
	
	//���ھ� ����
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_Add;
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_Sub;

	
	//�������
	//��Ų���� �������Ʈ
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

	//���ھ� �ε��ϱ�
	void LoadPlayerScore();
	//���� �ݱ�
	UFUNCTION()
	void OnClickCloseWidget();
	//�����ϱ�
	UFUNCTION()
	void OnClickBuyItem();
	
	UFUNCTION()
	void OnClickSkinEquip();
	UFUNCTION(Server, Reliable)
	void ServerRPC_OnClickSkinEquip(int32 a, int32 b, int32 c);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_OnClickSkinEquip(int32 a, int32 b, int32 c);


	//���ؽ� ���ھ� ����
	void AddScore(int32 value);
	//��Ų �ε���
	void AddSkinIndex(int32 value);
	
	
	//�ٹ̱�
	UFUNCTION()
	void OnClickMakeUp();
	UFUNCTION()
	void OnClickMakeUPNormalSkin();
	void ChangePlayerMeshNormal();
	
	
	UFUNCTION()
	void OnClickMakeUPAmorSkin();
	void ChangePlayerMeshAmor();


	//������
	UFUNCTION()
	void OnClickBossUp();
	//��Ʋ������
	UFUNCTION()
	void OnClickBattleItem();
	//���ھ������Լ�
	UFUNCTION()
	void OnClickIncrease();
	UFUNCTION()
	void OnClickDecrease();


};
