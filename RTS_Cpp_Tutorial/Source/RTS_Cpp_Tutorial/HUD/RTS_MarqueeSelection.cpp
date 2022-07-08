// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_MarqueeSelection.h"
#include "../Player/RTS_PlayerController.h"
#include "../RTS_Cpp_TutorialCharacter.h"
#include "../TP_Vehicle/TP_VehiclePawn.h"

void ARTS_MarqueeSelection::DrawHUD()
{
	Super::DrawHUD();

	if (bIsDrawing)
	{
		DrawMarquee();

		GetActorsInSelectionRectangle(ClassFilter, ClickedLocation, HoldingLocation, UnitsInSelectionArea,
			false, false);

		GetActorsInSelectionRectangle(ClassFilterVehicle, ClickedLocation, HoldingLocation, VehicleUnitsInSelectedArea,
			false, false);
	}
}

// Called when the game starts or when spawned
void ARTS_MarqueeSelection::BeginPlay()
{
	Super::BeginPlay();

	// Cast references once for the entire code, reduce system drain.
	ControllerRef = (ARTS_PlayerController*)GetWorld()->GetFirstPlayerController();
	if (!ControllerRef)
	{
		UE_LOG(LogTemp, Error, TEXT("ARTS_MarqueeSelection::BeginPlay() Bad PlayerController Class"));
	}
}

void ARTS_MarqueeSelection::OnInputStart()
{
	if (SelectedUnits.Num() >= 1 || SelectedVehicles.Num() >= 1)
	{
		ClearSelectedUnits();
		ClearVehicles();
		ControllerRef->SetSelectedUnits(SelectedUnits, SelectedVehicles);
	}

	ControllerRef->GetMousePosition(ClickedLocation.X, ClickedLocation.Y);
	HoldingLocation = ClickedLocation;
	bIsDrawing = true;
}

void ARTS_MarqueeSelection::OnInputRelease(float HoldTime)
{
	bIsDrawing = false;
	SetUnits();
	SetVehicles();
	ControllerRef->SetSelectedUnits(SelectedUnits, SelectedVehicles);
}

void ARTS_MarqueeSelection::OnInputHold()
{
	ControllerRef->GetMousePosition(HoldingLocation.X, HoldingLocation.Y);
}

void ARTS_MarqueeSelection::DrawMarquee()
{
	FLinearColor NoColorLocal = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
	FLinearColor MarqueecolorLocal = FLinearColor(0.091f, 0.536f, 0.761f, 0.6f);
	FVector2D ScreenLocal;

	DrawRect(NoColorLocal, ClickedLocation.X, ClickedLocation.Y, 0.0f, 0.0f);

	DrawRect(NoColorLocal, HoldingLocation.Y, HoldingLocation.Y, 0.0f, 0.0f);

	ScreenLocal = HoldingLocation - ClickedLocation;

	DrawRect(MarqueecolorLocal, ClickedLocation.X, ClickedLocation.Y, ScreenLocal.X, ScreenLocal.Y);
}

void ARTS_MarqueeSelection::ClearSelectedUnits()
{
	for (auto Unit : SelectedUnits)
	{
		Unit->SetDeselectedDecal();
		Unit->bIsSelected = false;
		ControllerRef->bIsUnitSelected = false;
	}

	SelectedUnits.Empty();
}

void ARTS_MarqueeSelection::ClearVehicles()
{
	for (auto Unit : SelectedVehicles)
	{
		Unit->SetDeselectedDecal();
		Unit->bIsSelected = false;
		ControllerRef->bIsUnitSelected = false;
	}

	SelectedVehicles.Empty();
}

void ARTS_MarqueeSelection::SetUnits()
{
	for (auto UnitElement : UnitsInSelectionArea)
	{
		if (UnitElement->GetClass()->IsChildOf<ARTS_Cpp_TutorialCharacter>())
		{
			SelectedUnit = Cast<ARTS_Cpp_TutorialCharacter>(UnitElement);

			SelectedUnits.AddUnique(SelectedUnit);

			for (auto Unit : SelectedUnits)
			{
				Unit->SetSelectedDecal();
				Unit->bIsSelected = true;
			}
		}
	}
}

void ARTS_MarqueeSelection::SetVehicles()
{
	for (auto UnitElement : VehicleUnitsInSelectedArea)
	{
		if (UnitElement->GetClass()->IsChildOf<ATP_VehiclePawn>())
		{
			/*UE_LOG(LogTemp, Warning, TEXT("ARTS_MarqueeSelection::DrawHUD() | VehicleUnitsInSelectedArea.Num(): %d"),
				VehicleUnitsInSelectedArea.Num());*/

			SelectedVehicle = Cast<ATP_VehiclePawn>(UnitElement);

			SelectedVehicles.AddUnique(SelectedVehicle);

			for (auto Unit : SelectedVehicles)
			{
				Unit->SetSelectedDecal();
				Unit->bIsSelected = true;
			}
		}
	}
}
