// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

	// 특정 폰 파괴 함수
	void HandleDestruction();

	// 인스턴스에서만 보이고 수정 불가
	UPROPERTY(VisibleInstanceOnly)
	int32 VisibleInstanceOnlyInt = 11;

	// 클래스 에디터에서만 보이고 수정 불가
	UPROPERTY(VisibleDefaultsOnly)
	int32 VisibleDefaultsOnlyInt = 5;

	// 클래스 에디터에서만 보이고 수정 가능
	UPROPERTY(EditDefaultsOnly)
	int32 EditDefaultsOnlyInt = 9;

	// 인스턴스에서만 보이고 수정 가능
	UPROPERTY(EditInstanceOnly)
	int32 EditInstanceOnlyInt = 14;

protected:
	void RotateTurret(FVector LookAtTarget);
	void Fire();

private:
	//* 전방 선언 : 사용할 컴포넌트의 헤더 파일 전체가 필요하지 않을 때 포인터와 같이 사용
	//* 헤더 파일에 헤더 파일 선언 시 해당 해더 파일의 모든 콘텐츠를 가져와 크기가 커짐.
	//* 필요할 때만 헤더를 포함하는 것이 좋은 프로그래밍 사례. 허나 상속에는 상속하는 타입의 헤더 파일 필요
	//* .cpp 파일엔 사용할 것만 포함, .h파일에는 가능한 한 적은 내용을 포함.
	//? 전방 선언 사용 시 새 포인터를 선언하는 데 헤더 파일이 필요하지 않고
	//? 특정한 타입의 오브젝트를 생성, 함수 및 변수와 같은 멤버에 엑세스하는 데는 헤더 파일이 필요.
	//! 전방 선언을 사용하지 않을 경우 모든 콘텐츠 사본이 들어가 파일이 커지고 컴파일 시간이 증가.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;

	// StaticMeshComponent는 모든 Pawn에 존재
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;

	// USceneComponent도 모든 Pawn에 존재
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	// 인스턴스와 클래스 에디터에서 보이고 수정 불가 / 블루프린트에서 엑세스 (읽기, 쓰기) 가능
	//* Private 변수를 블루프린트에서 사용 시 meta 데이터 지정을 통한 프라이빗 액세스 부여로 사용 가능
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Super Duper Variables", meta = (AllowPrivateAccess = "true"))
	int32 VisibleAnywhereInt = 12;

	// 인스턴스와 클래스 에디터에서 보이고 수정 가능 / 블루프린트에서 엑세스 (읽기) 가능
	//* 블루프린트에서 정리할 수 있도록 변수에 자체적인 카테고리 부여 가능
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Super Duper Variables", meta = (AllowPrivateAccess = "true"))
	int32 EditAnywhereInt = 22;

	// TSubclassOf는 Projectile에 기반한 특정 타입으로 발사체 설정
	// UClass에는 C++ 블루프린트 사이 리플렉션을 가능하게 하는 함수가 내장 
	// (정보가 C++와 블루프린트 사이에 흐름)
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;

	// 죽었을 때 폭발 파티클
	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* DeathParticles;

	// 죽었을 때 효과음
	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase* DeathSound;

	// 폰 파괴 시 카메라 흔들림 효과
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> DeathCameraShakeClass;

};
