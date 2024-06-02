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
#include "PlayerScoreComp.h"
#include "BossHunterGameMode.h"
#include "BossRoomGameStateBase.h"
#include "PlayerFSM.h"

AGunPlayer::AGunPlayer()
{
	// �⺻ �ɷ�ġ ����
	statment.attackPoint = 10;
	statment.defancePoint = 5;
	statment.attackSpeedPoint = 1.0f;
	statment.moveSpeedPoint = 500;
	//Player Curr HP
	statment.healthPoint = 300;
	statment.manaPoint = 300;
	statment.coolTimePoint = 1.0f;

	statment.fullHP = statment.healthPoint;
	statment.fullMP = statment.manaPoint;

	GetCharacterMovement()->MaxWalkSpeed = statment.moveSpeedPoint;

	// ���� ����Ű�� ȭ��ǥ
	positionComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Position"));
	positionComp->SetupAttachment(RootComponent);

	// �ñر� ����
	bombWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("BombWidget"));
	bombWidget->SetupAttachment(RootComponent);
	bombWidget->SetVisibility(false);

	//UPlayerScoreComp
	playerScore = CreateDefaultSubobject<UPlayerScoreComp>(TEXT("PlayerScore"));
	//UPlayerSkinComp
	playerSkin = CreateDefaultSubobject<UPlayerSkinComp>(TEXT("PlayerSkin"));
}

void AGunPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AGunPlayer::BeginPlay()
{
	Super::BeginPlay();
	// �÷��� �ϴ� ��� ������ ȭ��ǥ ���̰�
	if (!IsLocallyControlled()){
		positionComp->SetVisibility(false);
	}

	// ���� ����
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), startSound, GetActorLocation());

	// ��Ÿ�� ���� - ��ų�� ó�� �ѹ��� �ٷ� ���� �ְ�
	curr_E = cool_E;
	curr_F = cool_F;
	curr_Q = cool_Q;
	curr_R = cool_R;
	curr_SP = cool_SP;

	// ���� ����
	UActorComponent* findComp = FindComponentByClass<UPlayerScoreComp>();
	playerScore = Cast<UPlayerScoreComp>(findComp);
	
}

void AGunPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// ü���� 0 ���ϸ� �׾��
	if (statment.healthPoint <= 0 /*&& IsLocallyControlled()*/) {
		bIsDie = true;
		statment.healthPoint = 0;
		GetCharacterMovement()->MaxAcceleration = 0;
		GetCharacterMovement()->MaxWalkSpeed = 0;
		bIsAttack = true;
		return;
	}


	// ȸ���� ����
	if (bIsAttack == true)
		GetMesh()->SetWorldRotation(roll);
	else if (bIsAttack == false)
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	// ��Ÿ�� ����
	curr_E += DeltaTime;
	curr_R += DeltaTime;
	curr_Q += DeltaTime;
	curr_F += DeltaTime;
	curr_SP += DeltaTime;

	// ���� �ڵ� ȸ��
	if (statment.manaPoint <= statment.fullMP)
	{
		statment.manaPoint += DeltaTime;
	}

	// �нú� ȿ�� - 6�� ������ �ߵ�
	if (attackCount > 6)
	{
		// �нú갡 ������ �־����� �̼� �ι�
		if (speedyTime >= timeLimit && bIsPasiveActive == false)
		{
			bIsPasiveActive = true;
			statment.moveSpeedPoint *= 2.0f;
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Passive_Sound, GetActorLocation());
		}
		// �нú� �ð� ������
		else if(speedyTime <= 0)
		{
			bIsPasiveActive = false;
			attackCount = 0;
			speedyTime = timeLimit+1;
			statment.moveSpeedPoint *= 0.5f;
			EnableInput(GetWorld()->GetFirstPlayerController());
		}
		//// �нú� �ð� �ȿ� 100Ÿ �̻� ������ ��������
		//else if (attackCount > 100)
		//{
		//	DisableInput(GetWorld()->GetFirstPlayerController());
		//	speedyTime -= 1.0f;
		//}
		// �нú� ��Ÿ�� ���
		speedyTime -= DeltaTime;
	}

	// �÷��̾� ���׼��� Ŀ������ �޾ƿ���
	if (IsLocallyControlled())
	{
		APlayerController* playerController = Cast<APlayerController>(Controller);
		playerController->GetHitResultUnderCursor(ECC_Visibility, false, endInfo);
		endPosition = endInfo.Location;
		positionComp->SetWorldRotation(FRotationMatrix::MakeFromX(endPosition - GetActorLocation()).Rotator());
	}

	// �ñر� ������ ���� ���̰�
	if (bombWidget->GetVisibleFlag() == true)
	{
		bombWidget->SetWorldRotation(FRotator(90, 0, 0));
		bombWidget->SetWorldLocation(FVector(endPosition.X, endPosition.Y, endPosition.Z + 50));
	}

	// ȸ�Ǳ� ���� ���ư���
	if (GetCharacterMovement()->MaxWalkSpeed > statment.moveSpeedPoint) {
		h = 0;
		v = 0;
		const FVector ForwardDirection = GetActorForwardVector() * evv;
		const FVector RightDirection = GetActorRightVector() * evh;

		FVector newdir = ForwardDirection + RightDirection;
		AddMovementInput(newdir);
		GetCharacterMovement()->MaxWalkSpeed -= 50;
		return;
	}
	// ȸ�Ǳ� ������ �ٽ� �����̰�
	else if (evv + evh < 10)
	{
		evv = 100;
		evh = 100;
		if (IsLocallyControlled()) {
			EnableInput(GetWorld()->GetFirstPlayerController());
		}
		/*roll = FRotator(0, -90, 0);*/
	}

	// ���� ������ �̼��� ������ �̼����� ����
	GetCharacterMovement()->MaxWalkSpeed = statment.moveSpeedPoint;

	// �÷��̾ �ٽ� ������ �� �ְ� �ϱ�
	if (playerFSM->GetPlayerSkillState() == playerSkillSet::Idle && IsLocallyControlled()) {
		GetCharacterMovement()->MaxAcceleration = 2048;
	}
}

// ��ǲ �޾ƿ���
void AGunPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// ��ǲ�� ���� �ҷ����� q
void AGunPlayer::Ability_Q_Action()
{
	if (playerFSM->playerSkillState == playerSkillSet::Idle && monster)
	{
		if (bIsAttack == true || curr_Q < cool_Q) return;
		Super::Ability_Q_Action();
		ServerRPC_Q_Action();
	}
}

// ��ǲ�� ���� �ҷ����� e
void AGunPlayer::Ability_E_Action()
{
	if (playerFSM->playerSkillState == playerSkillSet::Idle && monster)
	{
		if (bIsAttack == true || curr_E < cool_E) return;
		Super::Ability_E_Action();
		ServerRPC_E_Action(endPosition);
	}
}

// ��ǲ�� ���� �ҷ����� r
void AGunPlayer::Ability_R_Action()
{
	if (playerFSM->playerSkillState == playerSkillSet::Idle && monster)
	{
		if (bIsAttack == true || curr_R < cool_R) return;
		Super::Ability_R_Action();
		R_Action();
	}
}

// ��ǲ�� ���� �ҷ����� r end
void AGunPlayer::Ability_R_End_Action()
{
	if (playerFSM->playerSkillState == playerSkillSet::R_Skill && monster)
	{
		Super::Ability_R_End_Action();
		ServerRPC_R_End_Action(endPosition);
	}
}

// ��ǲ�� ���� �ҷ����� f
void AGunPlayer::Ability_F_Action()
{
	if (playerFSM->playerSkillState == playerSkillSet::Idle && monster)
	{
		if (bIsAttack == true || curr_F < cool_F) return;
		Super::Ability_F_Action();
		ServerRPC_F_Action(endPosition);
	}
}

// ��ǲ�� ���� �ҷ����� space
void AGunPlayer::Ability_Ev_Action()
{
	if (playerFSM->playerSkillState == playerSkillSet::Idle && monster)
	{
		if (bIsAttack == true || curr_SP < cool_SP) return;
		Super::Ability_Ev_Action();
		ServerRPC_Ev_Action();
	}
}

