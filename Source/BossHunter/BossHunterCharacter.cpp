// Copyright Epic Games, Inc. All Rights Reserved.

#include "BossHunterCharacter.h"
#include <Net/UnrealNetwork.h>
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PlayerFSM.h"
#include "PlayerStat.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/UserWidget.h>
#include "PlayerWidget.h"
#include "Test_Boss.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include "StoreWidget.h"
#include "BossRoomGameStateBase.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetSwitcher.h>
#include "Blueprint/UserWidget.h"
#include "Components/SlateWrapperTypes.h"
#include "BossHunterGameMode.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ABossHunterCharacter

ABossHunterCharacter::ABossHunterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCamera->SetRelativeLocation(FVector(0, 0, 130));

	

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	playerFSM = CreateDefaultSubobject<UPlayerFSM>(TEXT("PlayerFSM"));
	playerStat = CreateDefaultSubobject<UPlayerStat>(TEXT("PlayerStat"));


	ConstructorHelpers::FClassFinder<UPlayerWidget> tempWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BulePrint/BP_PlayerWidget.BP_PlayerWidget_C'"));
	if (tempWidget.Succeeded())
	{
		allPlayerWidgetInstance = tempWidget.Class;
	}

	ConstructorHelpers::FClassFinder<UStoreWidget> tempStoreWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BulePrint/BP_StoreWidget.BP_StoreWidget_C'"));
	if (tempWidget.Succeeded())
	{
		allPlayerStoreWidgetInstance = tempStoreWidget.Class;
	}

	bReplicates = true;
	itemIndexArray.Empty();
}

void ABossHunterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//클라이언트 체력 동기화에 필요
	DOREPLIFETIME(ABossHunterCharacter, statment);
	//DOREPLIFETIME(ABossHunterCharacter, gold);
}

void ABossHunterCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	ABossHunterGameMode* gamemode = Cast<ABossHunterGameMode>(GetWorld()->GetAuthGameMode());
	allPlayerWidget = Cast<UPlayerWidget>(CreateWidget(GetWorld(), allPlayerWidgetInstance));

	//Add Input Mapping Context
	if (IsLocallyControlled()) {
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);

				// 스탯 위젯 띄우기
				if (IsValid(allPlayerWidgetInstance)) {
					//allPlayerWidget = Cast<UPlayerWidget>(CreateWidget(GetWorld(), allPlayerWidgetInstance));
					if (IsValid(allPlayerWidget)) {
						allPlayerWidget->SetOwningPlayer(PlayerController);
						allPlayerWidget->AddToViewport();
						PlayerController->bShowMouseCursor = true;
						//플레이어 위젯을 감추자.
						allPlayerWidget->SetVisibility(ESlateVisibility::Hidden);
					}
				}

				// 스토어 위젯 올리기
				if (IsValid(allPlayerStoreWidgetInstance)) {
					allPlayerStoreWidget = Cast<UStoreWidget>(CreateWidget(GetWorld(), allPlayerStoreWidgetInstance));
					if (IsValid(allPlayerStoreWidget)) {
						allPlayerStoreWidget->SetOwningPlayer(PlayerController);
						allPlayerStoreWidget->AddToViewport();
						allPlayerStoreWidget->WidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
					}
				}

				// 골드 값 받아오기
				gamestate = ABossRoomGameStateBase::Get();
				gold = gamestate->gold;
			}
			PlayerController->PlayerCameraManager->ViewPitchMin = -30;
			PlayerController->PlayerCameraManager->ViewPitchMax = 30;
			PlayerController->SetShowMouseCursor(true);
		}
	}

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("boss"), ActorsWithTag);
	for (AActor* Actor : ActorsWithTag)
	{
		if (Actor)
		{
			monster = Cast<ATest_Boss>(Actor);
		}
	}
}

void ABossHunterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(viewAllPlayerWidget == true) return;
	{
		currTime += GetWorld()->GetDeltaSeconds();
		if (IsLocallyControlled())
			if (currTime > 10)
			{
				allPlayerWidget->SetVisibility(ESlateVisibility::Visible);
				viewAllPlayerWidget = true;
				currTime = 0;
			}
	}
}

void ABossHunterCharacter::Ability_Q_Action()
{
	ServerRPC_SetState(playerSkillSet::Q_Skill);
}

void ABossHunterCharacter::Ability_E_Action()
{
	ServerRPC_SetState(playerSkillSet::E_Skill);
}

void ABossHunterCharacter::Ability_R_Action()
{
	ServerRPC_SetState(playerSkillSet::R_Skill);
}

void ABossHunterCharacter::Ability_R_End_Action(){}

void ABossHunterCharacter::Ability_F_Action()
{
	ServerRPC_SetState(playerSkillSet::F_Skill);
}

void ABossHunterCharacter::Ability_Ev_Action()
{
	ServerRPC_SetState(playerSkillSet::Evasion);
}

void ABossHunterCharacter::Ability_Normal_Action()
{
	ServerRPC_SetState(playerSkillSet::Plain);
}

