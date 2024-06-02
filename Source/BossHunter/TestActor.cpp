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

	//���ڸ� ������ �̵�
	FVector p0 = GetActorUpVector() * 100.0f;
	//���Ⱚ�� �븻����������.
	p0.Normalize();
	//������ ������ ���Ϳ��� �÷��̾� �������� ���� �����̰� ����, �ӵ�, �ð�
	FVector p1 = p0 * chargeForwardSpeed * GetWorld()->GetDeltaSeconds();
	//���� ������ġ�� p1�� ������.
	FVector p2 = GetActorLocation() + p1;
	//���Ⱚ�� �븻����������.
	//p2.Normalize();
	//����ġ�� p2�� �̵���Ű��.
	SetActorLocation(p2);
	//�ٴڿ� ������ ȿ�� ���, ���� ����� �ǰ�

	UE_LOG(LogTemp, Warning, TEXT(" JumpSmashSkill : %f "), p2.Length());

	currTime += GetWorld()->DeltaTimeSeconds;

	if (currTime > 5)
	{
		currTime = 0;
		isJumpSmash = false;
	}
}