// ��ǲ�� ���� �ҷ����� ��Ÿ
void AGunPlayer::Ability_Normal_Action()
{
	if (playerFSM->playerSkillState == playerSkillSet::Idle && monster)
	{
		Super::Ability_Normal_Action();
		ServerRPC_Normal_Action(endPosition);
	}
}

//////////////////////////////////////////////////////// Q��ų ����
// ������ ������
void AGunPlayer::ServerRPC_Q_Action_Implementation()
{
	MultiRPC_Q_Action();
}

// Ŭ�󿡰� �Ѹ���
void AGunPlayer::MultiRPC_Q_Action_Implementation()
{
	Q_Action();
}

// �ٴ� ��� - Q��ų
void AGunPlayer::Q_Action()
{
	Super::Q_Action();
	if (bIsAttack == false && curr_Q > cool_Q) {
		// ���� ������ ����
		if (statment.manaPoint < 20) {
			playerFSM->SetPlayerSkillState(playerSkillSet::Idle);
			return;
		}

		// ��Ÿ�� ����
		curr_Q = 0;

		// ���� �Ҹ�
		statment.manaPoint -= 20;

		// �������� ǥ��
		bIsAttack = true;

		// �Ҹ��� �ִϸ��̼�
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Q_Sound, GetActorLocation());
		PlayAnimMontage(animFullBodyMontage, 1.5f * statment.attackSpeedPoint, TEXT("Default"));

		// ������ �ڵ� - �Ÿ��� 700 �̳��� 300 ������
		if (FVector::Dist(GetActorLocation(), monster->GetActorLocation()) < 700)
		{
			monster->SetActorLocation(monster->GetActorLocation() + (monster->GetActorForwardVector() * -1 + monster->GetActorUpVector() * 0.2) * 300);
		}
		
		// ȿ���� ������ �Ҹ� ���
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), StunParticleSystem, GetActorLocation() + GetActorUpVector() * -40);
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Q_Active_Sound, GetActorLocation());
	}
	// �÷��̾ �ٽ� ������ �� �ְ� �ϱ�
	if (IsLocallyControlled()) {
		GetCharacterMovement()->MaxAcceleration = 2048;
	}
}

////////////////////////////////////////////////////////////////// E��ų ����
// ������ ����
void AGunPlayer::ServerRPC_E_Action_Implementation(FVector endLocation)
{
	MultiRPC_E_Action(endLocation);
}

// Ŭ�� �Ѹ�
void AGunPlayer::MultiRPC_E_Action_Implementation(FVector endLocation)
{
	endPosition = endLocation;
	E_Action();
}

