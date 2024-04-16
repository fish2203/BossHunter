// Copyright Epic Games, Inc. All Rights Reserved.

#include "BossHunterCharacter.h"
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
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
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
}

void ABossHunterCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			if (IsValid(allPlayerWidgetInstance)) {
				allPlayerWidget = Cast<UPlayerWidget>(CreateWidget(GetWorld(), allPlayerWidgetInstance));
				if (IsValid(allPlayerWidget)) {
					allPlayerWidget->SetOwningPlayer(PlayerController);
					allPlayerWidget->AddToViewport();
					PlayerController->bShowMouseCursor = true;
				}
			}
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

void ABossHunterCharacter::Ability_Q_Action()
{
	playerFSM->SetPlayerSkillState(playerSkillSet::Q_Skill);
}

void ABossHunterCharacter::Ability_E_Action()
{
	playerFSM->SetPlayerSkillState(playerSkillSet::E_Skill);
}

void ABossHunterCharacter::Ability_R_Action()
{
	playerFSM->SetPlayerSkillState(playerSkillSet::R_Skill);
}

void ABossHunterCharacter::Ability_R_End_Action()
{

}

void ABossHunterCharacter::Ability_F_Action()
{
	playerFSM->SetPlayerSkillState(playerSkillSet::F_Skill);
}

void ABossHunterCharacter::Ability_Ev_Action()
{
	playerFSM->SetPlayerSkillState(playerSkillSet::Evasion);
}

void ABossHunterCharacter::Ability_Normal_Action()
{
	playerFSM->SetPlayerSkillState(playerSkillSet::Plain);
}

void ABossHunterCharacter::Q_Action()
{
	
}

void ABossHunterCharacter::E_Action()
{

}

void ABossHunterCharacter::R_Action()
{

}

void ABossHunterCharacter::F_Action()
{

}

void ABossHunterCharacter::Ev_Action()
{

}

void ABossHunterCharacter::Normal_Action()
{

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
		EnhancedInputComponent->BindAction(NormalAction, ETriggerEvent::Triggered, this, &ABossHunterCharacter::Ability_Normal_Action);

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

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
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

		
	}
}