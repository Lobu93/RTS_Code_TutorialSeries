// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTS_Cpp_TutorialCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Library/EnumList.h"
#include "Player/RTS_PlayerController.h"
#include "GameSettings/RTS_GameState.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"

ARTS_Cpp_TutorialCharacter::ARTS_Cpp_TutorialCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	//GetCharacterMovement()->MaxAcceleration = 100000.0f;

	// Create a decal in the world to show the cursor's location
	SelectedDecal = CreateDefaultSubobject<UDecalComponent>("SelectedDecal");
	SelectedDecal->SetupAttachment(RootComponent);
	SelectedDecal->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	SelectedDecal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	SelectedDecal->SetRelativeLocation(FVector(-10.0f, 0.0f, -90.0f));
	SelectedDecal->SetVisibility(false);
	SelectedDecal->bHiddenInGame = true;

	// Create a decal in the world to show the unselected characters
	DeselectedDecal = CreateDefaultSubobject<UDecalComponent>("DeselectedDecal");
	DeselectedDecal->SetupAttachment(RootComponent);
	DeselectedDecal->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	DeselectedDecal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	DeselectedDecal->SetRelativeLocation(FVector(-10.0f, 0.0f, -90.0f));

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

}

// Called when the game starts or when spawned
void ARTS_Cpp_TutorialCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->bAutoActivate = true;

	if (bIsSpawnedIn)
	{
		SetUnitFeatures();
	}

	ReferenceCast();

	ControllerRef->DisplayUnitHUD_Delegate.AddDynamic(this, &ARTS_Cpp_TutorialCharacter::DisplayUnitHUD);

	SetUnitBirthday();

	UnitAIController = Cast<AAIController>(GetController());

	if (UnitAIController)
	{
		UnitAIController->ReceiveMoveCompleted.Add(ControllerRef->MovementCompleteDelegate);
	}
}

void ARTS_Cpp_TutorialCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	/*if (SelectedDecal != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				SelectedDecal->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			SelectedDecal->SetWorldLocation(TraceHitResult.Location);
			SelectedDecal->SetWorldRotation(CursorR);
		}
	}*/
}

void ARTS_Cpp_TutorialCharacter::ReferenceCast_Implementation()
{
	GameStateRef = (ARTS_GameState*)GetWorld()->GetGameState();
	if (!GameStateRef)
	{
		UE_LOG(LogTemp, Error, TEXT("ARTS_Cpp_TutorialCharacter::ReferenceCast() Bad GameState Class"));
	}

	ControllerRef = (ARTS_PlayerController*)GetWorld()->GetFirstPlayerController();
	if (!ControllerRef)
	{
		UE_LOG(LogTemp, Error, TEXT("ARTS_Cpp_TutorialCharacter::ReferenceCast() Bad PlayerController Class"));
	}
}

EFemaleNames ARTS_Cpp_TutorialCharacter::SetFemaleName()
{
	const UEnum* FemaleNamesEnum = StaticEnum<EFemaleNames>();
	int32 NumOfEntriesLocal;
	BYTE RandomNameLocal;

	NumOfEntriesLocal = FemaleNamesEnum->NumEnums() - 1;
	RandomNameLocal = BYTE(FMath::RandRange(0, NumOfEntriesLocal));

	return EFemaleNames(RandomNameLocal);
}

EMaleNames ARTS_Cpp_TutorialCharacter::SetMaleName()
{
	const UEnum* MaleNamesEnum = StaticEnum<EMaleNames>();
	int32 NumOfEntriesLocal;
	BYTE RandomNameLocal;

	NumOfEntriesLocal = MaleNamesEnum->NumEnums() - 1;
	RandomNameLocal = BYTE(FMath::RandRange(0, NumOfEntriesLocal));

	/*FString MaleName = MaleNamesEnum->GetNameStringByIndex(int32(RandomNameLocal));
	UE_LOG(LogTemp, Warning, TEXT("ARTS_Cpp_TutorialCharacter::SetMaleName() | MaleName: %s"), *MaleName);*/

	return EMaleNames(RandomNameLocal);
}

ESurnames ARTS_Cpp_TutorialCharacter::SetSurname()
{
	const UEnum* SurnamesEnum = StaticEnum<ESurnames>();
	int32 NumOfEntriesLocal;
	BYTE RandomNameLocal;

	NumOfEntriesLocal = SurnamesEnum->NumEnums() - 1;
	RandomNameLocal = BYTE(FMath::RandRange(0, NumOfEntriesLocal));

	/*FString Surname = SurnamesEnum->GetNameStringByIndex(int32(RandomNameLocal));
	UE_LOG(LogTemp, Warning, TEXT("ARTS_Cpp_TutorialCharacter::SetSurname() | Surname: %s"), *Surname);*/

	return ESurnames(RandomNameLocal);
}