// ������ ��ų
void AGunPlayer::E_Action()
{
	Super::E_Action();
	if (bIsAttack == false && curr_E > cool_E) {
		// ���� ������ ���
		if (statment.manaPoint < 20) {
			playerFSM->SetPlayerSkillState(playerSkillSet::Idle);
			return;
		}

		// ��Ÿ�� ����
		curr_E = 0;

		// ���� �Ҹ�
		statment.manaPoint -= 20;

		// ������ ǥ��
		bIsAttack = true;

		// ��ų ���ۼҸ��� �ִϸ��̼� ���
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), E_Sound, GetActorLocation());
		float timedelay = PlayAnimMontage(animMontage, 1.0f * statment.attackSpeedPoint, TEXT("Lazer"));
		timedelay *= statment.attackSpeedPoint;

		// ��� ��ġ ã�ƿ���
		lazerStartPosition = GetMesh()->GetBoneLocation(TEXT("pistol"));

		// Ŭ���� ��ġ�� ������ȯ
		roll = FRotationMatrix::MakeFromX(endPosition - GetActorLocation()).Rotator();
		roll.Pitch = 0;
		roll.Yaw += -90;
		GetMesh()->SetWorldRotation(roll);

		// ���� Ʈ���̽��� ��� ���� �ʿ��� ������
		FHitResult hitInfo;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);

		// ���� Ʈ���̽� ���
		GetWorld()->LineTraceSingleByChannel(hitInfo, lazerStartPosition, endPosition, ECC_Visibility, params);
		// ���� ��ġ ����
		if (hitInfo.GetActor())
			lazerEndPosition = hitInfo.GetActor()->GetActorLocation();
		else
			lazerEndPosition = endPosition;
		// ȿ�� ��ƼŬ ���
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), lazerStartParticleSystem, lazerStartPosition, (endPosition - GetActorLocation()).Rotation());
		//DrawDebugLineTraceSingle(GetWorld(), startPosition, endPosition, EDrawDebugTrace::ForDuration, bHitResult, hitInfo, FLinearColor::Blue, FLinearColor::Blue, 1.0f);

		// ��µ��� �������̰�
		if (IsLocallyControlled()) {
			GetCharacterMovement()->MaxAcceleration = 0;
		}


		// delay �Լ��� ����ϱ� ���� ����
		/*FTimerHandle delayHandle;
		if (playerFSM->GetPlayerSkillState() == playerSkillSet::Idle)
			LazerShot(hitInfo, startPosition, params, delayHandle);*/

		// ������ �Լ� �ɱ�
		//FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AGunPlayer::LazerShot, hitInfo, startPosition, params, delayHandle);
		//GetWorld()->GetTimerManager().SetTimer(delayHandle, TimerDelegate, timedelay*0.3f, false);
	}
}

// ������ �Լ� �Ŀ� �Ҹ��� ������
void AGunPlayer::LazerShot()
{
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	// ������ �ѹ� �� ���� ���� ����� �ִ��� ����
	GetWorld()->LineTraceSingleByChannel(hitInfo, lazerStartPosition, lazerEndPosition, ECC_Visibility, params);

	// ���� ���� ����
	AActor* HitActor = hitInfo.GetActor();

	// ���� ���Ͱ� boss�̸�
	if (HitActor && HitActor->ActorHasTag("boss"))
	{
		// 2��� �� ����
		ServerRPC_AttackBoss(2);

		// ������ ���ο� �ɱ�
		monster->GetCharacterMovement()->MaxWalkSpeed = 100;

		// ���� ȿ�� ��ƼŬ ǥ��
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), lazerEndParticleSystem, hitInfo.Location);
	}
	// ������ ����������
	else
	{
		// ���� ȿ�� ��ƼŬ ǥ��
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), lazerFailParticleSystem, endPosition);
	}
	// �Ҹ� ���
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), E_Active_Sound, endPosition);

	// �нú� ����� ���� ī��Ʈ
	attackCount += 1;

	// ȸ���� ���
	//roll = FRotator(0, -90, 0);

	// �÷��̾ �ٽ� ������ �� �ְ� �ϱ�
	if (IsLocallyControlled()) {
		GetCharacterMovement()->MaxAcceleration = 2048;
	}
	
	// Ÿ�̸� ����
	/*GetWorld()->GetTimerManager().ClearTimer(delayHandle);*/
}

