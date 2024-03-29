// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_PreviewBuilding.h"
#include "../Player/RTS_PlayerController.h"

// Sets default values
ARTS_PreviewBuilding::ARTS_PreviewBuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PreviewBuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewBuildingMesh"));
	PreviewBuildingMesh->SetupAttachment(RootComponent);
	PreviewBuildingMesh->bEditableWhenInherited = true;
	PreviewBuildingMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	/*Preview_Material_Instance = ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant>
		(TEXT("MaterialInstanceConstant'/Game/RTS_Tutorial/Materials/Construction/Preview_M_Inst.Preview_M_Inst'"));*/

	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant>
		Preview_M_Local(TEXT("MaterialInstanceConstant'/Game/RTS_Tutorial/Materials/Construction/Preview_M_Inst.Preview_M_Inst'"));

	Preview_Material_Instance = Preview_M_Local.Object;

	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant>
		PreviewBad_M_Local(TEXT("MaterialInstanceConstant'/Game/RTS_Tutorial/Materials/Construction/PreviewBad_M_Inst.PreviewBad_M_Inst'"));

	PreviewBad_Material_Instance = PreviewBad_M_Local.Object;
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

	if (Gate.IsOpen())
	{
		SetActorLocation(GridSnaps());

		if (CheckOverlap())
		{
			PreviewBuildingMesh->SetMaterial(0, PreviewBad_Material_Instance);
		}
		else
		{
			PreviewBuildingMesh->SetMaterial(0, Preview_Material_Instance);
		}
	}
}

void ARTS_PreviewBuilding::DestroyPreview()
{
	Gate.Close();

	BuildingClass = nullptr;
	DisplayMesh = nullptr;
	bIsPreviewing = false;

	PlayerControllerRef->SetBuildingModeActive(bIsPreviewing);

	PreviewBuildingMesh->SetStaticMesh(DisplayMesh);

	SetActorRotation(FRotator());
}

void ARTS_PreviewBuilding::SpawnPreview(TSubclassOf<ARTS_BuildingMaster> InBuildingClass, UStaticMesh* InDisplayMesh)
{
	if (bIsPreviewing)
	{
		DestroyPreview();
	}

	BuildingClass = InBuildingClass;
	DisplayMesh = InDisplayMesh;
	bIsPreviewing = true;

	PlayerControllerRef->SetBuildingModeActive(bIsPreviewing);

	PreviewBuildingMesh->SetStaticMesh(DisplayMesh);

	Gate.Open();
}

bool ARTS_PreviewBuilding::ConstructBuilding_Implementation(TSubclassOf<ARTS_BuildingMaster> BuildingMasterClass)
{
	ARTS_BuildingMaster* SpawnedClassLocal;
	// TSubclassOf<ARTS_BuildingMaster> BuildingClassLocal;


	UE_LOG(LogTemp, Warning, TEXT("ARTS_PreviewBuilding::ConstructBuilding() | EmptyTransform: %s"),
		*EmptyTransform.ToString());

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.bNoFail = true;
	SpawnedClassLocal = GetWorld()->SpawnActor<ARTS_BuildingMaster>(BuildingMasterClass, EmptyTransform, SpawnInfo);

	SpawnedClassLocal->PlayerControllerRef = PlayerControllerRef;

	// BuildingClassLocal = Cast<ARTS_BuildingMaster>(BuildingMasterClass);

	DestroyPreview();

	SpawnPreview(BuildingMasterClass, SpawnedClassLocal->DisplayMesh);

	SpawnedClassLocal->Destroy();

	return true;
}

void ARTS_PreviewBuilding::GetCursorPosition(FVector CursorLocationIn)
{
	CursorWorldPosition = CursorLocationIn;
}

FVector ARTS_PreviewBuilding::GridSnaps()
{
	FVector SnapPosiitonLocal;
	int32 Snap_X_Local;
	int32 Snap_Y_Local;

	Snap_X_Local = FMath::FloorToInt(CursorWorldPosition.X + 50.0f);
	Snap_X_Local /= SnapValue;
	Snap_X_Local *= SnapValue;

	Snap_Y_Local = FMath::FloorToInt(CursorWorldPosition.Y + 50.0f);
	Snap_Y_Local /= SnapValue;
	Snap_Y_Local *= SnapValue;

	SnapPosiitonLocal = FVector(Snap_X_Local, Snap_Y_Local, CursorWorldPosition.Z);

	return SnapPosiitonLocal;
}

bool ARTS_PreviewBuilding::CheckOverlap()
{
	//bool bIsOverlappingLocal;
	TArray<AActor*> OverlappingActorsLocal;

	GetOverlappingActors(OverlappingActorsLocal);

	return OverlappingActorsLocal.Num() >= 1;
}
