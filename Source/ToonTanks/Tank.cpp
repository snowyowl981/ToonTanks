// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"


ATank::ATank()
{
    // SpringArmComp 생성 후 RootComponent에 연결
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(RootComponent);

    // BaseMesh 생성 후 SpringArmComp에 연결
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Move 축 매핑에 Move 함수 매핑
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);

	// Turn 축 매핑에 Turn 함수 매핑
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);

	// Fire 축 매핑에 Fire 함수 매핑
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	// BeginPlay 함수의 모든 기능과 상위 클래스의 기능 호출
	Super::BeginPlay();
	
	// GetController는 Pawn 클래스에서 상속, 생성자에서 할당하기엔 이르기에 BeginPlay사용
	//* Cast : PlayerControllerRef는 APlayerController타입, GetController()는 그 상위인 AController 타입.
	//* GetController에서 반환되는 AController 값을 APlayerController로 캐스팅하는 과정.
	//? (APlayerController에 필요한 함수가 존재하기 때문)
	TankPlayerController = Cast<APlayerController>(GetController());

	
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// null 체크
	if (TankPlayerController)
	{
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(
			ECollisionChannel::ECC_Visibility, 
			false, 
			HitResult
		);
		
		// 포탑 회전 로직
		RotateTurret(HitResult.ImpactPoint);
	}
}

// 특정 폰 파괴 함수
void ATank::HandleDestruction()
{
	// BasePawn HandleDestruction 함수 사용 (시각/음향 효과)
    Super::HandleDestruction();

    // 탱크 숨기기
    SetActorHiddenInGame(true);

	// 틱 함수 비활성화
	SetActorTickEnabled(false);

}

// 탱크 전, 후진 함수
void ATank::Move(float Value)
{
	// Vector 초기화
	FVector DeltaLocation = FVector::ZeroVector;
	// 프레임 속도에 영향을 받지 않고 탱크를 부드럽고 일정하게 움직이기 위해 델타타임 연산 추가.
	DeltaLocation.X = Value * MoveSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
	// 움직일 때 다른 물체를 통과하지 못하도록 Sweep, 캡슐 콜라이더에서 BlockAllDynamics 설정
	AddActorLocalOffset(DeltaLocation, true);
}

// 탱크 좌, 우회전 함수
void ATank::Turn(float Value)
{
	// Rotator 초기화
	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
	// 움직일 때 다른 물체를 통과하지 못하도록 Sweep, 캡슐 콜라이더에서 BlockAllDynamics 설정
	AddActorLocalRotation(DeltaRotation, true);
}