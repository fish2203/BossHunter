// Fill out your copyright notice in the Description page of Project Settings.


#include "GunPlayer.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Public/KismetTraceUtils.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h>
#include "Test_Boss.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SkeletalMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/SkeletalMeshSocket.h>
#include <HAL/PlatformProcess.h>
#include "Components/ActorComponent.h"
#include "GunPlayer_Bomb.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/ActorComponent.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include "GunPlayer_CallBomb.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Particles/ParticleSystem.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Materials/Material.h>

AGunPlayer::AGunPlayer()
{
	statment.attackPoint = 10;
	statment.defancePoint = 5;
	statment.attackSpeedPoint = 1.0f;
	statment.moveSpeedPoint = 500;
	//Player Curr HP
	statment.healthPoint = 300;
	statment.manaPoint = 100;
	statment.coolTimePoint = 1.0f;

	statment.fullHP = statment.healthPoint;
	statment.fullMP = statment.manaPoint;

	GetCharacterMovement()->MaxWalkSpeed = statment.moveSpeedPoint;

	positionComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Position"));
	positionComp->SetupAttachment(RootComponent);

	bombWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("BombWidget"));
	bombWidget->SetupAttachment(RootComponent);
	bombWidget->SetVisibility(false);
}

void AGunPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGunPlayer, timeLimit);
	DOREPLIFETIME(AGunPlayer, lazerTime);

}

void AGunPlayer::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocallyControlled()){
		positionComp->SetVisibility(false);
	}

	curr_E = cool_E;
	curr_F = cool_F;
	curr_Q = cool_Q;
	curr_R = cool_R;

	gold = 200;
}

void AGunPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (statment.healthPoint <= 0) {
		bIsDie = true;
		GetCharacterMovement()->MaxAcceleration = 0;
		GetCharacterMovement()->MaxWalkSpeed = 0;
		bIsAttack = true;
		return;
	}

	curr_E += DeltaTime;
	curr_R += DeltaTime;
	curr_Q += DeltaTime;
	curr_F += DeltaTime;
	
	GetCharacterMovement()->MaxWalkSpeed = statment.moveSpeedPoint;

	if (attackCount > 6)
	{
		if (speedyTime >= timeLimit && bIsPasiveActive == false)
		{
			bIsPasiveActive = true;
			statment.moveSpeedPoint *= 2.0f;
		}
		else if(speedyTime <= 0)
		{
			bIsPasiveActive = false;
			attackCount = 0;
			speedyTime = timeLimit+1;
			statment.moveSpeedPoint *= 0.5f;
		}
		speedyTime -= DeltaTime;
	}

	if (IsLocallyControlled())
	{
		APlayerController* playerController = Cast<APlayerController>(Controller);
		playerController->GetHitResultUnderCursor(ECC_Visibility, false, endInfo);
		endPosition = endInfo.Location;
		positionComp->SetWorldRotation(FRotationMatrix::MakeFromX(endPosition - GetActorLocation()).Rotator());
	}

	if (bombWidget->GetVisibleFlag() == true)
	{
		bombWidget->SetWorldLocation(endPosition);
	}
}

void AGunPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGunPlayer::Ability_Q_Action()
{
	Super::Ability_Q_Action();
	ServerRPC_Q_Action();
}

void AGunPlayer::Ability_E_Action()
{
	Super::Ability_E_Action();
	ServerRPC_E_Action(endPosition);
}

void AGunPlayer::Ability_R_Action()
{
	Super::Ability_R_Action();
	R_Action();
}

void AGunPlayer::Ability_R_End_Action()
{
	Super::Ability_R_End_Action();
	ServerRPC_R_End_Action(endPosition);
}

void AGunPlayer::Ability_F_Action()
{
	Super::Ability_F_Action();
	ServerRPC_F_Action(endPosition);
}

void AGunPlayer::Ability_Ev_Action()
{
	Super::Ability_Ev_Action();
	Ev_Action();
}

void AGunPlayer::Ability_Normal_Action()
{
	Super::Ability_Normal_Action();
	ServerRPC_Normal_Action(endPosition);
}

void AGunPlayer::ServerRPC_Q_Action_Implementation()
{
	MultiRPC_Q_Action();
}

void AGunPlayer::MultiRPC_Q_Action_Implementation()
{
	Q_Action();
}

// gun Change
void AGunPlayer::Q_Action()
{
	Super::Q_Action();
	if (bIsAttack == false && curr_Q > cool_Q) {
		curr_Q = 0;
		bIsAttack = true;
		PlayAnimMontage(animMontage, 1.0f * statment.attackSpeedPoint, TEXT("ChangeGun"));
		if (bIsSlowGun == false) {
			bIsSlowGun = true;
			statment.attackSpeedPoint = 0.5f * statment.attackSpeedPoint;
			statment.attackPoint = 2 * statment.attackPoint;
		}
		else if (bIsSlowGun == true)
		{
			bIsSlowGun = false;
			statment.attackSpeedPoint = 2.0f * statment.attackSpeedPoint;
			statment.attackPoint = 0.5f * statment.attackPoint;
		}
	}
}

