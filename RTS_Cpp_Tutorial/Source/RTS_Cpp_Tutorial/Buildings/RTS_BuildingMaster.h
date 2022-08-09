// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RTS_BuildingMaster.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class ARTS_PlayerController;

UCLASS(Blueprintable)
class RTS_CPP_TUTORIAL_API ARTS_BuildingMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARTS_BuildingMaster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UBillboardComponent* BillboardComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	ARTS_PlayerController* PlayerControllerRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* BuildingMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMesh* DisplayMesh;

};
