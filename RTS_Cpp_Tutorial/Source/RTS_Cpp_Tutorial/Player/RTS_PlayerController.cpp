// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_PlayerController.h"
#include "RTS_CameraPawn.h"
#include "../GameSettings/RTS_GameState.h"
#include "../Library/RTS_FuncLib.h"
#include "../RTS_Cpp_TutorialCharacter.h"
#include "../TP_Vehicle/TP_VehiclePawn.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "AIController.h"
#include "../HUD/RTS_MarqueeSelection.h"
#include "GameFramework/PlayerInput.h"
#include "Components/DecalComponent.h"
#include "../Buildings/RTS_PreviewBuilding.h"


// Called when the game starts or when spawned
void ARTS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	ReferenceCasts();

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.bNoFail = true;
	ConstructionPreviewRef = GetWorld()->SpawnActor<ARTS_PreviewBuilding>(PreviewBuild, EmptyTransform, SpawnInfo);

	if (ConstructionPreviewRef)
	{
		ConstructionPreviewRef->PlayerControllerRef = this;

		SpawnWidgets();

		/*UE_LOG(LogTemp, Warning, TEXT("ARTS_PlayerController::BeginPlay() | ConstructionPreviewRef Name: %s"),
			*ConstructionPreviewRef->GetName());*/
	}

	bShowMouseCursor = true;

	bEnableClickEvents = true;

	bEnableMouseOverEvents = true;

	DefaultMouseCursor = EMouseCursor::Hand;

	FInputModeGameAndUI InputModeLocal;
	InputModeLocal.SetHideCursorDuringCapture(false);
	SetInputMode(FInputModeGameAndUI(InputModeLocal));

	MovementCompleteDelegate.BindUFunction(this, "AIMoveCompleted");
}

void ARTS_PlayerController::SpawnWidgets_Implementation()
{
}

void ARTS_PlayerController::Tick(float InDeltaTime)
{
	Super::Tick(DeltaTime);

	DeltaTime = InDeltaTime;

	if (bIsHoldingInput)
	{
		UpdateSelection();
	}
	else
	{
		if (bIsBuildingModeActive && ConstructionPreviewRef != nullptr)
		{
			FVector CursorLocationLocal = URTS_FuncLib::SetCursorWorldPosition(this, 10000.0f);

			ConstructionPreviewRef->GetCursorPosition(CursorLocationLocal);
		}
	}
}

void ARTS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ARTS_PlayerController::Move_CameraPawn_X);
	InputComponent->BindAxis("MoveRight", this, &ARTS_PlayerController::Move_CameraPawn_Y);

	InputComponent->BindAction("ZoomReset", IE_Pressed, this, &ARTS_PlayerController::ZoomReset);
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ARTS_PlayerController::ZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ARTS_PlayerController::ZoomOut);

	InputComponent->BindAxis("Mouse_X", this, &ARTS_PlayerController::MousePan_X);
	InputComponent->BindAxis("Mouse_Y", this, &ARTS_PlayerController::MousePan_Y);

	InputComponent->BindAction("Pan", IE_Pressed, this, &ARTS_PlayerController::PanPressed);
	InputComponent->BindAction("Pan", IE_Released, this, &ARTS_PlayerController::PanReleased);
	InputComponent->BindAction("PanReset", IE_Pressed, this, &ARTS_PlayerController::PanReset);

	InputComponent->BindAction("SpeedModifier", IE_Pressed, this, &ARTS_PlayerController::SpeedModifierPressed);
	InputComponent->BindAction("SpeedModifier", IE_Released, this, &ARTS_PlayerController::SpeedModifierReleased);

	InputComponent->BindAction("SecondaryAction", IE_Pressed, this, &ARTS_PlayerController::SecondaryAction);

	InputComponent->BindAction("PrimaryAction", IE_Pressed, this, &ARTS_PlayerController::PrimaryAction_Pressed);
	InputComponent->BindAction("PrimaryAction", IE_Released, this, &ARTS_PlayerController::PrimaryAction_Released);

	FInputActionKeyMapping SpawnUnitMapping("SpawnUnitAction", EKeys::V, 0, 0, 0, 0);
	this->PlayerInput->AddActionMapping(SpawnUnitMapping);
	InputComponent->BindAction("SpawnUnitAction", IE_Pressed, this, &ARTS_PlayerController::SpawnUnitDebug);

	FInputActionKeyMapping SpawnVehicleMapping("SpawnVehicleAction", EKeys::B, 0, 0, 0, 0);
	this->PlayerInput->AddActionMapping(SpawnVehicleMapping);
	InputComponent->BindAction("SpawnVehicleAction", IE_Pressed, this, &ARTS_PlayerController::SpawnVehicleDebug);
}

