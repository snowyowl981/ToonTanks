// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksPlayerController.h"
#include "GameFramework/Pawn.h"

void AToonTanksPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{   
    // 플레이어 활성화 시
    if (bPlayerEnabled)
    {
        // 입력 활성화
        GetPawn()->EnableInput(this);
    }
    // 플레이어 비활성화 시
    else
    {
        // 입력 비활성화
        GetPawn()->DisableInput(this);
    }
    bShowMouseCursor = bPlayerEnabled;
}