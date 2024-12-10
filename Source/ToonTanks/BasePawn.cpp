// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Projectile.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// CapsuleComponent 생성 및 루트에 연결
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Colider"));
	RootComponent = CapsuleComp;

	// BaseMesh 생성 후 CapsuleComponent에 연결
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	// TurretMesh 생성 후 BaseMesh에 연결
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	// ProjectileSpawnPoint 생성 후 TurretMesh에 연결
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

// 폰 사망 시 파티클 스폰
void ABasePawn::HandleDestruction()
{
	//TODO : 시각/음향 효과 구현
	// 파티클 null 체크
	if (DeathParticles)
	{
		// 파티클 스폰
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	}

	// 사운드 null 체크 및 사운드 재생
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	// 타겟 방향을 바라보는 벡터 계산 (타겟 위치 벡터 - 현재 위치 벡터)
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	// 이 중 Yaw값 (Z축 회전값) 만 이용해 회전값 계산
	FRotator LookAtRotation = FRotator(0.0f, ToTarget.Rotation().Yaw, 0.0f);
	// TurretMesh에 회전값 적용 및 보간 사용
	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
			TurretMesh->GetComponentRotation(), 
			LookAtRotation, 
			UGameplayStatics::GetWorldDeltaSeconds(this), 
			10.0f)
		);
}

void ABasePawn::Fire()
{
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	// Debug 구체 그리기
	// DrawDebugSphere(
	// 	GetWorld(), 
	// 	ProjectileSpawnPointLocation, 
	// 	10.0f, 
	// 	24, 
	// 	FColor::Red, 
	// 	false, 
	// 	3.0f
	// );

	// auto == var
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation);
	
	// 발사체를 생성한 폰으로 소유자 지정
	Projectile->SetOwner(this);
}