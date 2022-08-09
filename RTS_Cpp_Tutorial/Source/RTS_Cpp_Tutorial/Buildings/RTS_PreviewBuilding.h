// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Library/RTS_Construction_IF.h"
#include "../Library/Structures.h"
#include "RTS_PreviewBuilding.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class ARTS_PlayerController;

UCLASS()
class RTS_CPP_TUTORIAL_API ARTS_PreviewBuilding : public AActor, public IRTS_Construction_IF
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARTS_PreviewBuilding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	ARTS_PlayerController* PlayerControllerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* PreviewBuildingMesh;

	UPROPERTY(EditAnywhere)
	FGate Gate;

	class UStaticMesh* DisplayMesh;

	TSubclassOf<ARTS_BuildingMaster> BuildingClass;

	FTransform EmptyTransform = FTransform();
	FVector CursorWorldPosition;
	int32 SnapValue = 100;

	bool bIsPreviewing;

	void DestroyPreview();

	void SpawnPreview(TSubclassOf<ARTS_BuildingMaster> InBuildingClass, UStaticMesh* InDisplayMesh);

	// Interface Event
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Calendar")
	bool ConstructBuilding(TSubclassOf<ARTS_BuildingMaster> BuildingMasterClass);
	virtual bool ConstructBuilding_Implementation(TSubclassOf<ARTS_BuildingMaster> BuildingMasterClass);

	void GetCursorPosition(FVector CursorLocationIn);

	FVector GridSnaps();
};
