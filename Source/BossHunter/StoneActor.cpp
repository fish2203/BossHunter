// Fill out your copyright notice in the Description page of Project Settings.


#include "StoneActor.h"
#include <Components/BoxComponent.h>
#include "TestPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "GunPlayer.h"
#include "Test_Boss.h"
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>

// Sets default values
AStoneActor::AStoneActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box comp"));
	SetRootComponent(boxComp);
	//boxComp->SetupAttachment(RootComponent);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	meshComp->SetupAttachment(RootComponent);

	// 테스트 폭발 효과 가져오자
	//ConstructorHelpers::FObjectFinder<UParticleSystem> tempEffect(TEXT("/Script/Engine.ParticleSystem'/Game/BulePrint/Cheezebee/Test_Particle.Test_Particle'"));
	
	//폭발 효과 아이스를 가져오자
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempEffect(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonRampage/FX/Particles/Rampage_v001_IceBlue/FX/P_Rampage_Ice_Enraged_Impact.P_Rampage_Ice_Enraged_Impact'"));

	//폭발 효과를 세팅
	if (tempEffect.Succeeded())
	{
		exploEffect = tempEffect.Object;
	}
	
}

// Called when the game starts or when spawned
void AStoneActor::BeginPlay()
{
	Super::BeginPlay();


	// 게임을 시작하면 플레이어를 찾자, UGameplayStatics은 게임 시작할때만 사용가능
	//UGameplayStatics 인클루드
	//ATestPlayer 인클루드
	//AGunPlayer 인클루드
	
	//게임에서 플레이를 찾아라.
	AActor* findPlayer = UGameplayStatics::GetActorOfClass(GetWorld(), AGunPlayer::StaticClass());
	target = Cast<AGunPlayer>(findPlayer);

	//게임에서 보스를 찾아라.
	AActor* findBoss = UGameplayStatics::GetActorOfClass(GetWorld(), ATest_Boss::StaticClass());
	bossActor = Cast<ATest_Boss>(findBoss);
	
	//매쉬를 안보이게 하자.
	meshComp->SetActive(false);

}

// Called every frame
void AStoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isThorw == true)
	{
	StoneMove();
	}

}

void AStoneActor::StoneSpawn()
{	
	// 총알 생성
	/*AStoneActor* stoneBullet = GetWorld()->SpawnActor<AStoneActor>(bossActor->stoneFactory, bossActor->GetActorLocation(), FRotator::ZeroRotator);*/
	GetWorld()->SpawnActor<AStoneActor>(bossActor->stoneFactory, bossActor->GetActorLocation(), FRotator::ZeroRotator);
}

void AStoneActor::StoneFire()
{	
	FVector bossPos = bossActor->GetActorLocation();
	// Overlap 충돌이 되었을 때 호출되는 함수 등록 (Delegate = 함수를 담을수 있는 변수)
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AStoneActor::OnOverlap);

}


void AStoneActor::StoneMove()
{
	meshComp->SetActive(true);
	meshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	//플레이어의 위치를 구하자.
	FVector playerPos = target->GetActorLocation();
	//보스 위치를 구하자.
	FVector bossPos = bossActor->GetActorLocation();
	//내 위치를 빼자.
	dir = playerPos - GetActorLocation();
	dir.Normalize();

	// 플레이어를 추적하는 돌맹이
	//내위치+ 방향값(타겟 방향 * 이동속도 * 시간값)
	/*FVector p0 = GetActorLocation();
	FVector vt = dir * moveSpeed * GetWorld()->GetDeltaSeconds();
	FVector p = p0 + vt;
	SetActorLocation(p);*/

	//로그를 찍자.
	//UE_LOG(LogTemp, Warning, TEXT("stone actor StoneFire chase"));

	//플레이어 방향으로 전진하는 돌맹이
	
	//플레이어 위치에서 보스위치를 방향을 빼자
	FVector p0 = playerPos - bossPos;
	//방향값을 노말라이즈하자.
	p0.Normalize();
	//보스의 포워드 벡터에서 플레이어 방향으로 돌이 움직이게 하자, 속도, 시간
	FVector p1 = p0 * moveSpeed * GetWorld()->GetDeltaSeconds();
	//내 위치에 p1를 더하자.
	FVector p2 = GetActorLocation() + p1;
	//내위치를 p2로 이동시키자.
	SetActorLocation(p2);

	//출력하기
	//UE_LOG(LogTemp, Warning, TEXT("stone actor StoneFire foward"));

	//디버그 라인 그리기
	DrawDebugLine(GetWorld(), bossPos, playerPos, FColor::Red);

	//시간누적
	currTime += GetWorld()->GetDeltaSeconds();
	//2초지나면 액터 파괴
	if (currTime > 2)
	{
		currTime = 0;
		isThorw = false;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), exploEffect, GetActorLocation(), GetActorRotation());
		Destroy();
		
	}


}

void AStoneActor::SetAcitve(bool isActive)
{
	// 만약에 isActive 가 true 면
	if (isActive == true)
	{
		// 모양(mesh) 를 보이게 하자
		meshComp->SetVisibility(isActive);
		// 충돌 할 수 있게 하자
		meshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	// 그렇지 않으면
	else
	{
		// 모양(mesh) 를 안보이게 하자
		meshComp->SetVisibility(isActive);
		// 충돌 하지 않게 하자
		meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AStoneActor::OnOverlap(UPrimitiveComponent* abc, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

		if (OtherActor->GetName().Contains(TEXT("player")))
		{
			FVector bossPos = bossActor->GetActorLocation();
			SetActorLocation(bossPos);

			//효과를 스폰하자
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), exploEffect, GetActorLocation(), GetActorRotation());

			isstoneHit = false;

			//Destroy();



			/*UParticleSystemComponent* stoneEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), exploEffect, GetActorLocation(), GetActorRotation());

			AActor* OwnerActor = stoneEffect->GetOwner();
			*/
			UE_LOG(LogTemp, Warning, TEXT("stone actor, OnOverlap stoneEffect"));

			/*currTime += GetWorld()->GetDeltaSeconds();

			if (currTime > 2)
			{
				currTime = 0;
				OwnerActor->Destroy();
			}*/

		}
		else
		{
			isstoneHit = false;
		}
	}


