// Fill out your copyright notice in the Description page of Project Settings.


#include "BossSound.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UBossSound::UBossSound()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	//硅撇矫累 格家府
	ConstructorHelpers::FObjectFinder<USoundBase>tempBattleStartSound(TEXT("/Script/Engine.SoundCue'/Game/BulePrint/Cheezebee/BossSound/sample/Rampage_Ability_Ultimate_Ally.Rampage_Ability_Ultimate_Ally'"));
	if (tempBattleStartSound.Succeeded())
	{
		bossBattleStartSound = tempBattleStartSound.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase>tempBattleStartMusic(TEXT("/Script/Engine.SoundCue'/Game/BulePrint/Cheezebee/BossSound/sample/BossHuntStartMusic__cut_38sec__Cue.BossHuntStartMusic__cut_38sec__Cue'"));
	if (tempBattleStartMusic.Succeeded())
	{
		bossBattleStartMusic = tempBattleStartMusic.Object;
	}



	//叭绰家府	
	ConstructorHelpers::FObjectFinder<USoundBase>tempWalkSound(TEXT("/Script/Engine.SoundWave'/Game/BulePrint/Cheezebee/BossSound/sample/BossWalkSound.BossWalkSound'"));
	if (tempBattleStartSound.Succeeded())
	{
		bossWalkSound = tempWalkSound.Object;
	}
	//戚摹家府
	ConstructorHelpers::FObjectFinder<USoundBase>tempPunchSound(TEXT("/Script/Engine.SoundWave'/Game/BulePrint/Cheezebee/BossSound/sample/BossPunchSound__cut_1sec_.BossPunchSound__cut_1sec_'"));
	if (tempPunchSound.Succeeded())
	{
		bossPunchSound = tempPunchSound.Object;
	}

	//戚摹格家府
	ConstructorHelpers::FObjectFinder<USoundBase>tempPunchVoice(TEXT("/Script/Engine.SoundWave'/Game/BulePrint/Cheezebee/BossSound/sample/Sound_BossPunch.Sound_BossPunch'"));
	if (tempPunchVoice.Succeeded())
	{
		bossPunchVoice = tempPunchVoice.Object;
	}

	//瞒瘤格家府
	ConstructorHelpers::FObjectFinder<USoundBase>tempChargeVoice(TEXT("/Script/Engine.SoundWave'/Game/BulePrint/Cheezebee/BossSound/sample/BossChargeVoice.BossChargeVoice'"));
	if (tempChargeVoice.Succeeded())
	{
		bossChargeVoice = tempChargeVoice.Object;
	}

	//瞒瘤家府
	ConstructorHelpers::FObjectFinder<USoundBase>tempChargeSound(TEXT("/Script/Engine.SoundWave'/Game/BulePrint/Cheezebee/BossSound/sample/BossChargeSound__cut_0sec_.BossChargeSound__cut_0sec_'"));
	if (tempChargeSound.Succeeded())
	{
		bossChargeSound = tempChargeSound.Object;
	}

	//静肺快 格家府
	ConstructorHelpers::FObjectFinder<USoundBase>tempStoneThrowVoiece(TEXT("/Script/Engine.SoundCue'/Game/BulePrint/Cheezebee/BossSound/sample/BossThrowVoice__cut_1sec__Cue.BossThrowVoice__cut_1sec__Cue'"));
	if (tempStoneThrowVoiece.Succeeded())
	{
		bossStoneThrowVoiece = tempStoneThrowVoiece.Object;
	}

	//静肺快 家府
	ConstructorHelpers::FObjectFinder<USoundBase>tempStoneThrowSound(TEXT("/Script/Engine.SoundCue'/Game/BulePrint/Cheezebee/BossSound/sample/BossThrowSound__cut_2sec___cut_0sec__Cue.BossThrowSound__cut_2sec___cut_0sec__Cue'"));
	if (tempStoneThrowSound.Succeeded())
	{
		bossStoneThrowSound = tempStoneThrowSound.Object;
	}

	//胶概浆 家府
	ConstructorHelpers::FObjectFinder<USoundBase>tempSmashSound(TEXT("/Script/Engine.SoundCue'/Game/BulePrint/Cheezebee/BossSound/sample/BossSmashSound__cut_2sec__Cue.BossSmashSound__cut_2sec__Cue'"));
	if (tempSmashSound.Succeeded())
	{
		bossSmashSound = tempSmashSound.Object;
	}
	//胶概浆 格家府
	ConstructorHelpers::FObjectFinder<USoundBase>tempSmashVoice(TEXT("/Script/Engine.SoundCue'/Game/BulePrint/Cheezebee/BossSound/sample/Rampage_Ability_Ultimate_Self.Rampage_Ability_Ultimate_Self'"));
	if (tempSmashVoice.Succeeded())
	{
		bossSmashVoice = tempSmashVoice.Object;
	}


	//掉饭捞 格家府
	ConstructorHelpers::FObjectFinder<USoundBase>tempDelayVoice(TEXT("/Script/Engine.SoundCue'/Game/BulePrint/Cheezebee/BossSound/sample/BossDelayVoice__cut_1sec__Cue.BossDelayVoice__cut_1sec__Cue'"));
	if (tempDelayVoice.Succeeded())
	{
		bossDelayVoice = tempDelayVoice.Object;
	}


	//掉饭捞 格家府
	ConstructorHelpers::FObjectFinder<USoundBase>tempClearSound(TEXT("/Script/Engine.SoundCue'/Game/BulePrint/Cheezebee/BossSound/sample/BossDieVoice.BossDieVoice'"));
	if (tempClearSound.Succeeded())
	{
		bossDieVoice = tempClearSound.Object;
	}

}


