// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PlayerStat.h"
#include "BossHunterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ABossHunterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// Q Ability
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* QAction;

	// E Ability
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EAction;

	// R Ability
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RAction;

	// F Ability
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FAction;

	// Evasion Ability
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EvasionAction;

	// Normal Ability
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* NormalAction;

	// 임시 리스폰
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Respawn;

	// 인벤토리 사용 버튼 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Btn_1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Btn_2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Btn_3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Btn_4;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Btn_5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Btn_6;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Btn_7;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Btn_8;

public:
	ABossHunterCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> allPlayerWidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPlayerWidget* allPlayerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> allPlayerStoreWidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStoreWidget* allPlayerStoreWidget;

	class ATest_Boss* monster;
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }



public:
	// player
	UPROPERTY(BlueprintReadWrite)
	class UPlayerFSM* playerFSM;

	virtual void Ability_Q_Action();
	virtual void Ability_E_Action();
	virtual void Ability_R_Action();
	virtual void Ability_R_End_Action();
	virtual void Ability_F_Action();
	virtual void Ability_Ev_Action();
	virtual void Ability_Normal_Action();
	virtual void Ability_Respawn();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetState(playerSkillSet state);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SetState(playerSkillSet state);

	virtual void Q_Action();
	virtual void E_Action();
	virtual void R_Action();
	virtual void F_Action();
	virtual void Ev_Action();
	virtual void Normal_Action();

	virtual void Inventory_Btn(int32 index);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UPlayerStat* playerStat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	Fstat statment;

	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeStateMent(Fstat newStat);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ChangeStateMent(Fstat newStat);


public:
	UPROPERTY(EditAnywhere)
	int32 gold = 100;
	UPROPERTY(EditAnywhere)
	TArray<int32> itemIndexArray;

	class ABossRoomGameStateBase* gamestate;
	UFUNCTION()
	void ChangeStat();
	UFUNCTION()
	void ChangeStatByActiveItem(int32 itemindex);
	UFUNCTION()
	void ResetStatFromActiveItem(FTimerHandle delayHandle);
	bool bIsActiveItemUse = false;
	virtual void CoolTimePointChange();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttacked = false;

	UFUNCTION(Server, Reliable)
	void ServerRPC_hv(float hvh, float hvv);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_hv(float hvh, float hvv);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float h = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float v = 0;

	UFUNCTION(Server, Reliable)
	void ServerRPC_Roll(FRotator roll);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Roll(FRotator roll);

public:
	float origin_fullHP = 300;
	float origin_fullMP = 300;
	float origin_AttackPoint = 10;
	float origin_DefencePoint = 5;
	float currTime = 0;
	bool viewAllPlayerWidget = 0;
};

