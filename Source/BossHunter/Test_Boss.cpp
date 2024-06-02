// Fill out your copyright notice in the Description page of Project Settings.


#include "Test_Boss.h"
#include "Boss/BossFsmTest.h"
#include "TestPlayer.h"
#include "Camera/CameraComponent.h"
#include "Boss/BossHPBarTest.h"
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
#include "BossDropItem.h"
#include "BossWidgetComponent.h"



ATest_Boss::ATest_Boss()

{
	PrimaryActorTick.bCanEverTick = true;

	// Skeletal Mesh �о����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonRampage/Characters/Heroes/Rampage/Meshes/Rampage.Rampage'"));
	if (tempMesh.Succeeded())
	{
		// Mesh �� Skeletal Mesh ����
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}

	// �޽��� ��ġ/ȸ�� ���� ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, 270, 0));
	//���� �������� 2�� ��������.
	GetMesh()->SetWorldScale3D(FVector(2.5));

	//�ִϸ��̼� �������Ʈ�� ��������. fsm �ʱ�ȭ �Ҷ����� ��Ȱ��ȭ ��.
	ConstructorHelpers::FClassFinder<UAnimInstance>tempTestBossAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/BulePrint/Cheezebee/ABP_TestBoss.ABP_TestBoss_C'"));
	if (tempTestBossAnim.Succeeded())
	{
		GetMesh()->SetAnimClass(tempTestBossAnim.Class);
	}
	
	//�±׿� ������ �߰�����. �÷��̾ ������ ã���� �±׷� ã��.
	Tags.Add(TEXT("boss"));
	
	// FSM ���� ������Ʈ �߰�
	//UBossFsmTest ��Ŭ���
	fsm = CreateDefaultSubobject<UBossFsmTest>(TEXT("Boss FSM"));
	bossSoundComp = CreateDefaultSubobject<UBossSound>(TEXT("Boss Sound Comp"));

	// Widget ������Ʈ �߰�
	//UWidgetComponent ��Ŭ���
	comHpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("BP_BAR"));
	comHpBar->SetupAttachment(RootComponent);
	comHpBar->SetDrawSize(FVector2D(100, 30));
	comHpBar->SetRelativeLocation(FVector(0, 0, 180));
	comHpBar->SetRelativeRotation(FRotator(0,180,0));

	// HP Bar Widget �ҷ��ͼ� ����
	//UBossHPBarTest ��Ŭ���
	ConstructorHelpers::FClassFinder<UBossHPBarTest> tempHPBar(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BulePrint/Cheezebee/WBP_BossHpBarTest.WBP_BossHpBarTest_C'"));
	if (tempHPBar.Succeeded())
	{
		comHpBar->SetWidgetClass(tempHPBar.Class);
	}

	// �̵� �������� ���� ȸ�� ������ �ɼ� Ȱ��ȭ
	//UCharacterMovementComponent ��Ŭ���
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Capsule ������Ʈ CollisonPreset = EnemyProfile
	// Mesh ������Ʈ CollisionPreset = NoCollision
	//UCapsuleComponent ��Ŭ���
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyProfile"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	//Auto Possess ai ���� (spawn, placed �Ѵ� �����ϰ�)
	//AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	smashRange = CreateDefaultSubobject<USphereComponent>(TEXT("SmashRange"));
	smashRange->SetupAttachment(RootComponent);
	smashRange->SetWorldScale3D(FVector(10));
	smashRange->SetWorldLocation(FVector(0,0,-70));
	//SetActorLocation(FVector(1600, 19000, 25800));

	FVector bossPos = GetActorLocation();

	//�Ѿ� ������ ��������.
	ConstructorHelpers::FClassFinder<AStoneActor> tempFactory(TEXT("/Script/Engine.Blueprint'/Game/BulePrint/Cheezebee/BP_StoneActor.BP_StoneActor_C'"));
	if (tempFactory.Succeeded())
	{
		stoneFactory = tempFactory.Class;
	}

	//<Components / BoxComponent.h>
	//������ �����տ� ��ġ �ݸ����� ������.
	righthandBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightPunchBox"));
	//righthandBox -> SetupAttachment(GetMesh());
	//������ ��ġ �ݸ��� �̸�
	FName rightHandSoketName = FName(TEXT("rightHand03"));
	righthandBox->SetupAttachment(GetMesh(), rightHandSoketName);
	//��ġ�� 0���ͷ� ����
	righthandBox->SetRelativeLocation(FVector(0));
	//�ڽ�ũ������
	righthandBox->SetBoxExtent(FVector(20));


	dropItemPos = CreateDefaultSubobject<UBoxComponent>(TEXT("Drop Item Pos"));
	dropItemPos -> SetupAttachment(RootComponent);
	dropItemPos -> SetRelativeLocation(FVector(0,0,200));

	//UBossWidgetComponent
	//�������� ������Ʈ �߰�
	bossWidget = CreateDefaultSubobject <UBossWidgetComponent> (TEXT(" Boss widget "));


	//SnapToTargetNotIncludingScale, rightHandSoketName);

}