// Called when the game starts
void UBossSound::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBossSound::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UBossSound::playbossBattleStartSound()
{
	UGameplayStatics::PlaySound2D(GetWorld(), bossBattleStartSound);
}

void UBossSound::playbossBattleStartMusic()
{
	UGameplayStatics::PlaySound2D(GetWorld() , bossBattleStartMusic);
}


void UBossSound::playbossWalkSound()
{
	
	UGameplayStatics::PlaySound2D(GetWorld(), bossWalkSound);
}

void UBossSound::PlaybossPunchSound()
{
	//UGameplayStatics
	UGameplayStatics::PlaySound2D(GetWorld(), bossPunchSound);
	
}

void UBossSound::PlaybossPunchVoice()
{
	UGameplayStatics::PlaySound2D(GetWorld(), bossPunchVoice);
}

void UBossSound::playbossStoneThrowSound()
{
	UGameplayStatics::PlaySound2D(GetWorld(), bossStoneThrowSound, 0.3, 0.1);
}


void UBossSound::playbossStoneThrowVoice()
{
	UGameplayStatics::PlaySound2D(GetWorld(), bossStoneThrowVoiece);
}

void UBossSound::playbossChargeSound()
{
	UGameplayStatics::PlaySound2D(GetWorld(), bossChargeSound, 0.1 , 0.1 , 0.1);
}

void UBossSound::playbossChargeVoice()
{
	UGameplayStatics::PlaySound2D(GetWorld(), bossChargeVoice);
}

void UBossSound::playbossSmashSound()
{
	UGameplayStatics::PlaySound2D(GetWorld(), bossSmashSound);
}

void UBossSound::playbossSmashVoice()
{
	UGameplayStatics::PlaySound2D(GetWorld(), bossSmashVoice);
	
}

void UBossSound::playbossDelayVoice()
{
	UGameplayStatics::PlaySound2D(GetWorld(), bossDelayVoice);
}

void UBossSound::playbossDieVoice()
{
	UGameplayStatics::PlaySound2D(GetWorld(), bossDieVoice);
}