/////////////////////////////////////////////////////// R ��ų ����
// Call Bomb �ñر�
void AGunPlayer::R_Action()
{
	Super::R_Action();
	if (bIsAttack == false && curr_R > cool_R) {
		// ���� ������ ����
		if (statment.manaPoint < 50) {
			playerFSM->SetPlayerSkillState(playerSkillSet::Idle);
			return;
		}
		// �ñر� ���� ���̰�
		bombWidget->SetVisibility(true);
	}
}
// ������ ������
void AGunPlayer::ServerRPC_R_End_Action_Implementation(FVector endLocation)
{
	MultiRPC_R_End_Action(endLocation);
}
// Ŭ��� ������
void AGunPlayer::MultiRPC_R_End_Action_Implementation(FVector endLocation)
{
	endPosition = endLocation;
	R_End_Action();
}
// �ñر� ����
void AGunPlayer::R_End_Action()
{
	if (bombWidget->GetVisibleFlag() == true || !IsLocallyControlled()) {
		// ��Ÿ�� �ʱ�ȭ
		curr_R = 0;

		// ���� �ʱ�ȭ
		statment.manaPoint -= 50;

		// ��ų�� ���ư� ��ġ ��ƿ���
		if (IsLocallyControlled()) {
			bombWidget->SetVisibility(false);
			bombLocation = bombWidget->GetComponentLocation();
		}
		else
		{
			bombLocation = endPosition;
		}

		bIsAttack = true;

		// ���� ��ȯ
		roll = FRotationMatrix::MakeFromX(bombLocation - GetActorLocation()).Rotator();
		roll.Pitch = 0;
		roll.Yaw += -90;
		GetMesh()->SetWorldRotation(roll);
		
		// ��ų ����ϴ� �Ҹ� ���
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), R_Sound, GetActorLocation());

		// �ִϸ��̼� ���
		/*float delayTime = */PlayAnimMontage(animMontage, 1.0f * statment.attackSpeedPoint, TEXT("CallBomb"));
		/*delayTime *= statment.attackSpeedPoint;*/
		
		// �÷��̾� �������̰�
		if (IsLocallyControlled()) {
			GetCharacterMovement()->MaxAcceleration = 0;
		}

		//// ������ �Լ��� �ʿ��� ����
		//FTimerHandle delayHandle;

		//// ������ �Լ� ȣ��
		//FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AGunPlayer::CallBomb, delayHandle, bombLocation);
		//GetWorld()->GetTimerManager().SetTimer(delayHandle, TimerDelegate, delayTime * 0.5f, false);
	}
}

// ������ �Լ��� �Ҹ��� ��¥ ��ų
void AGunPlayer::CallBomb()
{
	// bomb ���� ��ȯ
	AGunPlayer_CallBomb* bomb = GetWorld()->SpawnActor<AGunPlayer_CallBomb>(callBombFactory, FTransform(bombLocation));
	
	// ��ȯ�� ���� ���� ����
	bomb->SetOwner(this);

	// �Ÿ� ��� �� �ٸ��� �� �־��ֱ�
	if (UKismetMathLibrary::Vector_Distance(monster->GetActorLocation(), bomb->GetActorLocation()) < 100)
		ServerRPC_AttackBoss(5);
	else if (UKismetMathLibrary::Vector_Distance(monster->GetActorLocation(), bomb->GetActorLocation()) < 400)
		ServerRPC_AttackBoss(3);
	
	// �÷��̾ ������ �� �ְ� ����
	if (IsLocallyControlled()) {
		GetCharacterMovement()->MaxAcceleration = 2048;
	}

	// �Ҹ� ���
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), R_Active_Sound, bombLocation);

	// ���� ��ȯ
	//roll = FRotator(0, -90, 0);

	//GetWorld()->GetTimerManager().ClearTimer(delayHandle);
}

////////////////////////////////////////////////////////////////////////////// F ��ų ����
// ������ ����
void AGunPlayer::ServerRPC_F_Action_Implementation(FVector endLocation)
{
	MultiRPC_F_Action(endLocation);
}
// Ŭ��� �Ѹ�
void AGunPlayer::MultiRPC_F_Action_Implementation(FVector endLocation)
{
	endPosition = endLocation;
	F_Action();
}

// ��ź ������
void AGunPlayer::F_Action()
{
	Super::F_Action();
	if (bIsAttack == false && curr_F > cool_F) {
		// ���� ������ ���� �ȵǰ�
		if (statment.manaPoint < 30) {
			playerFSM->SetPlayerSkillState(playerSkillSet::Idle);
			return;
		}

		// ��Ÿ�� ����
		curr_F = 0;

		// ���� �Ҹ�
		statment.manaPoint -= 30;

		// �������� ǥ��
		bIsAttack = true;

		// ���� ��ȯ
		roll = FRotationMatrix::MakeFromX(endPosition - GetActorLocation()).Rotator();
		roll.Pitch = 0;
		roll.Yaw += -90;
		GetMesh()->SetWorldRotation(roll);

		// ��� �Ҹ� ���
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), F_Sound, GetActorLocation());

		// �ִϸ��̼� ���
		float delay = PlayAnimMontage(animMontage, 1.0f * statment.attackSpeedPoint, TEXT("Bomb"));
		/*delay *= statment.attackSpeedPoint;*/

		//// ������ �Լ��� �ʿ��� ����
		//FTimerHandle delayHandle;

		//// ������ �Լ� ����
		//FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AGunPlayer::ThrowBomb, delayHandle);
		//GetWorld()->GetTimerManager().SetTimer(delayHandle, TimerDelegate, delay, false);
	}
}

