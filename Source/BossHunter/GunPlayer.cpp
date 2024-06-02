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
	// 기본 능력치 셋팅
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

	// 방향 가리키는 화살표
	positionComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Position"));
	positionComp->SetupAttachment(RootComponent);

	// 궁극기 위젯
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
	// 플레이 하는 사람 눈에만 화살표 보이게
	if (!IsLocallyControlled()){
		positionComp->SetVisibility(false);
	}

	// 시작 사운드
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), startSound, GetActorLocation());

	// 쿨타임 조정 - 스킬을 처음 한번은 바로 쓸수 있게
	curr_E = cool_E;
	curr_F = cool_F;
	curr_Q = cool_Q;
	curr_R = cool_R;
	curr_SP = cool_SP;

	// 점수 관련
	UActorComponent* findComp = FindComponentByClass<UPlayerScoreComp>();
	playerScore = Cast<UPlayerScoreComp>(findComp);
	
}

void AGunPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 체력이 0 이하면 죽어라
	if (statment.healthPoint <= 0 /*&& IsLocallyControlled()*/) {
		bIsDie = true;
		statment.healthPoint = 0;
		GetCharacterMovement()->MaxAcceleration = 0;
		GetCharacterMovement()->MaxWalkSpeed = 0;
		bIsAttack = true;
		return;
	}


	// 회전값 조정
	if (bIsAttack == true)
		GetMesh()->SetWorldRotation(roll);
	else if (bIsAttack == false)
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	// 쿨타임 세기
	curr_E += DeltaTime;
	curr_R += DeltaTime;
	curr_Q += DeltaTime;
	curr_F += DeltaTime;
	curr_SP += DeltaTime;

	// 마나 자동 회복
	if (statment.manaPoint <= statment.fullMP)
	{
		statment.manaPoint += DeltaTime;
	}

	// 패시브 효과 - 6대 때리면 발동
	if (attackCount > 6)
	{
		// 패시브가 안켜져 있었으면 이속 두배
		if (speedyTime >= timeLimit && bIsPasiveActive == false)
		{
			bIsPasiveActive = true;
			statment.moveSpeedPoint *= 2.0f;
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Passive_Sound, GetActorLocation());
		}
		// 패시브 시간 끝나면
		else if(speedyTime <= 0)
		{
			bIsPasiveActive = false;
			attackCount = 0;
			speedyTime = timeLimit+1;
			statment.moveSpeedPoint *= 0.5f;
			EnableInput(GetWorld()->GetFirstPlayerController());
		}
		//// 패시브 시간 안에 100타 이상 때리면 못움직임
		//else if (attackCount > 100)
		//{
		//	DisableInput(GetWorld()->GetFirstPlayerController());
		//	speedyTime -= 1.0f;
		//}
		// 패시브 쿨타임 재기
		speedyTime -= DeltaTime;
	}

	// 플레이어 한테서만 커서값을 받아오자
	if (IsLocallyControlled())
	{
		APlayerController* playerController = Cast<APlayerController>(Controller);
		playerController->GetHitResultUnderCursor(ECC_Visibility, false, endInfo);
		endPosition = endInfo.Location;
		positionComp->SetWorldRotation(FRotationMatrix::MakeFromX(endPosition - GetActorLocation()).Rotator());
	}

	// 궁극기 누르면 위젯 보이게
	if (bombWidget->GetVisibleFlag() == true)
	{
		bombWidget->SetWorldRotation(FRotator(90, 0, 0));
		bombWidget->SetWorldLocation(FVector(endPosition.X, endPosition.Y, endPosition.Z + 50));
	}

	// 회피기 사용시 날아가기
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
	// 회피기 끝나면 다시 움직이게
	else if (evv + evh < 10)
	{
		evv = 100;
		evh = 100;
		if (IsLocallyControlled()) {
			EnableInput(GetWorld()->GetFirstPlayerController());
		}
		/*roll = FRotator(0, -90, 0);*/
	}

	// 별일 없으면 이속은 스탯의 이속으로 유지
	GetCharacterMovement()->MaxWalkSpeed = statment.moveSpeedPoint;

	// 플레이어가 다시 움직일 수 있게 하기
	if (playerFSM->GetPlayerSkillState() == playerSkillSet::Idle && IsLocallyControlled()) {
		GetCharacterMovement()->MaxAcceleration = 2048;
	}
}

