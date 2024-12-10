// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	// 특정 폰 파괴 함수
	void HandleDestruction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	class ATank* Tank;

	// 동일한 사정거리를 위해 BluePrint에서만 편집
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float FireRange = 1500.0f;

	FTimerHandle FireRateTimerHandle;
	// 동일한 사정거리를 위해 BluePrint에서만 편집
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float FireRate = 2.0f;
	void CheckFireCondition();

	bool TargetInSight();
};