// ������ �� �Ҹ��� ���� ��ų �Լ�
void AGunPlayer::ThrowBomb()
{
	// ��𿡼� ������ ��ġ �޾ƿ���
	FTransform bombPosition = GetMesh()->GetSocketTransform(TEXT("Muzzle_02"));

	// ��ź �����ؼ� ������
	AGunPlayer_Bomb* bomb = GetWorld()->SpawnActor<AGunPlayer_Bomb>(bombFactory, bombPosition);
	bomb->SetOwner(this);

	// ���ư� ��ġ�� ��Ƽ� ��ź ������
	bomb->Fire((endPosition - GetMesh()->GetSocketLocation(TEXT("Muzzle_02"))).GetSafeNormal());
	
	// ���� ���
	//roll = FRotator(0, -90, 0);

	//GetWorld()->GetTimerManager().ClearTimer(delayHandle);
}

/////////////////////////////////////////////////////////////////// ȸ�Ǳ�
// ������ ����
void AGunPlayer::ServerRPC_Ev_Action_Implementation()
{
	MultiRPC_Ev_Action();
}
// Ŭ�� �Ѹ�
void AGunPlayer::MultiRPC_Ev_Action_Implementation()
{
	Ev_Action();
}

// ȸ�Ǳ�
void AGunPlayer::Ev_Action()
{
	Super::Ev_Action();
	if (bIsAttack == false && curr_SP >= cool_SP) {
		// ��Ÿ�� �ʱ�ȭ
		curr_SP = 0;

		// ������ ǥ��
		bIsAttack = true;

		// �Ҹ� ���
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), EV_Sound, GetActorLocation());

		// �ִϸ��̼� ���
		evTime = PlayAnimMontage(animFullBodyMontage, 1.0f * statment.attackSpeedPoint, TEXT("Jump"));

		// ���ư� �ӵ� ���� -  ƽ���� �����
		GetCharacterMovement()->MaxWalkSpeed = 3500;

		// ���ư� ���� ����
		evh = h;
		evv = v;
		if (evh == 0 && evv == 0) evv = 1;

		// �����̼� ������
		roll = FRotationMatrix::MakeFromX(GetActorForwardVector() * evv + GetActorRightVector() * evh).Rotator();
		roll.Yaw += -90;
		roll.Pitch = 0;
		GetMesh()->SetWorldRotation(roll);

		// �÷��̾� �������̰�
		if(IsLocallyControlled())
			DisableInput(GetWorld()->GetFirstPlayerController());
	}
	
}

////////////////////////////////////////////////////////////////////////////////// ��Ÿ
// ������ ����
void AGunPlayer::ServerRPC_Normal_Action_Implementation(FVector endLocation)
{
	MultiRPC_Normal_Action(endLocation);
}
// Ŭ�� �Ѹ�
void AGunPlayer::MultiRPC_Normal_Action_Implementation(FVector endLocation)
{
	endPosition = endLocation;
	Normal_Action();
}

