// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()
	
public:
	ATank();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 특정 폰 파괴 함수
	void HandleDestruction();

	APlayerController* GetTankPlayerController() const { return TankPlayerController; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// private 멤버를 블루프린트에서 사용시 AllowPrivateAccess 설정 필요
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComp;

	// 인스턴스와 클래스 에디터에서 보이고 수정 불가 / 블루프린트에서 엑세스 (읽기, 쓰기) 가능
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 450.0f;

	// 인스턴스와 클래스 에디터에서 보이고 수정 불가 / 블루프린트에서 엑세스 (읽기, 쓰기) 가능
	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 100.0f;

	void Move(float Value);
	void Turn(float Value);

	APlayerController* TankPlayerController;
};
