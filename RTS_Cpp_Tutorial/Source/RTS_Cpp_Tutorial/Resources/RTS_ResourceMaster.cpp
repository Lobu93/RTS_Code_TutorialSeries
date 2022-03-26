// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_ResourceMaster.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ARTS_ResourceMaster::ARTS_ResourceMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	ResourceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ResourceMesh"));
	ResourceMesh->SetupAttachment(RootComponent);
	ResourceMesh->bEditableWhenInherited = true;

	if (bIsRandomLevel)
	{
		BYTE TempByteLocal;

		TempByteLocal = (BYTE)FMath::RandRange(1, 6);

		ResourceLevelEnum = (EResourceLevel)TempByteLocal;
	}

	switch (ResourceLevelEnum)
	{
	case EResourceLevel::Resource_0:
		break;
	case EResourceLevel::Resource_1:
		ResourceModifier = 1.0f;
		break;
	case EResourceLevel::Resource_2:
		ResourceModifier = 1.2f;
		break;
	case EResourceLevel::Resource_3:
		ResourceModifier = 1.4f;
		break;
	case EResourceLevel::Resource_4:
		ResourceModifier = 1.6f;
		break;
	case EResourceLevel::Resource_5:
		ResourceModifier = 1.8f;
		break;
	case EResourceLevel::Resource_6:
		ResourceModifier = 2.0f;
		break;
	default:
		break;
	}

	CurrentResources = BaseResourceAmount * CurrentResources;

	if (ResourceMesh != nullptr)
	{
		if (bIsRandomSize)
		{
			float NewScaleLocal;

			NewScaleLocal = FMath::RandRange(MinMeshSize, MaxMeshSize);

			ResourceMesh->SetWorldScale3D(FVector(NewScaleLocal, NewScaleLocal, NewScaleLocal));
		}

		if (bIsRandomRotated)
		{
			ResourceMesh->SetWorldRotation(FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f));
		}
	}
}

// Called when the game starts or when spawned
void ARTS_ResourceMaster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARTS_ResourceMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

