// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Library/EnumList.h"
#include "RTS_SpawnVolume.generated.h"

class UBoxComponent;
class ARTS_ResourceMaster;
class ARTS_PlayerController;
class ARTS_GameState;

UCLASS()
class RTS_CPP_TUTORIAL_API ARTS_SpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARTS_SpawnVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ReferenceCasts();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class USceneComponent* SpawnAreaMarker;

	class UBoxComponent* SpawnVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	TSubclassOf<ARTS_ResourceMaster> ResourceRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	ARTS_PlayerController* PlayerControllerRef;

	ARTS_GameState* GameStateRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	ERespawnTimer TimeSetting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bool")
	bool bIsSpawnedOnBeginPlay = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bool")
	bool bIsSpawnedOnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceSettings")
	int32 NumOfResourcesSpawn = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ResourceSettings")
	int32 TotalNumberSpawned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceSettings")
	int32 MaxResourcesSpawned;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ResourceSettings")
	float RespawnTime;

	float GameSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceSettings")
	float UserRespawnTime;

	void SpawnResource(TSubclassOf<ARTS_ResourceMaster> Resource, int32 SpawnAmount);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ResourceSettings")
	void SpawnResourcesOnTime();
	virtual void SpawnResourcesOnTime_Implementation();

	float SetRespawnTimer();

	UFUNCTION(BlueprintCallable)
	void GetGameSpeed(float InGameSpeed);
};