// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Library/EnumList.h"
#include "RTS_ResourceMaster.generated.h"

class UStaticMeshComponent;

UCLASS()
class RTS_CPP_TUTORIAL_API ARTS_ResourceMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARTS_ResourceMaster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class USceneComponent* Scene;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	//TSubclassOf<UStaticMeshComponent> ResourceMesh;
	class UStaticMeshComponent* ResourceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceLegend")
	FString ResourceName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ResourceLegend")
	class UObject* ResourceIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bool")
	bool bIsRandomLevel = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bool")
	bool bIsRandomSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bool")
	bool bIsRandomRotated = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	EResourceLevel ResourceLevelEnum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	float ResourceModifier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceValue")
	float CurrentResources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceValue")
	float BaseResourceAmount = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceSize")
	float MinMeshSize = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceSize")
	float MaxMeshSize = 1.1f;
};