// 인풋 받아오기
void AGunPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// 인풋에 의해 불려지는 q
void AGunPlayer::Ability_Q_Action()
{
	if (playerFSM->playerSkillState == playerSkillSet::Idle && monster)
	{
		if (bIsAttack == true || curr_Q < cool_Q) return;
		Super::Ability_Q_Action();
		ServerRPC_Q_Action();
	}
}

// 인풋에 의해 불려지는 e
void AGunPlayer::Ability_E_Action()
{
	if (playerFSM->playerSkillState == playerSkillSet::Idle && monster)
	{
		if (bIsAttack == true || curr_E < cool_E) return;
		Super::Ability_E_Action();
		ServerRPC_E_Action(endPosition);
	}
}

// 인풋에 의해 불려지는 r
void AGunPlayer::Ability_R_Action()
{
	if (playerFSM->playerSkillState == playerSkillSet::Idle && monster)
	{
		if (bIsAttack == true || curr_R < cool_R) return;
		Super::Ability_R_Action();
		R_Action();
	}
}

// 인풋에 의해 불려지는 r end
void AGunPlayer::Ability_R_End_Action()
{
	if (playerFSM->playerSkillState == playerSkillSet::R_Skill && monster)
	{
		Super::Ability_R_End_Action();
		ServerRPC_R_End_Action(endPosition);
	}
}

// 인풋에 의해 불려지는 f
void AGunPlayer::Ability_F_Action()
{
	if (playerFSM->playerSkillState == playerSkillSet::Idle && monster)
	{
		if (bIsAttack == true || curr_F < cool_F) return;
		Super::Ability_F_Action();
		ServerRPC_F_Action(endPosition);
	}
}

// 인풋에 의해 불려지는 space
void AGunPlayer::Ability_Ev_Action()
{
	if (playerFSM->playerSkillState == playerSkillSet::Idle && monster)
	{
		if (bIsAttack == true || curr_SP < cool_SP) return;
		Super::Ability_Ev_Action();
		ServerRPC_Ev_Action();
	}
}

// 인풋에 의해 불려지는 평타
void AGunPlayer::Ability_Normal_Action()
{
	if (playerFSM->playerSkillState == playerSkillSet::Idle && monster)
	{
		Super::Ability_Normal_Action();
		ServerRPC_Normal_Action(endPosition);
	}
}

//////////////////////////////////////////////////////// Q스킬 시작
// 서버로 날리기
void AGunPlayer::ServerRPC_Q_Action_Implementation()
{
	MultiRPC_Q_Action();
}

// 클라에게 뿌리기
void AGunPlayer::MultiRPC_Q_Action_Implementation()
{
	Q_Action();
}

// 바닥 찍기 - Q스킬
void AGunPlayer::Q_Action()
{
	Super::Q_Action();
	if (bIsAttack == false && curr_Q > cool_Q) {
		// 마나 없으면 못씀
		if (statment.manaPoint < 20) {
			playerFSM->SetPlayerSkillState(playerSkillSet::Idle);
			return;
		}

		// 쿨타임 리셋
		curr_Q = 0;

		// 마나 소모
		statment.manaPoint -= 20;

		// 공격중을 표시
		bIsAttack = true;

		// 소리와 애니메이션
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Q_Sound, GetActorLocation());
		PlayAnimMontage(animFullBodyMontage, 1.5f * statment.attackSpeedPoint, TEXT("Default"));

		// 실질적 코드 - 거리가 700 이내면 300 날린다
		if (FVector::Dist(GetActorLocation(), monster->GetActorLocation()) < 700)
		{
			monster->SetActorLocation(monster->GetActorLocation() + (monster->GetActorForwardVector() * -1 + monster->GetActorUpVector() * 0.2) * 300);
		}
		
		// 효과와 날리는 소리 출력
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), StunParticleSystem, GetActorLocation() + GetActorUpVector() * -40);
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Q_Active_Sound, GetActorLocation());
	}
	// 플레이어가 다시 움직일 수 있게 하기
	if (IsLocallyControlled()) {
		GetCharacterMovement()->MaxAcceleration = 2048;
	}
}

