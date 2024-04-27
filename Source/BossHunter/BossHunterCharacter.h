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

	virtual void Q_Action();
	virtual void E_Action();
	virtual void R_Action();
	virtual void F_Action();
	virtual void Ev_Action();
	virtual void Normal_Action();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UPlayerStat* playerStat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	Fstat statment;

public:
	UPROPERTY(EditAnywhere)
	int32 gold = 100;
	UPROPERTY(EditAnywhere)
	TArray<int32> itemIndexArray;
	UPROPERTY()
	class ABossRoomGameStateBase* gamestate;
	UFUNCTION()
	void ChangeStat();
	virtual void CoolTimePointChange();

};

