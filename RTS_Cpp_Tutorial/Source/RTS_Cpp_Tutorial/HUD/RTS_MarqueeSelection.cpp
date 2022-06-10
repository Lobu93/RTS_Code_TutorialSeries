// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_MarqueeSelection.h"
#include "../Player/RTS_PlayerController.h"
#include "../RTS_Cpp_TutorialCharacter.h"

void ARTS_MarqueeSelection::DrawHUD()
{
	Super::DrawHUD();

	// TSubclassOf<ARTS_Cpp_TutorialCharacter> ClassFilter;
	// ARTS_Cpp_TutorialCharacter ClassFilter;

	if (bIsDrawing)
	{
		DrawMarquee();

		GetActorsInSelectionRectangle(ClassFilter, ClickedLocation, HoldingLocation, UnitsInSelectionArea,
			false, false);

		for (auto UnitElement : UnitsInSelectionArea)
		{
			if (UnitElement->GetClass()->IsChildOf<ARTS_Cpp_TutorialCharacter>())
			{
				/*UE_LOG(LogTemp, Warning, TEXT("ARTS_MarqueeSelection::DrawHUD() | UnitsInSelectionArea.Num(): %d"),
					UnitsInSelectionArea.Num());*/

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
	if (SelectedUnits.Num() >= 1)
	{
		for (auto Unit : SelectedUnits)
		{
			Unit->SetDeselectedDecal();
			Unit->bIsSelected = false;
		}

		SelectedUnits.Empty();
	}

	ControllerRef->GetMousePosition(ClickedLocation.X, ClickedLocation.Y);
	HoldingLocation = ClickedLocation;
	bIsDrawing = true;
}

void ARTS_MarqueeSelection::OnInputRelease(float HoldTime)
{
	bIsDrawing = false;
	ControllerRef->SetSelectedUnits(SelectedUnits);
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
