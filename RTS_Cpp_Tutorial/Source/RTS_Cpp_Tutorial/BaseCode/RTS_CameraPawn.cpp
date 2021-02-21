// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_CameraPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
ARTS_CameraPawn::ARTS_CameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->bEditableWhenInherited = true;
	Sphere->SetWorldScale3D(FVector(0.25f, 0.25f, 0.25f));
	Sphere->SetRelativeRotation(FQuat(FRotator(-70.0f, 0.0f, 0.0f)));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 1500.0f;
	SpringArm->SetupAttachment(Sphere);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	
}

// Called when the game starts or when spawned
void ARTS_CameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARTS_CameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARTS_CameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

