// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::BeginPlay()
{
    Super::BeginPlay();

    // ATank가 APawn의 하위 타입이기 때문에 Cast 사용
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    // 타이머 설정 로직
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 사정거리 내에 있다면 터렛이 탱크를 향해 회전
    if (TargetInSight())
    {
		RotateTurret(Tank->GetActorLocation());
    }
}

void ATower::HandleDestruction()
{
    // BasePawn HandleDestruction 함수 사용 (시각/음향 효과)
    Super::HandleDestruction();

    // 타워 파괴
    Destroy();
}

// 발사 타이머 체크 함수
void ATower::CheckFireCondition()
{
    if (Tank == nullptr)
    {
        return;
    }

    // 탱크가 파괴되지 않고, 사정거리 내에 있을 때 터렛이 탱크를 향해 발사
    if (TargetInSight() && Tank->bAlive)
    {
	    Fire();
    }
}

// 타겟이 사정거리 내에 있는지 확인하는 함수
bool ATower::TargetInSight()
{   
    // 탱크 포인터 null 체크
    if (Tank)
    {
        // 탱크와의 거리 계산
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

        // 탱크가 사정거리에 있는지 확인
        if (Distance <= FireRange)
        {
            return true;
        }
    }
    return false;
}