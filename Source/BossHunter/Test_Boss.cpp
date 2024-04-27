// Fill out your copyright notice in the Description page of Project Settings.


#include "Test_Boss.h"
#include "BossFsmTest.h"
#include "TestPlayer.h"
#include "Camera/CameraComponent.h"
#include "BossHPBarTest.h"
#include <Components/WidgetComponent.h>
#include <Components/ProgressBar.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/CapsuleComponent.h>
#include "GunPlayer.h"
#include <Components/SphereComponent.h>
#include "StoneActor.h"
#include <Kismet/GameplayStatics.h>
#include <Components/BoxComponent.h>


ATest_Boss::ATest_Boss()

{
	PrimaryActorTick.bCanEverTick = true;

	//애니메이션 블루프린트를 세팅하자. fsm 초기화 할때마다 비활성화 됨.
	ConstructorHelpers::FClassFinder<UAnimInstance>tempTestBossAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/BulePrint/Cheezebee/ABP_TestBoss.ABP_TestBoss_C'"));
	if (tempTestBossAnim.Succeeded())
	{
		GetMesh()->SetAnimClass(tempTestBossAnim.Class);
	}
	
	//보스 스케일을 2로 설정하자.
	GetMesh()->SetWorldScale3D(FVector(4));
	//태그에 보스를 추가하자. 플레이어가 보스를 찾을때 태그로 찾음.
	Tags.Add(TEXT("boss"));
	//righthandBOx
	//<Components / BoxComponent.h>
	righthandBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightPunchBox"));
	//소켓이름을 받아오자
	FName rightHandSoketName = FName(TEXT("rightHand03"));
	//컴포넌트의 매쉬의 소켓이름에 붙이자
	righthandBox -> AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, rightHandSoketName);
	

	// Skeletal Mesh 읽어오자
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/BulePrint/Cheezebee/Rampage.Rampage'"));
	if (tempMesh.Succeeded())
	{
		// Mesh 에 Skeletal Mesh 셋팅
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}

	// 메쉬의 위치/회전 값을 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, 270, 0));

	// FSM 액터 컴포넌트 추가
	//UBossFsmTest 인클루드
	fsm = CreateDefaultSubobject<UBossFsmTest>(TEXT("Boss FSM"));

	// Widget 컴포넌트 추가
	//UWidgetComponent 인클루드
	comHpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("BP_BAR"));
	comHpBar->SetupAttachment(RootComponent);
	comHpBar->SetDrawSize(FVector2D(100, 30));
	comHpBar->SetRelativeLocation(FVector(0, 0, 180));
	comHpBar->SetRelativeRotation(FRotator(0,180,0));

	// HP Bar Widget 불러와서 셋팅
	//UBossHPBarTest 인클루드
	ConstructorHelpers::FClassFinder<UBossHPBarTest> tempHPBar(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BulePrint/Cheezebee/WBP_BossHpBarTest.WBP_BossHpBarTest_C'"));
	if (tempHPBar.Succeeded())
	{
		comHpBar->SetWidgetClass(tempHPBar.Class);
	}

	// 이동 방향으로 나를 회전 시켜줘 옵션 활성화
	//UCharacterMovementComponent 인클루드
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Capsule 컴포넌트 CollisonPreset = EnemyProfile
	// Mesh 컴포넌트 CollisionPreset = NoCollision
	//UCapsuleComponent 인클루드
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyProfile"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	//Auto Possess ai 설정 (spawn, placed 둘다 동작하게)
	//AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	smashRange = CreateDefaultSubobject<USphereComponent>(TEXT("SmashRange"));
	smashRange->SetupAttachment(RootComponent);
	smashRange->SetWorldScale3D(FVector(10));
	smashRange->SetWorldLocation(FVector(0,0,-70));

	FVector bossPos = GetActorLocation();


	//총알 공장을 가져오자.
	ConstructorHelpers::FClassFinder<AStoneActor> tempFactory(TEXT("/Script/Engine.Blueprint'/Game/BulePrint/Cheezebee/BP_StoneActor.BP_StoneActor_C'"));
	if (tempFactory.Succeeded())
	{
		stoneFactory = tempFactory.Class;
	}

}