void AGunPlayer::ServerRPC_E_Action_Implementation(FVector endLocation)
{
	MultiRPC_E_Action(endLocation);
}

void AGunPlayer::MultiRPC_E_Action_Implementation(FVector endLocation)
{
	endPosition = endLocation;
	E_Action();
}

// Lazer
void AGunPlayer::E_Action()
{
	Super::E_Action();
	if (bIsAttack == false && curr_E > cool_E) {
		curr_E = 0;
		bIsAttack = true;
		float timedelay = PlayAnimMontage(animMontage, 1.0f * statment.attackSpeedPoint, TEXT("Lazer"));

		FVector startPosition = GetMesh()->GetBoneLocation(TEXT("pistol"));

		// 쏠때 방향전환 어색해서 넣을지 말지 고민중..
		SetActorRotation(FRotationMatrix::MakeFromX(endPosition - GetActorLocation()).Rotator());


		FHitResult hitInfo;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);

		bool bHitResult = GetWorld()->LineTraceSingleByChannel(hitInfo, startPosition, endPosition, ECC_Visibility, params);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), lazerStartParticleSystem, startPosition);
		DrawDebugLineTraceSingle(GetWorld(), startPosition, endPosition, EDrawDebugTrace::ForDuration, bHitResult, hitInfo, FLinearColor::Blue, FLinearColor::Blue, timedelay);

		if (IsLocallyControlled()) {
			GetCharacterMovement()->MaxAcceleration = 0;
		}

		FTimerHandle delayHandle;
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AGunPlayer::LazerShot, hitInfo, startPosition, params, delayHandle);
		GetWorld()->GetTimerManager().SetTimer(delayHandle, TimerDelegate, 2.0f, false);
	}
}

void AGunPlayer::LazerShot(FHitResult hitInfo, FVector startPosition, FCollisionQueryParams params, FTimerHandle delayHandle)
{
	GetWorld()->LineTraceSingleByChannel(hitInfo, startPosition, endPosition, ECC_Visibility, params);
	AActor* HitActor = hitInfo.GetActor();
	FVector lengthvector = endPosition - startPosition;
	if (HitActor && HitActor->ActorHasTag("boss") && lengthvector.Length() < normalLength)
	{
		ServerRPC_AttackBoss(2);
		// 보스에 슬로우 걸기
		monster->GetCharacterMovement()->MaxWalkSpeed = 100;
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), lazerEndParticleSystem, hitInfo.Location);
	attackCount += 1;

	if (IsLocallyControlled()) {
		GetCharacterMovement()->MaxAcceleration = 2048;
	}
	if(bIsAttack == false) StopAnimMontage(animMontage);
	GetWorld()->GetTimerManager().ClearTimer(delayHandle);
}

// Call Bomb 궁극기
void AGunPlayer::R_Action()
{
	Super::R_Action();
	if (bIsAttack == false && curr_R > cool_R) {
		bombWidget->SetVisibility(true);
	}
}

void AGunPlayer::ServerRPC_R_End_Action_Implementation(FVector endLocation)
{
	MultiRPC_R_End_Action(endLocation);
}

void AGunPlayer::MultiRPC_R_End_Action_Implementation(FVector endLocation)
{
	endPosition = endLocation;
	R_End_Action();
}

void AGunPlayer::R_End_Action()
{
	if (bombWidget->GetVisibleFlag() == true) {
		curr_R = 0;
		FVector bombLocation;
		if (IsLocallyControlled()) {
			bombWidget->SetVisibility(false);
			bombLocation = bombWidget->GetComponentLocation();
		}
		else
		{
			bombLocation = endPosition;
		}
		bIsAttack = true;
		float delayTime = PlayAnimMontage(animMontage, 1.0f * statment.attackSpeedPoint, TEXT("CallBomb"));
		if (IsLocallyControlled()) {
			GetCharacterMovement()->MaxAcceleration = 0;
		}
		FTimerHandle delayHandle;
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AGunPlayer::CallBomb, delayHandle, bombLocation);
		GetWorld()->GetTimerManager().SetTimer(delayHandle, TimerDelegate, delayTime / 5, false);
	}
}