void ARTS_PlayerController::Move_CameraPawn_X(float AxisValue)
{
	float X_DirectionLocal;
	FVector DirectionLocal;
	FTransform ActorTransformLocal;
	FVector LocationLocal;
	FVector ActorNewLocation;
	
	X_DirectionLocal = (AxisValue * DefaultMovementSpeed) * MovementSpeedModifier;

	DirectionLocal = FVector(X_DirectionLocal, 0.0f, 0.0f);

	ActorTransformLocal = CameraPawnRef->GetActorTransform();

	LocationLocal = UKismetMathLibrary::TransformDirection(ActorTransformLocal, DirectionLocal);

	LocationLocal += ActorTransformLocal.GetLocation();

	ActorTransformLocal = FTransform(ActorTransformLocal.GetRotation(), LocationLocal, ActorTransformLocal.GetScale3D());

	ActorNewLocation = FVector(ActorTransformLocal.GetLocation().X, ActorTransformLocal.GetLocation().Y, 110.0f);

	if (!bDisableCamMovement)
	{
		CameraPawnRef->SetActorLocation(ActorNewLocation, true);
	}
}

void ARTS_PlayerController::Move_CameraPawn_Y(float AxisValue)
{
	float Y_DirectionLocal;
	FVector DirectionLocal;
	FTransform ActorTransformLocal;
	FVector ActorNewLocation;

	Y_DirectionLocal = AxisValue * (MovementSpeedModifier * DefaultMovementSpeed);

	DirectionLocal = FVector(0.0f, Y_DirectionLocal , 0.0f);

	ActorTransformLocal = CameraPawnRef->GetActorTransform();

	ActorNewLocation = UKismetMathLibrary::TransformDirection(ActorTransformLocal, DirectionLocal);

	ActorNewLocation += ActorTransformLocal.GetLocation();

	if (!bDisableCamMovement)
	{
		CameraPawnRef->SetActorLocation(ActorNewLocation, true);
	}
}

void ARTS_PlayerController::ZoomReset()
{
	CameraPawnRef->SpringArm->TargetArmLength = 1500.0f;
}

void ARTS_PlayerController::ZoomIn()
{
	float TargetArmLengthLocal;

	TargetArmLengthLocal = CameraPawnRef->SpringArm->TargetArmLength - ZoomSpeed;

	CameraPawnRef->SpringArm->TargetArmLength = FMath::Clamp(TargetArmLengthLocal, MinZoomLimit, MaxZoomLimit);
}

void ARTS_PlayerController::ZoomOut()
{
	float TargetArmLengthLocal;

	TargetArmLengthLocal = CameraPawnRef->SpringArm->TargetArmLength + ZoomSpeed;

	CameraPawnRef->SpringArm->TargetArmLength = FMath::Clamp(TargetArmLengthLocal, MinZoomLimit, MaxZoomLimit);
}

void ARTS_PlayerController::PanPressed()
{
	bDisableCamMovement = true;
}

void ARTS_PlayerController::PanReleased()
{
	bDisableCamMovement = false;
}

void ARTS_PlayerController::PanReset()
{
	FRotator NewRotationLocal;
	FRotator ActorRotationLocal;

	ActorRotationLocal = CameraPawnRef->GetActorRotation();

	NewRotationLocal = FRotator(0.0f, ActorRotationLocal.Yaw, ActorRotationLocal.Roll);

    CameraPawnRef->SetActorRotation(NewRotationLocal);

}

void ARTS_PlayerController::MousePan_X(float AxisValue)
{
	FRotator NewRotationLocal;
	FRotator ActorRotationLocal;
	float YawLocal;

	if (bDisableCamMovement)
	{
		ActorRotationLocal = CameraPawnRef->GetActorRotation();

		YawLocal = ActorRotationLocal.Yaw + (AxisValue * PanSpeed);

		NewRotationLocal = FRotator(ActorRotationLocal.Pitch, YawLocal, ActorRotationLocal.Roll);

		CameraPawnRef->SetActorRotation(NewRotationLocal);
	}
	else
	{
		CameraPawnRef->AddActorLocalOffset(EdgeScroll(), true);
	}
}