void ATest_Boss::BeginPlay()
{
	//오버라이드 한 함수에 이걸 안하면 틱이 동작하지 않음
	Super::BeginPlay();
	//위치를 0벡터로 고정
	righthandBox->SetRelativeLocation(FVector(0));
	//박스크기조정
	righthandBox->SetBoxExtent(FVector(20));


	
	// 현재 HP 를 maxHP 로 하자
	currbossHP = maxbossHP;
	
	//출력하자.
	//UE_LOG(LogTemp, Warning, TEXT(" beginplay boss hp"));

	//총알을 배치하자.
	//GetWorld()->SpawnActor<AStoneActor>(stoneFactory, GetActorLocation(),GetActorRotation());

	// 탄창에 총알을 만들어서 넣자
	//for (int32 i = 0; i < 5; i++)
	//{
	//	// 1. 총알을 생성한다.
	//	AStoneActor* stoneBullet = GetWorld()->SpawnActor<AStoneActor>(stoneFactory);
	//	// 2. 생성된 총알을 탄창에 넣는다.
	//	magazine.Add(stoneBullet);
	//	// 3. 생성된 총알을 비활성화 한다.
	//	stoneBullet->SetAcitve(false);
	//	// 4. 생성된 총알이 가지고 있는 Delegate 에 InsertBullet 함수를 등록
	//	stoneBullet->insertMagazineDel.BindUObject(this, &ATest_Boss::InsertBullet);

	//}


}

void ATest_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 슬로우에 걸렸을때 5초 후에 풀자 - JINA
	if (GetCharacterMovement()->MaxWalkSpeed < 550) {
		// 시간 재기
		slowTime += DeltaTime;
		// 시간이 5초를 넘어가면
		if (slowTime >= 5.0f) {
			// 시간 재는 변수 초기화
			slowTime = 0;
			// 이동 속도 복구
			GetCharacterMovement()->MaxWalkSpeed = 550;
		}
	}


	//UE_LOG(LogTemp, Warning, TEXT(" tick "));

	//include AGunPlayer
	//fsm 오류로 값을 못받는 경우, 게임에 배치된 보스에 fsm이 적용되어있는지 확인해야합니다.
	//if (fsm->target != nullptr) //!= nullptr
	//{
		// #include "TestPlayer.h"
		// #include <Camera/CameraComponent.h>
		// 카메라의 앞방향
		//FVector camForward = fsm->target->FollowCamera->GetForwardVector();
		// 카메라의 윗방향
		//FVector camUp = fsm->target->FollowCamera->GetUpVector();
		// 카메라의 앞방향 반대, 윗방향을 향하는 각도를 구해주세요
		//UKismetMathLibrary 인클루드
		//FRotator rot = UKismetMathLibrary::MakeRotFromXZ(-camForward, camUp);
		// rot 의 값으로 comHPBar 의 각도(월드)로 셋팅
		//comHpBar->SetWorldRotation(rot);

		//UE_LOG(LogTemp, Warning, TEXT(" set hp widget Location"));
	//}

	/*else

	{
		UE_LOG(LogTemp, Warning, TEXT(" test boss tick no player"));
	}*/

}

void ATest_Boss::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

}

void ATest_Boss::InsertBullet(class AStoneActor* bullet)
{
	// 탄창에 bullet 넣는다.
	magazine.Add(bullet);

	// 총알을 비화성화
	bullet->SetAcitve(false);
}

void ATest_Boss::DamageProcess(float damage)
{
	// 나의 HP 를 줄이자
	currbossHP -= damage;

	// HPBar progress Percent 값 설정 (0 ~ 1)
	UUserWidget* widget = comHpBar->GetWidget();
	UBossHPBarTest* hpBarWidget = Cast<UBossHPBarTest>(widget);
	//#include <Components/ProgressBar.h>
	hpBarWidget->HPBar->SetPercent((float)currbossHP / maxbossHP);

	// 만약에 HP 가 0보다 크면
	if (currbossHP > 0)
	{
		// DAMAGE 상태로 전환
		//fsm->ChangeState(EEnemyState::DAMAGE);
	}
	// 그렇지 않으면 (HP 가 0)
	else
	{
		// DIE 로 상태로 전환
		fsm->ChangeState(EEnemyState::DIE);
	}

}

void ATest_Boss::AttackTarget()
{
	//fsm->target->DamageProcess(50);

}