////////////////////////////////////////////////////////////////// E스킬 시작
// 서버에 날림
void AGunPlayer::ServerRPC_E_Action_Implementation(FVector endLocation)
{
	MultiRPC_E_Action(endLocation);
}

// 클라에 뿌림
void AGunPlayer::MultiRPC_E_Action_Implementation(FVector endLocation)
{
	endPosition = endLocation;
	E_Action();
}

// 레이저 스킬
void AGunPlayer::E_Action()
{
	Super::E_Action();
	if (bIsAttack == false && curr_E > cool_E) {
		// 마나 없으면 취소
		if (statment.manaPoint < 20) {
			playerFSM->SetPlayerSkillState(playerSkillSet::Idle);
			return;
		}

		// 쿨타임 리셋
		curr_E = 0;

		// 마나 소모
		statment.manaPoint -= 20;

		// 공격중 표시
		bIsAttack = true;

		// 스킬 시작소리와 애니메이션 출력
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), E_Sound, GetActorLocation());
		float timedelay = PlayAnimMontage(animMontage, 1.0f * statment.attackSpeedPoint, TEXT("Lazer"));
		timedelay *= statment.attackSpeedPoint;

		// 출력 위치 찾아오기
		lazerStartPosition = GetMesh()->GetBoneLocation(TEXT("pistol"));

		// 클릭한 위치로 방향전환
		roll = FRotationMatrix::MakeFromX(endPosition - GetActorLocation()).Rotator();
		roll.Pitch = 0;
		roll.Yaw += -90;
		GetMesh()->SetWorldRotation(roll);

		// 라인 트레이스를 쏘기 위해 필요한 변수들
		FHitResult hitInfo;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);

		// 라인 트레이스 쏘기
		GetWorld()->LineTraceSingleByChannel(hitInfo, lazerStartPosition, endPosition, ECC_Visibility, params);
		// 맞은 위치 저장
		if (hitInfo.GetActor())
			lazerEndPosition = hitInfo.GetActor()->GetActorLocation();
		else
			lazerEndPosition = endPosition;
		// 효과 파티클 출력
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), lazerStartParticleSystem, lazerStartPosition, (endPosition - GetActorLocation()).Rotation());
		//DrawDebugLineTraceSingle(GetWorld(), startPosition, endPosition, EDrawDebugTrace::ForDuration, bHitResult, hitInfo, FLinearColor::Blue, FLinearColor::Blue, 1.0f);

		// 쏘는동안 못움직이게
		if (IsLocallyControlled()) {
			GetCharacterMovement()->MaxAcceleration = 0;
		}


		// delay 함수를 사용하기 위한 변수
		/*FTimerHandle delayHandle;
		if (playerFSM->GetPlayerSkillState() == playerSkillSet::Idle)
			LazerShot(hitInfo, startPosition, params, delayHandle);*/

		// 딜레이 함수 걸기
		//FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AGunPlayer::LazerShot, hitInfo, startPosition, params, delayHandle);
		//GetWorld()->GetTimerManager().SetTimer(delayHandle, TimerDelegate, timedelay*0.3f, false);
	}
}

// 딜레이 함수 후에 불리는 레이저
void AGunPlayer::LazerShot()
{
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	// 라인을 한번 더 쏴서 아직 맞출수 있는지 검출
	GetWorld()->LineTraceSingleByChannel(hitInfo, lazerStartPosition, lazerEndPosition, ECC_Visibility, params);

	// 맞은 액터 검출
	AActor* HitActor = hitInfo.GetActor();

	// 맞은 액터가 boss이면
	if (HitActor && HitActor->ActorHasTag("boss"))
	{
		// 2배수 딜 공격
		ServerRPC_AttackBoss(2);

		// 보스에 슬로우 걸기
		monster->GetCharacterMovement()->MaxWalkSpeed = 100;

		// 성공 효과 파티클 표출
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), lazerEndParticleSystem, hitInfo.Location);
	}
	// 보스를 못맞췄으면
	else
	{
		// 실패 효과 파티클 표출
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), lazerFailParticleSystem, endPosition);
	}
	// 소리 출력
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), E_Active_Sound, endPosition);

	// 패시브 계산을 위한 카운트
	attackCount += 1;

	// 회전값 계산
	//roll = FRotator(0, -90, 0);

	// 플레이어가 다시 움직일 수 있게 하기
	if (IsLocallyControlled()) {
		GetCharacterMovement()->MaxAcceleration = 2048;
	}
	
	// 타이머 종료
	/*GetWorld()->GetTimerManager().ClearTimer(delayHandle);*/
}