void ARTS_PlayerController::MousePan_Y(float AxisValue)
{
	FRotator NewRotationLocal;
	FRotator ActorRotationLocal;
	float PitchLocal;

	if (bDisableCamMovement)
	{
		ActorRotationLocal = CameraPawnRef->GetActorRotation();

		PitchLocal = ActorRotationLocal.Pitch + (AxisValue * PanSpeed);

		PitchLocal = FMath::Clamp(PitchLocal, -25.0f, 45.0f);

		NewRotationLocal = FRotator(PitchLocal, ActorRotationLocal.Yaw, ActorRotationLocal.Roll);

		CameraPawnRef->SetActorRotation(NewRotationLocal);
	}
	else
	{
		CameraPawnRef->AddActorLocalOffset(EdgeScroll(), true);
	}
}

void ARTS_PlayerController::SpeedModifierPressed()
{
	MovementSpeedModifier = 2.0f;
}

void ARTS_PlayerController::SpeedModifierReleased()
{
	MovementSpeedModifier = 1.0f;
}

FVector ARTS_PlayerController::EdgeScroll()
{
	float Proportion_X_Local;
	float Proportion_Y_Local;
	float DeltaSpeed_X;
	float DeltaSpeed_Y;
	FVector2D MousePositionLocal;

	if (GetMousePosition(MousePositionLocal.X, MousePositionLocal.Y))
	{
		MouseLastValidPosition = MousePositionLocal;
	}
	else
	{
		MousePositionLocal = MouseLastValidPosition;
	}

	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

	Proportion_X_Local = MousePositionLocal.X / ViewportSize.X;

	Proportion_Y_Local = MousePositionLocal.Y / ViewportSize.Y;

	if (Proportion_X_Local >= 0.975f)
	{
		EdgeScrollSpeedX = 10.0f;
		
		/*FString MessageString = FString("Move Right. Because of Mouse X value is ");

		MessageString.AppendInt(MousePositionLocal.X);
		GEngine->AddOnScreenDebugMessage(-1, 30, FColor::White, MessageString);*/
	}
	else if (Proportion_X_Local <= 0.025f)
	{
		EdgeScrollSpeedX = -10.0f;

		/*FString MessageString = FString("Move Left. Because of Mouse X value is ");

		MessageString.AppendInt(MousePositionLocal.X);
		GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Black, MessageString);*/
	}
	else
	{
		EdgeScrollSpeedX = 0.0f;
	}
	
	if (Proportion_Y_Local >= 0.975f)
	{
		EdgeScrollSpeedY = -10.0f;
	}
	else if (Proportion_Y_Local <= 0.025f)
	{
		EdgeScrollSpeedY = 10.0f;
	}
	else
	{
		EdgeScrollSpeedY = 0.0f;
	}

	/*UE_LOG(LogTemp, Warning, TEXT("ARTS_PlayerController::EdgeScroll() | EdgeScrollSpeed_X: %f EdgeScrollSpeed_Y: %f"),
		EdgeScrollSpeedX, EdgeScrollSpeedY);*/

	DeltaSpeed_X = EdgeScrollSpeedY * MovementSpeedModifier;

	DeltaSpeed_Y = EdgeScrollSpeedX * MovementSpeedModifier;

	return FVector(DeltaSpeed_X, DeltaSpeed_Y, 0.0f);
}

void ARTS_PlayerController::SecondaryAction()
{
	if (bIsBuildingModeActive)
	{
		ConstructionPreviewRef->DestroyPreview();
	}
	else
	{
		if (bIsUnitSelected)
		{
			FHitResult HitResultLocal;
			ETraceTypeQuery TraceTypeLocal;

			TraceTypeLocal = UEngineTypes::ConvertToTraceType(ECC_Visibility);

			GetHitResultUnderCursorByChannel(TraceTypeLocal, false, HitResultLocal);

			TargetLocation = HitResultLocal.Location;
			HitActor = Cast<AActor>(HitResultLocal.GetActor());

			UnitMovement();

			VehicleMovement();
		}
		else
		{
			GetUnitHUD();
		}
	}
}