void ABossHunterCharacter::Ability_Respawn()
{
	statment.healthPoint = statment.fullHP;
}

void ABossHunterCharacter::ServerRPC_SetState_Implementation(playerSkillSet state)
{
	MultiRPC_SetState(state);
}

void ABossHunterCharacter::MultiRPC_SetState_Implementation(playerSkillSet state)
{
	playerFSM->SetPlayerSkillState(state);
}

void ABossHunterCharacter::Q_Action(){}
void ABossHunterCharacter::E_Action(){}
void ABossHunterCharacter::R_Action(){}
void ABossHunterCharacter::F_Action(){}
void ABossHunterCharacter::Ev_Action(){}
void ABossHunterCharacter::Normal_Action(){}

void ABossHunterCharacter::Inventory_Btn(int32 index)
{
	if (itemIndexArray.Num() - 1 < index) return;
	FItem* item = gamestate->itemDataTable->FindRow<FItem>(*FString::FromInt(itemIndexArray[index]), TEXT(""));
	if (item->bIsActiveItem == true && bIsActiveItemUse == false || item->itemIndex >= 10)
	{
		ChangeStatByActiveItem(itemIndexArray[index]);
		itemIndexArray[index] = 0;
		allPlayerWidget->ChangeInventory();
	}
}

void ABossHunterCharacter::ServerRPC_ChangeStateMent_Implementation(Fstat newStat)
{
	MultiRPC_ChangeStateMent(newStat);
}

void ABossHunterCharacter::MultiRPC_ChangeStateMent_Implementation(Fstat newStat)
{
	statment = newStat;
}


void ABossHunterCharacter::ChangeStat()
{
	float _attackPoint = 1;
	float _defancePoint = 1;
	float _attackSpeedPoint = 1;
	float _moveSpeedPoint = 1;
	float _coolTimePoint = 1;
	float _fullHP = 1;
	float _fullMP = 1;

	gamestate = ABossRoomGameStateBase::Get();

	for (int32 itemindex : itemIndexArray)
	{
		FItem* item = gamestate->itemDataTable->FindRow<FItem>(*FString::FromInt(itemindex), TEXT(""));
		if (item->bIsActiveItem == true) continue;
		_attackPoint += item->attackPoint;
		_defancePoint += item->defancePoint;
		_attackSpeedPoint += item->attackSpeedPoint;
		_moveSpeedPoint += item->moveSpeedPoint;
		_coolTimePoint += item->coolTimePoint;
		_fullHP += item->healthPoint;
		_fullMP += item->manaPoint;
	}

	if(bIsActiveItemUse == false)
		statment.attackPoint = origin_AttackPoint;
	if (bIsActiveItemUse == false)
		statment.defancePoint = origin_DefencePoint;
	statment.attackSpeedPoint = 1;
	statment.moveSpeedPoint = 500;
	statment.coolTimePoint = 1;
	statment.fullHP = origin_fullHP;
	statment.fullMP = origin_fullMP;

	statment.attackPoint *= _attackPoint;
	statment.defancePoint *= _defancePoint;
	statment.attackSpeedPoint *= _attackSpeedPoint;
	statment.moveSpeedPoint *= _moveSpeedPoint;
	statment.fullHP *= _fullHP;
	statment.fullMP *= _fullMP;
	statment.healthPoint += statment.fullHP * (_fullHP - 1);
	if (statment.healthPoint > statment.fullHP) statment.healthPoint = statment.fullHP;
	statment.manaPoint += statment.fullMP * (_fullMP - 1);
	if (statment.manaPoint > statment.fullMP) statment.manaPoint = statment.fullMP;
	statment.coolTimePoint *= _coolTimePoint;

	CoolTimePointChange();

	ServerRPC_ChangeStateMent(statment);
}

void ABossHunterCharacter::ChangeStatByActiveItem(int32 itemindex)
{
	FItem* item = gamestate->itemDataTable->FindRow<FItem>(*FString::FromInt(itemindex), TEXT(""));

	if (item->itemIndex == 10)
	{
		if (statment.healthPoint >= statment.fullHP) return;
		statment.healthPoint += 200;
		if (statment.healthPoint > statment.fullHP) statment.healthPoint = statment.fullHP;
		ServerRPC_ChangeStateMent(statment);
		return;
	}
	else if (item->itemIndex == 11)
	{
		if (statment.manaPoint >= statment.fullMP) return;
		statment.manaPoint += 200;
		if (statment.manaPoint > statment.fullMP) statment.manaPoint = statment.fullMP;
		ServerRPC_ChangeStateMent(statment);
		return;
	}


	bIsActiveItemUse = true;

	statment.attackPoint *= item->attackPoint;
	statment.defancePoint *= item->defancePoint;
	ServerRPC_ChangeStateMent(statment);
	//statment.attackSpeedPoint *= item->attackSpeedPoint;
	//statment.moveSpeedPoint *= item->moveSpeedPoint;
	//statment.healthPoint *= item->healthPoint;
	//statment.manaPoint *= item->manaPoint;
	//statment.coolTimePoint *= item->coolTimePoint;
	//statment.fullHP *= item->healthPoint;
	//statment.fullMP *= item->manaPoint;

	FTimerHandle delayHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ABossHunterCharacter::ResetStatFromActiveItem, delayHandle);
	GetWorld()->GetTimerManager().SetTimer(delayHandle, TimerDelegate, 300.0f, false);
}