/////////////////////////////////////////////////////// R 스킬 시작
// Call Bomb 궁극기
void AGunPlayer::R_Action()
{
	Super::R_Action();
	if (bIsAttack == false && curr_R > cool_R) {
		// 마나 없으면 종료
		if (statment.manaPoint < 50) {
			playerFSM->SetPlayerSkillState(playerSkillSet::Idle);
			return;
		}
		// 궁극기 위젯 보이게
		bombWidget->SetVisibility(true);
	}
}
// 서버에 날리기
void AGunPlayer::ServerRPC_R_End_Action_Implementation(FVector endLocation)
{
	MultiRPC_R_End_Action(endLocation);
}
// 클라로 날리기
void AGunPlayer::MultiRPC_R_End_Action_Implementation(FVector endLocation)
{
	endPosition = endLocation;
	R_End_Action();
}
// 궁극기 시작
void AGunPlayer::R_End_Action()
{
	if (bombWidget->GetVisibleFlag() == true || !IsLocallyControlled()) {
		// 쿨타임 초기화
		curr_R = 0;

		// 마나 초기화
		statment.manaPoint -= 50;

		// 스킬이 날아갈 위치 잡아오기
		if (IsLocallyControlled()) {
			bombWidget->SetVisibility(false);
			bombLocation = bombWidget->GetComponentLocation();
		}
		else
		{
			bombLocation = endPosition;
		}

		bIsAttack = true;

		// 방향 전환
		roll = FRotationMatrix::MakeFromX(bombLocation - GetActorLocation()).Rotator();
		roll.Pitch = 0;
		roll.Yaw += -90;
		GetMesh()->SetWorldRotation(roll);
		
		// 스킬 사용하는 소리 출력
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), R_Sound, GetActorLocation());

		// 애니메이션 출력
		/*float delayTime = */PlayAnimMontage(animMontage, 1.0f * statment.attackSpeedPoint, TEXT("CallBomb"));
		/*delayTime *= statment.attackSpeedPoint;*/
		
		// 플레이어 못움직이게
		if (IsLocallyControlled()) {
			GetCharacterMovement()->MaxAcceleration = 0;
		}

		//// 딜레이 함수에 필요한 변수
		//FTimerHandle delayHandle;

		//// 딜레이 함수 호츌
		//FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AGunPlayer::CallBomb, delayHandle, bombLocation);
		//GetWorld()->GetTimerManager().SetTimer(delayHandle, TimerDelegate, delayTime * 0.5f, false);
	}
}

// 딜레이 함수로 불리는 진짜 스킬
void AGunPlayer::CallBomb()
{
	// bomb 액터 소환
	AGunPlayer_CallBomb* bomb = GetWorld()->SpawnActor<AGunPlayer_CallBomb>(callBombFactory, FTransform(bombLocation));
	
	// 소환한 액터 오너 설정
	bomb->SetOwner(this);

	// 거리 계산 후 다르게 딜 넣어주기
	if (UKismetMathLibrary::Vector_Distance(monster->GetActorLocation(), bomb->GetActorLocation()) < 100)
		ServerRPC_AttackBoss(5);
	else if (UKismetMathLibrary::Vector_Distance(monster->GetActorLocation(), bomb->GetActorLocation()) < 400)
		ServerRPC_AttackBoss(3);
	
	// 플레이어가 움직일 수 있게 변경
	if (IsLocallyControlled()) {
		GetCharacterMovement()->MaxAcceleration = 2048;
	}

	// 소리 출력
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), R_Active_Sound, bombLocation);

	// 방향 전환
	//roll = FRotator(0, -90, 0);

	//GetWorld()->GetTimerManager().ClearTimer(delayHandle);
}

