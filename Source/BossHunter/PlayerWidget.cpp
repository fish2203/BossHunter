// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWidget.h"
#include <Kismet/GameplayStatics.h>
#include "GunPlayer.h"
#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>
#include "Test_Boss.h"
#include "Boss/BossFsmTest.h"
#include "BossHunterGameMode.h"
#include "BossRoomGameStateBase.h"
#include <Components/Button.h>
#include <Components/Image.h>
#include "RestartBossZone.h"
#include "GateBattleBoss.h"
//UGameplayStatics



void UPlayerWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    // player 찾기
	player = Cast<ABossHunterCharacter>(GetOwningPlayer()->GetPawn());
    if (player != nullptr)
    {
        gunPlayer = Cast<AGunPlayer>(player);
        if (gunPlayer) {
            HPBarText->SetText(FText::FromString(FString::Printf(TEXT("%0.0f / %0.0f"), gunPlayer->statment.healthPoint, gunPlayer->statment.fullHP)));
            MPBarText->SetText(FText::FromString(FString::Printf(TEXT("%0.0f / %0.0f"), gunPlayer->statment.manaPoint, gunPlayer->statment.fullMP)));
            statText->SetText(FText::FromString(FString::Printf(TEXT("AP : %0.0f\nDP : %0.0f\nASP : %0.1f\nMSP : %0.0f\nCP : %0.1f"), gunPlayer->statment.attackPoint, gunPlayer->statment.defancePoint, gunPlayer->statment.attackSpeedPoint, gunPlayer->statment.moveSpeedPoint, gunPlayer->statment.coolTimePoint)));
        }
    }

    // 보스 찾기
    TArray<AActor*> ActorsWithTag;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("boss"), ActorsWithTag);
    for (AActor* Actor : ActorsWithTag)
    {
        if (Actor)
        {
            monster = Cast<ATest_Boss>(Actor);
        }
    }

    // 보스 체력바 표기
    if (monster != nullptr)
    {
        monster->currbossHP = monster->maxbossHP;
        text_MonsterHP->SetText(FText::FromString(FString::Printf(TEXT("%0.0f / %0.0f"), monster->currbossHP, monster->maxbossHP)));
    }

    // 게임 스테이트 받아오기
    gamestate = ABossRoomGameStateBase::Get();

    //UButton
    //보스존으로 같이 이동하는 버튼
    Btn_restart->OnClicked.AddDynamic(this, &UPlayerWidget::OnClickRestarGame);

    // inventory
    inventory.Add(slot_1);
    inventory.Add(slot_2);
    inventory.Add(slot_3);
    inventory.Add(slot_4);
    inventory.Add(slot_5);
    inventory.Add(slot_6);
    inventory.Add(slot_7);
    inventory.Add(slot_8);

}

void UPlayerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // 몬스터 있으면 체력 표기
    if (monster) {
        text_MonsterHP->SetText(FText::FromString(FString::Printf(TEXT("%0.0f / %0.0f"), monster->currbossHP, monster->maxbossHP)));
        MonsterHP->SetPercent(monster->currbossHP / monster->maxbossHP);
    }

    // 건 플레이어 전용
    if (gunPlayer) {
        HPBarText->SetText(FText::FromString(FString::Printf(TEXT("%0.0f / %0.0f"), gunPlayer->statment.healthPoint, gunPlayer->statment.fullHP)));
        MPBarText->SetText(FText::FromString(FString::Printf(TEXT("%0.0f / %0.0f"), gunPlayer->statment.manaPoint, gunPlayer->statment.fullMP)));
        HealthBar->SetPercent(gunPlayer->statment.healthPoint / gunPlayer->statment.fullHP);
        ManaBar->SetPercent(gunPlayer->statment.manaPoint / gunPlayer->statment.fullMP);
        
        // 쿨타임 표기
        Q_Btn->SetPercent(gunPlayer->curr_Q / gunPlayer->cool_Q);
        E_Btn->SetPercent(gunPlayer->curr_E / gunPlayer->cool_E);
        F_Btn->SetPercent(gunPlayer->curr_F / gunPlayer->cool_F);
        R_Btn->SetPercent(gunPlayer->curr_R / gunPlayer->cool_R);
        SP_Btn->SetPercent(gunPlayer->curr_SP / gunPlayer->cool_SP);

        // 스킬 설명
        if (Q_Btn->IsHovered()) {
            SkillNameText->SetText(FText::FromString(FString::Printf(TEXT("내려찍기"))));
            statText->SetText(FText::FromString(FString::Printf(TEXT("\n\n적을 날림\n마나소모량 20"))));
        }
        else if (E_Btn->IsHovered()) {
            SkillNameText->SetText(FText::FromString(FString::Printf(TEXT("레이저"))));
            statText->SetText(FText::FromString(FString::Printf(TEXT("\n\n2초 둔화\n2*%0.0f의 피해\n마나소모량 20"), gunPlayer->statment.attackPoint)));
        }
        else if (F_Btn->IsHovered()) {
            SkillNameText->SetText(FText::FromString(FString::Printf(TEXT("폭탄"))));
            statText->SetText(FText::FromString(FString::Printf(TEXT("\n\n3*%0.0f의 피해\n마나소모량 30"), gunPlayer->statment.attackPoint)));
        }
        else if (R_Btn->IsHovered()) {
            SkillNameText->SetText(FText::FromString(FString::Printf(TEXT("핵폭틴"))));
            statText->SetText(FText::FromString(FString::Printf(TEXT("\n\n큰피해 후 지속피해\n마나 소모량 50"), gunPlayer->statment.attackPoint, gunPlayer->statment.attackPoint)));
        }
        else if (SP_Btn->IsHovered()) {
            SkillNameText->SetText(FText::FromString(FString::Printf(TEXT("대쉬"))));
            statText->SetText(FText::FromString(FString::Printf(TEXT("\n\n회피기\n일정 거리 날아감\n마나 소모량 없음"))));
        }
        else if (Passive_Btn->IsHovered()) {
            SkillNameText->SetText(FText::FromString(FString::Printf(TEXT("과부하"))));
            statText->SetText(FText::FromString(FString::Printf(TEXT("\n\n패시브\n평타 6타 이속증가\n평타 150타 스턴"))));
        }
        else {
            SkillNameText->SetText(FText::FromString(FString::Printf(TEXT(""))));
            statText->SetText(FText::FromString(FString::Printf(TEXT("ATK    :   %0.0f\nDFN    :   %0.0f\nRPM   :   %0.0f\nSPD    :   %0.0f\nCOL    :   %0.0f\n"), gunPlayer->statment.attackPoint, gunPlayer->statment.defancePoint, gunPlayer->statment.attackSpeedPoint, gunPlayer->statment.moveSpeedPoint, gunPlayer->statment.coolTimePoint)));
        }
    }

    // 맞았을 때 피격 화면 출력
    if (player->bIsAttacked == true)
    {
        BloodImg->SetOpacity(0.3f);
        bloodOpacity = 0.4f;
        player->bIsAttacked = false;
    }
    else if (bloodOpacity > 0)
    {
        bloodOpacity -= InDeltaTime;
        BloodImg->SetOpacity(bloodOpacity);
    }
}

void UPlayerWidget::ChangeInventory()
{
    // 인벤토리 칸에 아이템 출력
    int32 i = 0;
    int32 num = player->itemIndexArray.Num() - 1;
    if (num >= 0) {
        for (int32 index : player->itemIndexArray)
        {
            // 사용한 아이템은 삭제
            if (index == 0)
            {
                player->itemIndexArray.Remove(index);
                continue;
            }
            FItem* item = gamestate->itemDataTable->FindRow<FItem>(*FString::FromInt(index), TEXT(""));
            //UImage 인클루드
            inventory[i]->SetBrushFromTexture(item->image);
            i++;
        }
    }

    if (i <= 7)
    {
        while(i <= 7)
        {
            FItem* item = gamestate->itemDataTable->FindRow<FItem>(*FString::FromInt(0), TEXT(""));
            inventory[i]->SetBrushFromTexture(item->image);
            i++;
        }
    }
}

void UPlayerWidget::OnClickRestarGame()
{
    //UWidget* restartBossZone = GetWidgetFromName(FName("restart"));
    AActor* findGate = UGameplayStatics::GetActorOfClass(GetWorld(), AGateBattleBoss::StaticClass());
    gateBossZone = Cast<AGateBattleBoss>(findGate);
    gateBossZone->viewRestartWidget();
    //URestartBossZone
    //restartBossZone->SetVisibility(ESlateVisibility::Visible);

    /*
    AGameModeBase* findgm = UGameplayStatics::GetGameMode(GetWorld());
    bhGameMode = Cast<ABossHunterGameMode>(findgm);
    if (findgm == nullptr)
    {
    return;
    }
    bhGameMode->URLTravel();
    */
}
