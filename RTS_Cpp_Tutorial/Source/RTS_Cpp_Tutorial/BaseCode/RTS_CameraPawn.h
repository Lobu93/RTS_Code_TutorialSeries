// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RTS_CameraPawn.generated.h"

// Houses components for camera
UCLASS()
class RTS_CPP_TUTORIAL_API ARTS_CameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARTS_CameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* Scene;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UStaticMeshComponent* Sphere;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UFloatingPawnMovement* FloatingPawnMovement;
};