void ATest_Boss::BeginPlay()
{
	//�������̵� �� �Լ��� �̰� ���ϸ� ƽ�� �������� ����
	Super::BeginPlay();	
	
	// ���� HP �� maxHP �� ����
	currbossHP = maxbossHP;


	
	//�������.
	//UE_LOG(LogTemp, Warning, TEXT(" beginplay boss hp"));

	//�Ѿ��� ��ġ����.
	//GetWorld()->SpawnActor<AStoneActor>(stoneFactory, GetActorLocation(),GetActorRotation());

	// źâ�� �Ѿ��� ���� ����
	//for (int32 i = 0; i < 5; i++)
	//{
	//	// 1. �Ѿ��� �����Ѵ�.
	//	AStoneActor* stoneBullet = GetWorld()->SpawnActor<AStoneActor>(stoneFactory);
	//	// 2. ������ �Ѿ��� źâ�� �ִ´�.
	//	magazine.Add(stoneBullet);
	//	// 3. ������ �Ѿ��� ��Ȱ��ȭ �Ѵ�.
	//	stoneBullet->SetAcitve(false);
	//	// 4. ������ �Ѿ��� ������ �ִ� Delegate �� InsertBullet �Լ��� ���
	//	stoneBullet->insertMagazineDel.BindUObject(this, &ATest_Boss::InsertBullet);

	//}


}

void ATest_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���ο쿡 �ɷ����� 5�� �Ŀ� Ǯ�� - JINA
	if (GetCharacterMovement()->MaxWalkSpeed < 550) {
		// �ð� ���
		slowTime += DeltaTime;
		// �ð��� 5�ʸ� �Ѿ��
		if (slowTime >= 5.0f) {
			// �ð� ��� ���� �ʱ�ȭ
			slowTime = 0;
			// �̵� �ӵ� ����
			GetCharacterMovement()->MaxWalkSpeed = 550;
		}
	}

	if (fsm->isJumpSmash)

	{
		fsm->JumpSmashSkill();
		
	}

	if(fsm->isChaseJumpSmash)
	{
		fsm->ChaseJumpSmashSkill();
	}




	//UE_LOG(LogTemp, Warning, TEXT(" tick "));

	//include AGunPlayer
	//fsm ������ ���� ���޴� ���, ���ӿ� ��ġ�� ������ fsm�� ����Ǿ��ִ��� Ȯ���ؾ��մϴ�.
	//if (fsm->target != nullptr) //!= nullptr
	//{
		// #include "TestPlayer.h"
		// #include <Camera/CameraComponent.h>
		// ī�޶��� �չ���
		//FVector camForward = fsm->target->FollowCamera->GetForwardVector();
		// ī�޶��� ������
		//FVector camUp = fsm->target->FollowCamera->GetUpVector();
		// ī�޶��� �չ��� �ݴ�, �������� ���ϴ� ������ �����ּ���
		//UKismetMathLibrary ��Ŭ���
		//FRotator rot = UKismetMathLibrary::MakeRotFromXZ(-camForward, camUp);
		// rot �� ������ comHPBar �� ����(����)�� ����
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
	// źâ�� bullet �ִ´�.
	magazine.Add(bullet);

	// �Ѿ��� ��ȭ��ȭ
	bullet->SetAcitve(false);
}

void ATest_Boss::DamageProcess(float damage)
{
	// ���� HP �� ������
	currbossHP -= damage;

	// HPBar progress Percent �� ���� (0 ~ 1)
	UUserWidget* widget = comHpBar->GetWidget();
	UBossHPBarTest* hpBarWidget = Cast<UBossHPBarTest>(widget);
	//#include <Components/ProgressBar.h>
	hpBarWidget->HPBar->SetPercent((float)currbossHP / maxbossHP);

	// ���࿡ HP �� 0���� ũ��
	if (currbossHP > 0)
	{
		// DAMAGE ���·� ��ȯ
		//fsm->ChangeState(EEnemyState::DAMAGE);
	}
	// �׷��� ������ (HP �� 0)
	else
	{
		// DIE �� ���·� ��ȯ
		fsm->ChangeState(EEnemyState::DIE);
	}

}

void ATest_Boss::AttackTarget()
{
	//fsm->target->DamageProcess(50);

}

void ATest_Boss::DropItem()
{
	
	if (isDropItem == true) return;
	
	//������ �����Ѵ�.
	GetWorld()->SpawnActor<ABossDropItem>(this->dropItemPos->GetComponentLocation(), FRotator::ZeroRotator);
	
	//bossDropItem->clearChest->SetVisibility(true);
	isDropItem = true;
	UE_LOG(LogTemp, Warning, TEXT("Drop Item"));

}

void ATest_Boss::ReSpawnBoss()
{
	currbossHP = 4000;
	fsm->ChangeState(EEnemyState::DIE);
}

void ATest_Boss::DieBoss()
{

}
