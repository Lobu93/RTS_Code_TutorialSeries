// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_PreviewBuilding.h"

// Sets default values
ARTS_PreviewBuilding::ARTS_PreviewBuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PreviewBuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewBuildingMesh"));
	PreviewBuildingMesh->SetupAttachment(RootComponent);
	PreviewBuildingMesh->bEditableWhenInherited = true;
}

// Called when the game starts or when spawned
void ARTS_PreviewBuilding::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARTS_PreviewBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARTS_PreviewBuilding::DestroyPreview()
{

}