void ARTS_PlayerController::PrimaryAction_Pressed()
{
	if (!bIsBuildingModeActive)
	{
		MarqueeRef->OnInputStart();

		HoldingTime = 0.0f;

		bIsHoldingInput = true;
	}
}

void ARTS_PlayerController::PrimaryAction_Released()
{
	if (!bIsBuildingModeActive)
	{
		MarqueeRef->OnInputRelease(HoldingTime);

		bIsHoldingInput = false;
	}
}

void ARTS_PlayerController::ReferenceCasts()
{
	CameraPawnRef = (ARTS_CameraPawn*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!CameraPawnRef)
	{
		UE_LOG(LogTemp, Error, TEXT("ARTS_PlayerController::ReferenceCasts() Bad CameraPawn Class"));
	}

	GameStateRef = (ARTS_GameState*)GetWorld()->GetGameState();
	if (!GameStateRef)
	{
		UE_LOG(LogTemp, Error, TEXT("ARTS_PlayerController::ReferenceCasts() Bad GameState Class"));
	}

	MarqueeRef = (ARTS_MarqueeSelection*)this->GetHUD();
	if (!MarqueeRef)
	{
		UE_LOG(LogTemp, Error, TEXT("ARTS_PlayerController::ReferenceCasts() Bad MarqueeSelection Class"));
	}
}

void ARTS_PlayerController::UpdateSelection()
{
	if (bIsHoldingInput)
	{
		HoldingTime += DeltaTime;

		MarqueeRef->OnInputHold();
	}
}

void ARTS_PlayerController::GetUnitHUD()
{
	FHitResult HitResultLocal;

	GetHitResultUnderCursorForObjects(SelectableObjectsEnum, true, HitResultLocal);

	if (HitResultLocal.GetActor() != nullptr)
	{
		if (DisplayUnitHUD_Delegate.IsBound())
		{
			DisplayUnitHUD_Delegate.Broadcast(HitResultLocal.GetActor(), false);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ARTS_PlayerController::GetUnitHUD() | HitResultLocal.Actor: nullptr"));
		}
	}
}

void ARTS_PlayerController::UnitMovement()
{
	FVector DecalSize(32.0f, 64.0f, 64.0f);
	FRotator DecalRotation(-90.0f, 0.0f, 0.0f);
	bool ClassNotEqualLocal;

	if (SelectedUnits.Num() >= 1)
	{
		for (auto Unit : SelectedUnits)
		{
			UnitAI = Cast<AAIController>(Unit->GetController());

			if (UnitAI != nullptr)
			{
				if (IsValid(PreviousLocationDecal))
				{
					PreviousLocationDecal->DestroyComponent();
					UnitAI->StopMovement();
				}

				UClass* HitClass = HitActor->GetClass()->GetSuperClass();
				ClassNotEqualLocal = UKismetMathLibrary::NotEqual_ClassClass(HitClass, ATP_VehiclePawn::StaticClass());

				if (ClassNotEqualLocal)
				{
					UE_LOG(LogTemp, Warning, TEXT("AARTS_PlayerController::UnitMovement() | HitActor: %s StaticClass(): %s"), 
						*HitActor->GetClass()->GetName(), *ATP_VehiclePawn::StaticClass()->GetName());

					PreviousLocationDecal = UGameplayStatics::SpawnDecalAtLocation(HitActor, DecalMaterial, DecalSize, TargetLocation, DecalRotation);
				}
				else
				{					
					TargetVehicle = Cast<ATP_VehiclePawn>(HitActor);

					UE_LOG(LogTemp, Error, TEXT("AARTS_PlayerController::UnitMovement() | ClassNotEqual: %s"),
						*TargetVehicle->GetName());
				}
				
				UnitAI->MoveToLocation(TargetLocation, 150.0f, false);
			}
		}
	}
}