void ABossHunterCharacter::ResetStatFromActiveItem(FTimerHandle delayHandle)
{
	bIsActiveItemUse = false;
	ChangeStat();

	GetWorld()->GetTimerManager().ClearTimer(delayHandle);
}

void ABossHunterCharacter::CoolTimePointChange(){}

void ABossHunterCharacter::ServerRPC_hv_Implementation(float hvh, float hvv)
{
	MultiRPC_hv(hvh, hvv);
}

void ABossHunterCharacter::MultiRPC_hv_Implementation(float hvh, float hvv)
{
	h = hvh;
	v = hvv;
}

void ABossHunterCharacter::ServerRPC_Roll_Implementation(FRotator roll)
{
	MultiRPC_Roll(roll);	
}

void ABossHunterCharacter::MultiRPC_Roll_Implementation(FRotator roll)
{
	if (!IsLocallyControlled())
	{
		FRotator rot = roll;
		rot.Pitch = 0;
		SetActorRotation(rot);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABossHunterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABossHunterCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABossHunterCharacter::Look);

		// Q Ability
		EnhancedInputComponent->BindAction(QAction, ETriggerEvent::Triggered, this, &ABossHunterCharacter::Ability_Q_Action);

		// E Ability
		EnhancedInputComponent->BindAction(EAction, ETriggerEvent::Triggered, this, &ABossHunterCharacter::Ability_E_Action);

		// R Ability
		EnhancedInputComponent->BindAction(RAction, ETriggerEvent::Started, this, &ABossHunterCharacter::Ability_R_Action);
		EnhancedInputComponent->BindAction(RAction, ETriggerEvent::Ongoing, this, &ABossHunterCharacter::Ability_R_Action);
		EnhancedInputComponent->BindAction(RAction, ETriggerEvent::Completed, this, &ABossHunterCharacter::Ability_R_End_Action);

		// F Ability
		EnhancedInputComponent->BindAction(FAction, ETriggerEvent::Triggered, this, &ABossHunterCharacter::Ability_F_Action);

		// Evasion Ability
		EnhancedInputComponent->BindAction(EvasionAction, ETriggerEvent::Triggered, this, &ABossHunterCharacter::Ability_Ev_Action);

		// Normal Ability
		EnhancedInputComponent->BindAction(NormalAction, ETriggerEvent::Started, this, &ABossHunterCharacter::Ability_Normal_Action);

		// 임시 리스폰
		EnhancedInputComponent->BindAction(Respawn, ETriggerEvent::Triggered, this, &ABossHunterCharacter::Ability_Respawn);

		// 인벤토리 사용 버튼
		EnhancedInputComponent->BindAction(Btn_1, ETriggerEvent::Started, this, &ABossHunterCharacter::Inventory_Btn, 0);
		EnhancedInputComponent->BindAction(Btn_2, ETriggerEvent::Started, this, &ABossHunterCharacter::Inventory_Btn, 1);
		EnhancedInputComponent->BindAction(Btn_3, ETriggerEvent::Started, this, &ABossHunterCharacter::Inventory_Btn, 2);
		EnhancedInputComponent->BindAction(Btn_4, ETriggerEvent::Started, this, &ABossHunterCharacter::Inventory_Btn, 3);
		EnhancedInputComponent->BindAction(Btn_5, ETriggerEvent::Started, this, &ABossHunterCharacter::Inventory_Btn, 4);
		EnhancedInputComponent->BindAction(Btn_6, ETriggerEvent::Started, this, &ABossHunterCharacter::Inventory_Btn, 5);
		EnhancedInputComponent->BindAction(Btn_7, ETriggerEvent::Started, this, &ABossHunterCharacter::Inventory_Btn, 6);
		EnhancedInputComponent->BindAction(Btn_8, ETriggerEvent::Started, this, &ABossHunterCharacter::Inventory_Btn, 7);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ABossHunterCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	h = MovementVector.X;
	v = MovementVector.Y;
	ServerRPC_hv(h, v);

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		//const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector ForwardDirection = GetActorForwardVector() * v;
	
		// get right vector 
		//const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		const FVector RightDirection = GetActorRightVector() * h;

		FVector newdir = ForwardDirection + RightDirection;

		// add movement 
		//AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(newdir);
		//AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABossHunterCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
		FRotator r = FRotationMatrix::MakeFromX(FollowCamera->GetForwardVector()).Rotator();
		r.Pitch = 0;
		SetActorRotation(r);
		if (!HasAuthority())
		{
			ServerRPC_Roll(FRotationMatrix::MakeFromX(FollowCamera->GetForwardVector()).Rotator());
		}
		//ServerRPC_Roll(FRotationMatrix::MakeFromX(FollowCamera->GetForwardVector()).Rotator());
	}
}