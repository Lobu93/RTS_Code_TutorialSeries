// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_SpawnVolume.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Landscape.h"
#include "UObject/UObjectBaseUtility.h"
#include "Components/BoxComponent.h"
#include "RTS_ResourceMaster.h"

// Sets default values
ARTS_SpawnVolume::ARTS_SpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnAreaMarker = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnAreaMarker"));
	SetRootComponent(SpawnAreaMarker);

	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SpawnVolume->SetupAttachment(RootComponent);
	SpawnVolume->bEditableWhenInherited = true;
	SpawnVolume->SetWorldScale3D(FVector(25.0f, 25.0f, 5.5f));

}

// Called when the game starts or when spawned
void ARTS_SpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandleLocal;
	
	if (bIsSpawnedOnBeginPlay)
	{
		SpawnResource(ResourceRef, NumOfResourcesSpawn);

		TotalNumberSpawned += NumOfResourcesSpawn;

		if ((bIsSpawnedOnTimer) && (TotalNumberSpawned < MaxResourcesSpawned))
		{
			GetWorldTimerManager().SetTimer(TimerHandleLocal, this, &ARTS_SpawnVolume::SpawnResourcesOnTime, RespawnTime, true);
		}
	}
}

// Called every frame
void ARTS_SpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARTS_SpawnVolume::SpawnResource(TSubclassOf<ARTS_ResourceMaster> Resource, int32 SpawnAmount)
{
	TSubclassOf<ARTS_ResourceMaster> ResourceRefLocal;
	int32 SpawnAmountLocal;
	TArray<FHitResult> HitResult;
	FVector StartPositionLocal;
	FVector HalfSizeLocal;
	FVector OriginBoundsLocal;
	FVector BoxExtentLocal;
	FVector RandomBoxExtentLocal;
	float RandomExtent_X;
	float RandomExtent_Y;
	FCollisionShape CollisionBox;
	bool bIsHit;

	FActorSpawnParameters SpawnInfo;
	FRotator ResourceRotationLocal = FRotator(0, 0, 0);

	ResourceRefLocal = Resource;
	SpawnAmountLocal = SpawnAmount;

	GetActorBounds(false, OriginBoundsLocal, BoxExtentLocal);

	for (int i = 0; i < SpawnAmount; i++)
	{
		RandomExtent_X = FMath::RandRange((BoxExtentLocal.X - (BoxExtentLocal.X * 2.0f)), BoxExtentLocal.X);

		RandomExtent_Y = FMath::RandRange((BoxExtentLocal.Y - (BoxExtentLocal.Y * 2.0f)), BoxExtentLocal.Y);

		RandomBoxExtentLocal = FVector(RandomExtent_X, RandomExtent_Y, OriginBoundsLocal.Z);

		StartPositionLocal = (OriginBoundsLocal + RandomBoxExtentLocal) + FVector(0.0f, 0.0f, 200.0f);

		HalfSizeLocal = (OriginBoundsLocal + RandomBoxExtentLocal) - FVector(0.0f, 0.0f, 200.0f);

		CollisionBox = FCollisionShape::MakeBox(HalfSizeLocal);

		// DrawDebugBox(GetWorld(), StartPositionLocal, HalfSizeLocal, FColor::Purple, true);

		bIsHit = GetWorld()->SweepMultiByChannel(
			HitResult,
			StartPositionLocal,
			FVector(0.0f, 0.0f, 0.0f),
			FQuat::Identity,
			ECC_WorldStatic,
			CollisionBox
			);

		if (bIsHit)
		{
			for (auto& Hit : HitResult)
			{				
				if (Hit.GetActor()->IsA(ALandscape::StaticClass()))
				{
					GetWorld()->SpawnActor<ARTS_ResourceMaster>(ResourceRefLocal, Hit.Location, ResourceRotationLocal, SpawnInfo);

					//UE_LOG(LogTemp, Warning, TEXT("ARTS_SpawnVolume::SpawnResource() | ResourceRef: %s Hit.Location: %s"), *ResourceRefLocal->GetName(), *Hit.Location.ToString());

					break;
				}
				/* else
				{
					UE_LOG(LogTemp, Error, TEXT("ARTS_SpawnVolume::SpawnResource() | Hit Result: %s"), *Hit.Actor->GetClass()->GetName());

					// UE_LOG(LogTemp, Error, TEXT("ARTS_SpawnVolume::SpawnResource() | LandscapeLocal: %s"), *LandscapeLocal->GetName());
				} */

				// UE_LOG(LogTemp, Warning, TEXT("ARTS_SpawnVolume::SpawnResource() | Index: %d"), i);

				// UE_LOG(LogTemp, Warning, TEXT("ARTS_SpawnVolume::SpawnResource() | Hit Result: %s"), *Hit.Actor->GetName());
			}
		}
	}
}

void ARTS_SpawnVolume::SpawnResourcesOnTime()
{
	if ((bIsSpawnedOnTimer) && (TotalNumberSpawned < MaxResourcesSpawned))
	{
		SpawnResource(ResourceRef, NumOfResourcesSpawn);

		TotalNumberSpawned += NumOfResourcesSpawn;

		// UE_LOG(LogTemp, Warning, TEXT("ARTS_SpawnVolume::SpawnResourcesOnTime() | TotalNumberSpawned: %d"), TotalNumberSpawned);
	}
}

