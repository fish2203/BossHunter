// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//JumpSmash();

	if (isJumpSmash)
	{
		//JumpSmash();

	}

}

void ATestActor::JumpSmash()
{

	//제자리 점프와 이동
	FVector p0 = GetActorUpVector() * 100.0f;
	//방향값을 노말라이즈하자.
	p0.Normalize();
	//보스의 포워드 벡터에서 플레이어 방향으로 돌이 움직이게 하자, 속도, 시간
	FVector p1 = p0 * chargeForwardSpeed * GetWorld()->GetDeltaSeconds();
	//현재 보스위치에 p1를 더하자.
	FVector p2 = GetActorLocation() + p1;
	//방향값을 노말라이즈하자.
	//p2.Normalize();
	//내위치를 p2로 이동시키자.
	SetActorLocation(p2);
	//바닥에 도착후 효과 출력, 범위 내라면 피격

	UE_LOG(LogTemp, Warning, TEXT(" JumpSmashSkill : %f "), p2.Length());

	currTime += GetWorld()->DeltaTimeSeconds;

	if (currTime > 5)
	{
		currTime = 0;
		isJumpSmash = false;
	}
}

