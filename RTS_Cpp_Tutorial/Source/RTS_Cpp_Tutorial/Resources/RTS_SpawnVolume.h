// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "RTS_ResourceMaster.h"
#include "RTS_SpawnVolume.generated.h"

class UBoxComponent;
class ARTS_ResourceMaster;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class USceneComponent* SpawnAreaMarker;

	class UBoxComponent* SpawnVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	TSubclassOf<ARTS_ResourceMaster> ResourceRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bool")
	bool bIsSpawnedOnBeginPlay = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bool")
	bool bIsSpawnedOnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceSettings")
	int32 NumOfResourcesSpawn = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceSettings")
	int32 TotalNumberSpawned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceSettings")
	int32 MaxResourcesSpawned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceSettings")
	float RespawnTime;

	void SpawnResource(TSubclassOf<ARTS_ResourceMaster> Resource, int32 SpawnAmount);

	void SpawnResourcesOnTime();
};
