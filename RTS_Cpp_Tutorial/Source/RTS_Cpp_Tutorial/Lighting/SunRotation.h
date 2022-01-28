// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DirectionalLight.h"
#include "SunRotation.generated.h"

class ARTS_GameState;

UCLASS()
class RTS_CPP_TUTORIAL_API ASunRotation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASunRotation();

private:
	ARTS_GameState* GameStateRef;

	float GameSpeed;

	float GameTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GetGameSpeed(float InGameSpeed);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	ADirectionalLight* SunRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	AActor* SkySphereRef;

	// ADirectionalLight* SunRef;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	TSubclassOf<ADirectionalLight> SunClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	TSubclassOf<AActor> SkySphereRef;*/

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	UClass* SunClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	UClass* SkySphereRef;*/

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "References")
	//ARTS_GameState* GameStateRef;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	//float GameSpeed;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	//float GameTime;
};
