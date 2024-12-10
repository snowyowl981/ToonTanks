// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// ProjectileMesh 생성 및 루트에 할당
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	// ParticleSystemComponent 생성 및 루트에 연결
	ProjectileTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	ProjectileTrail->SetupAttachment(ProjectileMesh);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->MaxSpeed = 1300.0f;
	ProjectileMovementComponent->InitialSpeed = 1300.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	// 사운드 null 체크 및 발사 시 사운드 재생
	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}
	
	// 발사체가 다른 물체에 닿았을 때 OnComponentHit 델리게이트와 OnHit 콜백 함수 바인딩
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// HitComp : 부딫히는 컴포넌트 (Ex. 발사체 Mesh), 
// OtherActor : 충돌 당한 액터, 
// OtherComp : 충돌 당한 컴포넌트 (Ex. StaticMesh), 
// NormalImpluse : 충돌 시 적용할 임펄스 벡터, 
// Hit : OnHit 호출 시 FHitResult에 접근해 충돌에 대한 정보를 더 얻을 수 있도록 저장
void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpluse, const FHitResult& Hit)
{
	// 소유자 호출, auto == AActor*
	auto MyOwner = GetOwner();

	// 소유자가 nullptr 체크 및 nullptr 인 경우 그대로 종료
	if (MyOwner == nullptr)
	{
		// null 일 때 발사체가 삭제되지 않는 문제 방지
		Destroy();
		return;
	}

	// 대미지를 가한 액터의 컨트롤러 호출
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();

	// UDamageType의 C++ 클래스
	auto DamageTypeClass = UDamageType::StaticClass();

	// 발사체 nullptr 체크, 발사체가 자기 자신 및 소유자에게 대미지를 주는 것 예외처리
	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		// 대미지 적용 로직
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);

		// 파티클 생성 로직
		if (HitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
		}

		// 사운드 null 체크 및 발사 시 사운드 재생
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		}

		if (HitCameraShakeClass)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		}
	}
	
	// 발사체가 삭제되지 않는 문제 방지
	Destroy();
}