void AGunPlayer::CallBomb(FTimerHandle delayHandle, FVector bombLocation)
{
	//AGunPlayer_CallBomb* bomb = GetWorld()->SpawnActor<AGunPlayer_CallBomb>(callBombFactory, FTransform(bombWidget->GetComponentLocation()));
	AGunPlayer_CallBomb* bomb = GetWorld()->SpawnActor<AGunPlayer_CallBomb>(callBombFactory, FTransform(bombLocation));
	bomb->SetOwner(this);
	if (UKismetMathLibrary::Vector_Distance(monster->GetActorLocation(), bomb->GetActorLocation()) < 100)
		ServerRPC_AttackBoss(5);
	else if (UKismetMathLibrary::Vector_Distance(monster->GetActorLocation(), bomb->GetActorLocation()) < 400)
		ServerRPC_AttackBoss(3);
	if (IsLocallyControlled()) {
		GetCharacterMovement()->MaxAcceleration = 2048;
	}
	GetWorld()->GetTimerManager().ClearTimer(delayHandle);
}

void AGunPlayer::ServerRPC_F_Action_Implementation(FVector endLocation)
{
	MultiRPC_F_Action(endLocation);
}

void AGunPlayer::MultiRPC_F_Action_Implementation(FVector endLocation)
{
	endPosition = endLocation;
	F_Action();
}

// 폭탄 던지기
void AGunPlayer::F_Action()
{
	Super::F_Action();
	if (bIsAttack == false && curr_F > cool_F) {
		curr_F = 0;
		bIsAttack = true;
		// 쏠때 방향전환 어색해서 넣을지 말지 고민중..
		SetActorRotation(FRotationMatrix::MakeFromX(endPosition - GetActorLocation()).Rotator());

		PlayAnimMontage(animMontage, 1.0f * statment.attackSpeedPoint, TEXT("Bomb"));

		FTimerHandle delayHandle;
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AGunPlayer::ThrowBomb, delayHandle);
		GetWorld()->GetTimerManager().SetTimer(delayHandle, TimerDelegate, 0.3f, false);
	}
}

void AGunPlayer::ThrowBomb(FTimerHandle delayHandle)
{
	FTransform bombPosition = GetMesh()->GetSocketTransform(TEXT("Muzzle_02"));
	AGunPlayer_Bomb* bomb = GetWorld()->SpawnActor<AGunPlayer_Bomb>(bombFactory, bombPosition);
	bomb->SetOwner(this);
	bomb->Fire((endPosition - GetMesh()->GetSocketLocation(TEXT("Muzzle_02"))).GetSafeNormal());
	if (bIsAttack == false) StopAnimMontage(animMontage);
	GetWorld()->GetTimerManager().ClearTimer(delayHandle);
}

// 회피기
void AGunPlayer::Ev_Action()
{
	Super::Ev_Action();
}

void AGunPlayer::ServerRPC_Normal_Action_Implementation(FVector endLocation)
{
	MultiRPC_Normal_Action(endLocation);
}

void AGunPlayer::MultiRPC_Normal_Action_Implementation(FVector endLocation)
{
	endPosition = endLocation;
	Normal_Action();
}

// 평타
void AGunPlayer::Normal_Action()
{
	Super::Normal_Action();
	if (bIsAttack == false) {
		bIsAttack = true;
		PlayAnimMontage(animMontage, 1.0f * statment.attackSpeedPoint, TEXT("Default"));
		
		FVector startPosition = GetMesh()->GetBoneLocation(TEXT("pistol"));
		FVector lengthvector = endPosition - startPosition;

		// 쏠때 방향전환 어색해서 넣을지 말지 고민중..
		SetActorRotation(FRotationMatrix::MakeFromX(endPosition - GetActorLocation()).Rotator());


		FHitResult hitInfo;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);

		bool bHitResult = GetWorld()->LineTraceSingleByChannel(hitInfo, startPosition, endPosition, ECC_Visibility, params);
		//DrawDebugLineTraceSingle(GetWorld(), startPosition, endPosition, EDrawDebugTrace::ForDuration, bHitResult, hitInfo, FLinearColor::Red, FLinearColor::Green, 2.0f);
		if (bHitResult)
		{
			AActor* HitActor = hitInfo.GetActor();
			if (HitActor && HitActor->ActorHasTag("boss") && lengthvector.Length() < normalLength)
			{
				ServerRPC_AttackBoss(1);
			}
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), normalParticleSystem, hitInfo.Location);
			attackCount += 1;
		}
	}
}

void AGunPlayer::CoolTimePointChange()
{
	Super::CoolTimePointChange();
	cool_Q = 2 * statment.coolTimePoint;
	cool_E = 10 * statment.coolTimePoint;
	cool_F = 15 * statment.coolTimePoint;
	cool_R = 60 * statment.coolTimePoint;
}

void AGunPlayer::ServerRPC_AttackBoss_Implementation(float damagePercent)
{
	MultiRPC_AttackBoss(damagePercent);
}

void AGunPlayer::MultiRPC_AttackBoss_Implementation(float damagePercent)
{
	AttackBoss(damagePercent);
}

// 보스에 데미지 넣기
void AGunPlayer::AttackBoss(float damagePercent)
{
	float damage = statment.attackPoint * damagePercent;

	monster->DamageProcess(damage);
}