// ��Ÿ
void AGunPlayer::Normal_Action()
{
	Super::Normal_Action();
	if (bIsAttack == false) {
		// �����̼��� ���� �� ����
		bIsAttack = true;

		// �ִϸ��̼� ���
		PlayAnimMontage(animMontage, 1.0f * statment.attackSpeedPoint, TEXT("Default"));
		
		// ���� ��ġ�� ��Ÿ� ���
		FVector startPosition = GetMesh()->GetBoneLocation(TEXT("pistol"));
		FVector lengthvector = endPosition - startPosition;

		// ���� ��ȯ
		roll = FRotationMatrix::MakeFromX(endPosition - GetActorLocation()).Rotator();
		roll.Pitch = 0;
		roll.Yaw += -90;
		GetMesh()->SetWorldRotation(roll);

		// ���� ���� Ʈ���̽��� �ʿ��� ����
		FHitResult hitInfo;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);

		// ���� Ʈ���̽��� ����
		bool bHitResult = GetWorld()->LineTraceSingleByChannel(hitInfo, startPosition, endPosition, ECC_Visibility, params);
		//DrawDebugLineTraceSingle(GetWorld(), startPosition, endPosition, EDrawDebugTrace::ForDuration, bHitResult, hitInfo, FLinearColor::Red, FLinearColor::Green, 2.0f);
		
		// �¾�����
		if (bHitResult)
		{
			// ���� ���� ��������
			AActor* HitActor = hitInfo.GetActor();
			// ���� ���Ͱ� ��������, ��Ÿ� �ȿ��� ���Դ��� Ȯ��
			if (HitActor && HitActor->ActorHasTag("boss") && lengthvector.Length() < normalLength)
			{
				// ����
				ServerRPC_AttackBoss(1);
			}

			// ���� ���� ����Ʈ ǥ��
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), normalParticleSystem, hitInfo.Location);
			attackCount += 1;
		}
		else
		{
			// ���� ���� ����Ʈ ǥ��
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), normalParticleSystem, endPosition);
		}
		// ���� �Ҹ� ���
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Normal_Sound, endPosition);
	}
	// �÷��̾ �ٽ� ������ �� �ְ� �ϱ�
	if (IsLocallyControlled()) {
		GetCharacterMovement()->MaxAcceleration = 2048;
	}
}

// ������ ���Ž� ��Ÿ�� ����ǵ���
void AGunPlayer::CoolTimePointChange()
{
	Super::CoolTimePointChange();
	cool_Q = 10 * statment.coolTimePoint; // 10
	cool_E = 15 * statment.coolTimePoint; // 15
	cool_F = 5 * statment.coolTimePoint; // 5
	cool_R = 60 * statment.coolTimePoint; // 60
	cool_SP = 5 * statment.coolTimePoint; // 5

}

//////////////////////////////////////////////////////////////////////////// ������ ������ �ֱ�
// ������ ����
void AGunPlayer::ServerRPC_AttackBoss_Implementation(float damagePercent)
{
	MultiRPC_AttackBoss(damagePercent);
}
// Ŭ�� �Ѹ�
void AGunPlayer::MultiRPC_AttackBoss_Implementation(float damagePercent)
{
	AttackBoss(damagePercent);
}

// ������ ������ �ֱ�
void AGunPlayer::AttackBoss(float damagePercent)
{
	UE_LOG(LogTemp, Warning, TEXT("--- attackPoint : %f, damagePercent : %f"), statment.attackPoint, damagePercent);

	// ������ ��� ����ؼ� �� ������ ���
	float damage = statment.attackPoint * damagePercent;
	// ����
	monster->DamageProcess(damage);
}

//////////////////////////////////////////////////////// �ӽ� �������� ���� ��ư
// ���� ����
void AGunPlayer::ServerRPC_Respawn_Implementation()
{
	MultiRPC_Respawn();
}
// Ŭ�� �Ѹ�
void AGunPlayer::MultiRPC_Respawn_Implementation()
{
	Respawn();
}
// �ӽ� ��Ȱ
void AGunPlayer::Respawn()
{
	bIsDie = false;
	GetCharacterMovement()->MaxAcceleration = 2048;
	GetCharacterMovement()->MaxWalkSpeed = statment.moveSpeedPoint;
	statment.healthPoint = statment.fullHP;
	statment.manaPoint = statment.fullMP;
	bIsAttack = false;
}

void AGunPlayer::Ability_Respawn()
{
	Super::Ability_Respawn();
	ServerRPC_Respawn();
}

void AGunPlayer::Inventory_Btn(int32 index)
{
	Super::Inventory_Btn(index);
}