void ARTS_PlayerController::VehicleMovement()
{
	FVector DecalSize(32.0f, 64.0f, 64.0f);
	FRotator DecalRotation(-90.0f, 0.0f, 0.0f);

	if (SelectedGroundVehicles.Num() >= 1)
	{
		for (auto Unit : SelectedGroundVehicles)
		{
			if (IsValid(PreviousLocationDecal))
			{
				PreviousLocationDecal->DestroyComponent();
			}

			// FVector DebugLocation = Unit->ActorToMove->GetActorLocation();

			PreviousLocationDecal = UGameplayStatics::SpawnDecalAtLocation(HitActor, DecalMaterial, DecalSize, TargetLocation, DecalRotation);

			Unit->ReceiveMoveCommand(TargetLocation);
		}
	}
}

void ARTS_PlayerController::AIMoveCompleted()
{
	bool ClassNotEqualLocal;

	UClass* HitClass = HitActor->GetClass()->GetSuperClass();
	ClassNotEqualLocal = UKismetMathLibrary::NotEqual_ClassClass(HitClass, ATP_VehiclePawn::StaticClass());

	if (ClassNotEqualLocal)
	{
		RemoveLocationDecals();
	}
	else
	{
		GetInVehicle();
	}
}

void ARTS_PlayerController::RemoveLocationDecals()
{
	if (IsValid(PreviousLocationDecal))
	{
		PreviousLocationDecal->DestroyComponent();
	}
}

void ARTS_PlayerController::GetInVehicle()
{
	for (auto Unit : SelectedUnits)
	{
		SelectedUnit = Unit;

		if (PassengerSent < TargetVehicle->MaxPassengers)
		{
			SelectedUnit->SetActorHiddenInGame(true);

			float DestLocationX = FMath::RandRange(0.0f, 600.0f);
			float DestLocationY = FMath::RandRange(0.0f, 600.0f);
			FVector DestLocation = FVector(DestLocationX, DestLocationY, -250.0f);
			SelectedUnit->TeleportTo(DestLocation, FRotator(0.0f, 0.0f, 0.0f));

			SelectedUnit->SetDeselectedDecal();

			SelectedUnit->bIsSelected = false;

			int32 IndexOfArray = SetPassengers.AddUnique(SelectedUnit);
			PassengerSent = IndexOfArray + 1;

			SelectedUnit->PassengerIn = TargetVehicle;

			TargetVehicle->bHasPassengers = true;
		}
		else
		{
			break;
		}
	}

	MarqueeRef->ClearSelectedUnits();

	TargetVehicle->GetPassengers(SetPassengers);

	SelectedUnits.Empty();

	SetPassengers.Empty();

	PassengerSent = 0;
}

void ARTS_PlayerController::SetSelectedUnits(TArray<ARTS_Cpp_TutorialCharacter*> InSelectedUnits, TArray<ATP_VehiclePawn*> SelectedVehicles)
{
	SelectedUnits = InSelectedUnits;
	SelectedGroundVehicles = SelectedVehicles;

	if ((SelectedUnits.Num() >= 1) || (SelectedGroundVehicles.Num() >= 1))
	{
		bIsUnitSelected = true;
	}
}

void ARTS_PlayerController::SpawnUnitDebug()
{
	FHitResult HitResult;
	FActorSpawnParameters SpawnInfo;
	FVector UnitLocationLocal;
	FRotator UnitRotationLocal = FRotator(0, 0, 0);

	UnitLocationLocal = URTS_FuncLib::SetCursorWorldPosition(this, 10000.0f);
	UnitLocationLocal.Z = 150.0f;

	// For Debug Only
	// GameStateRef->GetAllUnits();

	GetWorld()->SpawnActor<ARTS_Cpp_TutorialCharacter>(UnitForDebug, UnitLocationLocal, UnitRotationLocal, SpawnInfo);
}

void ARTS_PlayerController::SpawnVehicleDebug()
{
	FHitResult HitResult;
	FActorSpawnParameters SpawnInfo;
	FVector UnitLocationLocal;
	FRotator UnitRotationLocal = FRotator(0, 0, 0);

	UnitLocationLocal = URTS_FuncLib::SetCursorWorldPosition(this, 10000.0f);
	UnitLocationLocal.Z = 150.0f;

	GetWorld()->SpawnActor<ATP_VehiclePawn>(VehicleForDebug, UnitLocationLocal, UnitRotationLocal, SpawnInfo);
}

void ARTS_PlayerController::SetBuildingModeActive(bool bIsActive)
{
	bIsBuildingModeActive = bIsActive;
}
