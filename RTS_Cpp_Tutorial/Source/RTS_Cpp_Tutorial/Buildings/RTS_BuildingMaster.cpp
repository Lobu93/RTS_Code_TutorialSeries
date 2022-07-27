// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_BuildingMaster.h"
#include "Components/BillboardComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

// Sets default values
ARTS_BuildingMaster::ARTS_BuildingMaster()
: Super() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BillboardComp = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComp"));
	BillboardComp->SetHiddenInGame(false, true);
	RootComponent = BillboardComp;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	BuildingMesh->SetupAttachment(RootComponent);
	BuildingMesh->bEditableWhenInherited = true;
	
	// DisplayMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("DisplayMesh"));
	// DisplayMesh->SetupAttachment(RootComponent);
	// DisplayMesh->EditableMesh() = true;

	/*static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(MeshObj);
	UStaticMesh* Asset = MeshAsset.Object;*/

	// DisplayMesh->set

	//bool bSetStaticMesh = BuildingMesh->SetStaticMesh(DisplayMesh);

	//UE_LOG(LogTemp, Warning, TEXT("ARTS_BuildingMaster::ARTS_BuildingMaster() | SetStaticMesh(DisplayMesh): %s"), 
	//	bSetStaticMesh ? TEXT("true") : TEXT("false"));

	//UE_LOG(LogTemp, Warning, TEXT("ARTS_BuildingMaster::ARTS_BuildingMaster() | SetStaticMesh(DisplayMesh): %s"),
	//	*DisplayMesh->GetName());
}

// Called when the game starts or when spawned
void ARTS_BuildingMaster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARTS_BuildingMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