FString ARTS_Cpp_TutorialCharacter::GetRandomName(int32 Sex)
{
	FString UnitNameLocal;
	const UEnum* MaleNamesEnum = StaticEnum<EMaleNames>();
	const UEnum* FemaleNamesEnum = StaticEnum<EFemaleNames>();
	const UEnum* SurnamesEnum = StaticEnum<ESurnames>();

	if (Sex == 0)
	{
		MaleName = SetMaleName();
		Surname = SetSurname();

		UnitNameLocal = MaleNamesEnum->GetNameStringByIndex(int32(MaleName));
		UnitNameLocal.Append(TEXT(" "));
		UnitNameLocal.Append(SurnamesEnum->GetNameStringByIndex(int32(Surname)));

		// UE_LOG(LogTemp, Warning, TEXT("ARTS_Cpp_TutorialCharacter::GetRandomName() | UnitMaleName: %s"), *UnitNameLocal);
	}
	else
	{
		FemaleName = SetFemaleName();
		Surname = SetSurname();

		UnitNameLocal = FemaleNamesEnum->GetNameStringByIndex(int32(FemaleName));
		UnitNameLocal.Append(TEXT(" "));
		UnitNameLocal.Append(SurnamesEnum->GetNameStringByIndex(int32(Surname)));

		// UE_LOG(LogTemp, Warning, TEXT("ARTS_Cpp_TutorialCharacter::GetRandomName() | UnitFemaleName: %s"), *UnitNameLocal);
	}

	return UnitNameLocal;
}

void ARTS_Cpp_TutorialCharacter::SetUnitFeatures()
{
	// 0 = Male
	UnitSex = FMath::RandRange(0, 1);

	DynMaterial = UMaterialInstanceDynamic::Create(MaterialToEdit, this);
	GetMesh()->SetMaterial(0, DynMaterial);

	if (UnitSex == 0)
	{	
		NewColor = FLinearColor::Blue;
		UnitName = GetRandomName(UnitSex);
		UnitAge = FMath::RandRange(18, 70);
		UnitImage = MaleImage;
		UnitProfile.Name = UnitName;
		UnitProfile.Sex = UnitSex;
		UnitProfile.Age = UnitAge;
		UnitProfile.UnitImage = UnitImage;

		/*UE_LOG(LogTemp, Warning, TEXT("ARTS_Cpp_TutorialCharacter::SetUnitFeatures() | Name: %s Sex: %d Age: %d"), 
			*UnitProfile.Name, UnitProfile.Sex, UnitProfile.Age);*/
	}
	else
	{
		NewColor = FLinearColor(FColor::Purple);
		UnitName = GetRandomName(UnitSex);
		UnitAge = FMath::RandRange(18, 70);
		UnitImage = FemaleImage;
		UnitProfile.Name = UnitName;
		UnitProfile.Sex = UnitSex;
		UnitProfile.Age = UnitAge;
		UnitProfile.UnitImage = UnitImage;

		/*UE_LOG(LogTemp, Warning, TEXT("ARTS_Cpp_TutorialCharacter::SetUnitFeatures() | Name: %s Sex: %d Age: %d"),
			*UnitProfile.Name, UnitProfile.Sex, UnitProfile.Age);*/
	}

	if (DynMaterial)
	{
		DynMaterial->SetVectorParameterValue("BodyColor", NewColor);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ARTS_Cpp_TutorialCharacter::SetUnitFeatures() | Error: DynMaterial is null..."));
	}
}

void ARTS_Cpp_TutorialCharacter::SetUnitBirthday()
{
	if (!bIsSpawnedIn)
	{
		BirthYear = GameStateRef->Year - UnitAge;
		Birthday = GameStateRef->Day;
		BirthMonth = GameStateRef->Month;
	}
	else
	{
		BirthYear = GameStateRef->Year - UnitAge;
		BirthMonth = FMath::RandRange(1, 12);
		Birthday = FMath::Clamp(FMath::RandRange(1, 31), 0, UKismetMathLibrary::DaysInMonth(BirthYear, BirthMonth));
	}
}

void ARTS_Cpp_TutorialCharacter::BirthdayCheck()
{
	if ((GameStateRef->Month == BirthMonth) && (GameStateRef->Day == Birthday))
	{
		UnitAge++;
	}
}

void ARTS_Cpp_TutorialCharacter::DisplayUnitHUD_Implementation(AActor* Actor, bool bBypass)
{
	if ((Actor == this) && ((bBypass == true) || (!bIsHidden)))
	{
		bCanDisplayUnitHUD = true;
	}
	else
	{
		bCanDisplayUnitHUD = false;
	}
}

void ARTS_Cpp_TutorialCharacter::SetSelectedDecal()
{
	DeselectedDecal->SetVisibility(false);
	DeselectedDecal->SetHiddenInGame(true);
	SelectedDecal->SetVisibility(true);
	SelectedDecal->SetHiddenInGame(false);
}

void ARTS_Cpp_TutorialCharacter::SetDeselectedDecal()
{
	SelectedDecal->SetVisibility(false);
	SelectedDecal->SetHiddenInGame(true);
	DeselectedDecal->SetVisibility(true);
	DeselectedDecal->SetHiddenInGame(false);
}

void ARTS_Cpp_TutorialCharacter::ExitVehicle()
{
	FVector GoalLocal;
	float DestLocationX = FMath::RandRange(100.0f, 400.0f);
	float DestLocationY = FMath::RandRange(100.0f, 400.0f);
	FVector DestLocation = FVector(DestLocationX, DestLocationY, 100.0f);

	GoalLocal = GetActorLocation() + DestLocation;

	UnitAIController->MoveToLocation(GoalLocal);
}
