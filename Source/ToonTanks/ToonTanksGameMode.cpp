// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
    // 탱크 파괴 시
    if (DeadActor == Tank)
    {
        // 탱크 파괴 함수 호출
        Tank->HandleDestruction();

        // 플레이어 컨트롤러 null 체크
        if (ToonTanksPlayerController)
        {
            // // 탱크 입력 비활성화 함수 호출
            // Tank->DisableInput(Tank->GetTankPlayerController());
            // // 마우스 커서 비활성화
            // Tank->GetTankPlayerController()->bShowMouseCursor = false;

            // 탱크 입력 비활성화, 마우스 커서 비활성화
            ToonTanksPlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);
    }
    // 타워 파괴 시
    else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
    {
        // 타워 파괴 함수 호출
        DestroyedTower->HandleDestruction();

        // 타워 개수 감소
        --TargetTowers;

        // 타워 개수 0 이하일 경우 게임오버
        if (TargetTowers <= 0)
        {
            GameOver(true);
        }
    }
}

void AToonTanksGameMode::BeginPlay()
{
    Super::BeginPlay();
    // 게임 시작
    HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
    // 타워 개수 할당 및 초기화
    TargetTowers = GetTargetTowerCount();

    // GetPlayerPawn 은 ATank의 상위인 APawn을 반환하므로 캐스팅
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    // GetPlayerController 는 AToonTanksPlayerController의 상위인 APlayerController를 반환하므로 캐스팅
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    // 게임 시작 로직. C++에서 호출, BlueprintImplementableEvent를 사용해 로직은 블루프린트에서 구현
    StartGame();
    
    UE_LOG(LogTemp, Warning, TEXT("TargetTowers : %i"), TargetTowers);

    // 플레이어 컨트롤러 nullptr 체크
    if (ToonTanksPlayerController)
    {
        // 처음 입력 비활성화
        ToonTanksPlayerController->SetPlayerEnabledState(false);

        // 타이머 핸들 로컬 변수
        FTimerHandle PlayerEnableTimerHandle;

        // 지연 시간이 끝나고 콜백이 호출될 때 콜백 함수에 넘어가는 타이머 델리게이트
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
            ToonTanksPlayerController, 
            &AToonTanksPlayerController::SetPlayerEnabledState, 
            true
        );

        // 타이머 델리게이트를 사용한 SetTimer 호출
        GetWorldTimerManager().SetTimer(
            PlayerEnableTimerHandle, 
            PlayerEnableTimerDelegate,
            StartDelay,
            false
        );
    }
}

// 타워 개수 세서 반환하는 함수
int32 AToonTanksGameMode::GetTargetTowerCount()
{
    // 타워 배열 변수
    TArray<AActor*> Towers;

    // 월드 내 타워 클래스 가져오기
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    
    // 타워 개수 반환
    return Towers.Num();
}