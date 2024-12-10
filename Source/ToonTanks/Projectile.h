// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

// 같은 타입의 전방 선언의 경우 다른 줄에서 선언 시 바로 사용 가능
class USoundBase;

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// StaticMeshComponent는 모든 Pawn에 존재
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	// 멤버가 아니니 전방 선언
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	// 히트 이벤트를 위한 콜백 함수
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpluse, const FHitResult& Hit);

	// 대미지 변수
	UPROPERTY(EditAnywhere, Category = "Combat")
	float Damage = 50.0f;

	// 발사체 충돌 효과
	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* HitParticles;

	// 발사체 연기 궤적
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class UParticleSystemComponent* ProjectileTrail;

	// 발사 효과음
	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* LaunchSound;
	
	// 발사체 충돌 효과음
	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* HitSound;

	// 발사체 충돌 시 카메라 흔들림 효과
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
