// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWidget.h"
#include <Kismet/GameplayStatics.h>
#include "GunPlayer.h"
#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>
#include "Test_Boss.h"
#include "BossFsmTest.h"


void UPlayerWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
	player = Cast<ABossHunterCharacter>(GetOwningPlayer()->GetPawn());
    if (player != nullptr)
    {
        gunPlayer = Cast<AGunPlayer>(player);
        if (gunPlayer) {
            HPBarText->SetText(FText::FromString(FString::Printf(TEXT("%0.2f / %0.2f"), gunPlayer->statment.healthPoint, gunPlayer->statment.fullHP)));
            MPBarText->SetText(FText::FromString(FString::Printf(TEXT("%0.2f / %0.2f"), gunPlayer->statment.manaPoint, gunPlayer->statment.fullMP)));
            statText->SetText(FText::FromString(FString::Printf(TEXT("AP : %0.2f\nDP : %0.2f\nASP : %0.2f\nMSP : %0.2f\nCP : %0.2f\n"), gunPlayer->statment.attackPoint, gunPlayer->statment.defancePoint, gunPlayer->statment.attackSpeedPoint, gunPlayer->statment.moveSpeedPoint, gunPlayer->statment.coolTimePoint)));
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

    //float maxbossHP = 3;
    //float currbossHP = 0;
    
    //임시로 보스를 찾을때
  /*  AActor * BossActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATest_Boss::StaticClass());
    monster = Cast<ATest_Boss>(BossActor);*/

    monster->currbossHP = monster->maxbossHP;
}

void UPlayerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    if (monster) {
        MonsterHP->SetPercent(monster->currbossHP / monster->maxbossHP);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Player Widget: No Monster"));
    }
    if (gunPlayer) {
        HPBarText->SetText(FText::FromString(FString::Printf(TEXT("%0.2f / %0.2f"), gunPlayer->statment.healthPoint, gunPlayer->statment.fullHP)));
        MPBarText->SetText(FText::FromString(FString::Printf(TEXT("%0.2f / %0.2f"), gunPlayer->statment.manaPoint, gunPlayer->statment.fullMP)));
        statText->SetText(FText::FromString(FString::Printf(TEXT("AP : %0.2f\nDP : %0.2f\nASP : %0.2f\nMSP : %0.2f\nCP : %0.2f\n"), gunPlayer->statment.attackPoint, gunPlayer->statment.defancePoint, gunPlayer->statment.attackSpeedPoint, gunPlayer->statment.moveSpeedPoint, gunPlayer->statment.coolTimePoint)));
        HealthBar->SetPercent(gunPlayer->statment.healthPoint / gunPlayer->statment.fullHP);
        ManaBar->SetPercent(gunPlayer->statment.manaPoint / gunPlayer->statment.fullMP);
        Q_Btn->SetPercent(gunPlayer->curr_Q / gunPlayer->cool_Q);
        E_Btn->SetPercent(gunPlayer->curr_E / gunPlayer->cool_E);
        F_Btn->SetPercent(gunPlayer->curr_F / gunPlayer->cool_F);
        R_Btn->SetPercent(gunPlayer->curr_R / gunPlayer->cool_R);

        if (Q_Text->IsHovered()) {
            ExplainText->SetText(FText::FromString(FString::Printf(TEXT("무기 교체\n on/off가 가능하며 무기를 바꿀 경우 전체 스킬 및 평타 둔화, 공격력 상승 효과가 있습니다."))));
        }
        else if (E_Text->IsHovered()) {
            ExplainText->SetText(FText::FromString(FString::Printf(TEXT("레이저\n 맞추면 2초간 몬스터 둔화 효과를 주며, 2*%0.1f의 피해를 입힙니다."), gunPlayer->statment.attackPoint)));
        }
        else if (F_Text->IsHovered()) {
            ExplainText->SetText(FText::FromString(FString::Printf(TEXT("폭탄\n 소형 폭탄을 던집니다, 3*%0.1f의 피해를 입힙니다."), gunPlayer->statment.attackPoint)));
        }
        else if (R_Text->IsHovered()) {
            ExplainText->SetText(FText::FromString(FString::Printf(TEXT("핵폭탄\n 정 가운데를 맞추면 5*%0.1f의 피해, 외곽에 맞출 경우 3*%0.1f의 피해를 입히며 2초간 지속피해를 입힙니다."), gunPlayer->statment.attackPoint, gunPlayer->statment.attackPoint)));
        }
        else {
            ExplainText->SetText(FText::FromString(FString::Printf(TEXT(" "))));
        }
    }
}

void UPlayerWidget::DamageProgressBoss(float damage)
{
    // HP 를 줄이자
    monster->currbossHP -= damage;

    // HPBar progress Percent 값 설정 (0 ~ 1)
    //UUserWidget* widget = this;
    //UPlayerWidget* playerWidget = Cast<UPlayerWidget>(widget);
    //#include <Components/ProgressBar.h>
    //playerWidget->
    MonsterHP->SetPercent((float)monster->currbossHP / monster->maxbossHP);

    UE_LOG(LogTemp, Warning, TEXT("player widget monseter HP : %f"), monster-> currbossHP);

    if (monster->currbossHP < 0)
    {
        
       //ABossFsmTest
        monsterFsm->ChangeState(EEnemyState::DIE);
    }

    //만약에 HP 가 0보다 크면
    //if (currHP > 0)
    //{
    // DAMAGE 상태로 전환
    //    fsm->ChangeState(EEnemyState::DAMAGE);
    //}
    // 그렇지 않으면 (HP 가 0)
    //else
    //{
    //    // DIE 로 상태로 전환
    //    fsm->ChangeState(EEnemyState::DIE);
    //}
}