////////////////////////////////////////////////////////////////////////////// F 스킬 시작
// 서버에 날림
void AGunPlayer::ServerRPC_F_Action_Implementation(FVector endLocation)
{
	MultiRPC_F_Action(endLocation);
}
// 클라로 뿌림
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
		// 마나 없으면 실행 안되게
		if (statment.manaPoint < 30) {
			playerFSM->SetPlayerSkillState(playerSkillSet::Idle);
			return;
		}

		// 쿨타임 리셋
		curr_F = 0;

		// 마나 소모
		statment.manaPoint -= 30;

		// 공격중을 표시
		bIsAttack = true;

		// 방향 전환
		roll = FRotationMatrix::MakeFromX(endPosition - GetActorLocation()).Rotator();
		roll.Pitch = 0;
		roll.Yaw += -90;
		GetMesh()->SetWorldRotation(roll);

		// 사용 소리 출력
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), F_Sound, GetActorLocation());

		// 애니메이션 출력
		float delay = PlayAnimMontage(animMontage, 1.0f * statment.attackSpeedPoint, TEXT("Bomb"));
		/*delay *= statment.attackSpeedPoint;*/

		//// 딜레이 함수에 필요한 변수
		//FTimerHandle delayHandle;

		//// 딜레이 함수 실행
		//FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AGunPlayer::ThrowBomb, delayHandle);
		//GetWorld()->GetTimerManager().SetTimer(delayHandle, TimerDelegate, delay, false);
	}
}

// 딜레이 후 불리는 실질 스킬 함수
void AGunPlayer::ThrowBomb()
{
	// 어디에서 날라갈지 위치 받아오기
	FTransform bombPosition = GetMesh()->GetSocketTransform(TEXT("Muzzle_02"));

	// 폭탄 생성해서 날리기
	AGunPlayer_Bomb* bomb = GetWorld()->SpawnActor<AGunPlayer_Bomb>(bombFactory, bombPosition);
	bomb->SetOwner(this);

	// 날아갈 위치를 잡아서 폭탄 날리기
	bomb->Fire((endPosition - GetMesh()->GetSocketLocation(TEXT("Muzzle_02"))).GetSafeNormal());
	
	// 방향 잡기
	//roll = FRotator(0, -90, 0);

	//GetWorld()->GetTimerManager().ClearTimer(delayHandle);
}

/////////////////////////////////////////////////////////////////// 회피기
// 서버에 날림
void AGunPlayer::ServerRPC_Ev_Action_Implementation()
{
	MultiRPC_Ev_Action();
}
// 클라에 뿌림
void AGunPlayer::MultiRPC_Ev_Action_Implementation()
{
	Ev_Action();
}

// 회피기
void AGunPlayer::Ev_Action()
{
	Super::Ev_Action();
	if (bIsAttack == false && curr_SP >= cool_SP) {
		// 쿨타임 초기화
		curr_SP = 0;

		// 공격중 표시
		bIsAttack = true;

		// 소리 재생
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), EV_Sound, GetActorLocation());

		// 애니메이션 재생
		evTime = PlayAnimMontage(animFullBodyMontage, 1.0f * statment.attackSpeedPoint, TEXT("Jump"));

		// 날아갈 속도 지정 -  틱에서 변경됨
		GetCharacterMovement()->MaxWalkSpeed = 3500;

		// 날아갈 방향 지정
		evh = h;
		evv = v;
		if (evh == 0 && evv == 0) evv = 1;

		// 로테이션 돌리기
		roll = FRotationMatrix::MakeFromX(GetActorForwardVector() * evv + GetActorRightVector() * evh).Rotator();
		roll.Yaw += -90;
		roll.Pitch = 0;
		GetMesh()->SetWorldRotation(roll);

		// 플레이어 못움직이게
		if(IsLocallyControlled())
			DisableInput(GetWorld()->GetFirstPlayerController());
	}
	
}

////////////////////////////////////////////////////////////////////////////////// 평타
// 서버에 날림
void AGunPlayer::ServerRPC_Normal_Action_Implementation(FVector endLocation)
{
	MultiRPC_Normal_Action(endLocation);
}
// 클라에 뿌림
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
		// 로테이션을 위한 값 변경
		bIsAttack = true;

		// 애니메이션 재생
		PlayAnimMontage(animMontage, 1.0f * statment.attackSpeedPoint, TEXT("Default"));
		
		// 시작 위치와 사거리 계산
		FVector startPosition = GetMesh()->GetBoneLocation(TEXT("pistol"));
		FVector lengthvector = endPosition - startPosition;

		// 방향 전환
		roll = FRotationMatrix::MakeFromX(endPosition - GetActorLocation()).Rotator();
		roll.Pitch = 0;
		roll.Yaw += -90;
		GetMesh()->SetWorldRotation(roll);

		// 공격 라인 트레이스에 필요한 변수
		FHitResult hitInfo;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);

		// 라인 트레이스로 공격
		bool bHitResult = GetWorld()->LineTraceSingleByChannel(hitInfo, startPosition, endPosition, ECC_Visibility, params);
		//DrawDebugLineTraceSingle(GetWorld(), startPosition, endPosition, EDrawDebugTrace::ForDuration, bHitResult, hitInfo, FLinearColor::Red, FLinearColor::Green, 2.0f);
		
		// 맞았으면
		if (bHitResult)
		{
			// 맞은 액터 가져오기
			AActor* HitActor = hitInfo.GetActor();
			// 맞은 액터가 보스인지, 사거리 안에는 들어왔는지 확인
			if (HitActor && HitActor->ActorHasTag("boss") && lengthvector.Length() < normalLength)
			{
				// 공격
				ServerRPC_AttackBoss(1);
			}

			// 공격 성공 이펙트 표시
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), normalParticleSystem, hitInfo.Location);
			attackCount += 1;
		}
		else
		{
			// 공격 실패 이펙트 표시
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), normalParticleSystem, endPosition);
		}
		// 공격 소리 출력
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Normal_Sound, endPosition);
	}
	// 플레이어가 다시 움직일 수 있게 하기
	if (IsLocallyControlled()) {
		GetCharacterMovement()->MaxAcceleration = 2048;
	}
}

// 아이템 구매시 쿨타임 변경되도록
void AGunPlayer::CoolTimePointChange()
{
	Super::CoolTimePointChange();
	cool_Q = 10 * statment.coolTimePoint; // 10
	cool_E = 15 * statment.coolTimePoint; // 15
	cool_F = 5 * statment.coolTimePoint; // 5
	cool_R = 60 * statment.coolTimePoint; // 60
	cool_SP = 5 * statment.coolTimePoint; // 5

}

//////////////////////////////////////////////////////////////////////////// 보스에 데미지 넣기
// 서버에 날림
void AGunPlayer::ServerRPC_AttackBoss_Implementation(float damagePercent)
{
	MultiRPC_AttackBoss(damagePercent);
}
// 클라에 뿌림
void AGunPlayer::MultiRPC_AttackBoss_Implementation(float damagePercent)
{
	AttackBoss(damagePercent);
}

// 보스에 데미지 넣기
void AGunPlayer::AttackBoss(float damagePercent)
{
	UE_LOG(LogTemp, Warning, TEXT("--- attackPoint : %f, damagePercent : %f"), statment.attackPoint, damagePercent);

	// 데미지 계수 계산해서 총 데미지 계산
	float damage = statment.attackPoint * damagePercent;
	// 공격
	monster->DamageProcess(damage);
}

//////////////////////////////////////////////////////// 임시 리스폰을 위한 버튼
// 서버 날림
void AGunPlayer::ServerRPC_Respawn_Implementation()
{
	MultiRPC_Respawn();
}
// 클라에 뿌림
void AGunPlayer::MultiRPC_Respawn_Implementation()
{
	Respawn();
}
// 임시 부활
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
