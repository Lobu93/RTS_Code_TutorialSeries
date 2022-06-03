// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Library/Structures.h"
#include "Library/EnumList.h"
#include "SlateCore.h"
#include "RTS_Cpp_TutorialCharacter.generated.h"

class ARTS_PlayerController;
class ARTS_GameState;

UCLASS(Blueprintable)
class ARTS_Cpp_TutorialCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARTS_Cpp_TutorialCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return SelectedDecal; }

private:
	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* SelectedDecal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* DeselectedDecal;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	EFemaleNames FemaleName;
	EMaleNames MaleName;
	ESurnames Surname;
	bool bIsSpawnedIn = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	ARTS_GameState* GameStateRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	ARTS_PlayerController* ControllerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Master")
	FRTS_UnitProfileStruct UnitProfile;

	UPROPERTY()
	UMaterialInstanceDynamic* DynMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Unit|Configs")
	UMaterialInterface* MaterialToEdit;

	UPROPERTY(EditAnywhere, Category = "Unit|Configs")
	FLinearColor NewColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Details")
	int32 UnitSex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Details")
	FString UnitName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Stats")
	int32 UnitAge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Details")
	int32 BirthYear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Details")
	int32 Birthday;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Details")
	int32 BirthMonth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Legend")
	FSlateBrush UnitImage;

	UPROPERTY(EditDefaultsOnly, Category = "Unit|Legend")
	FSlateBrush MaleImage;

	UPROPERTY(EditDefaultsOnly, Category = "Unit|Legend")
	FSlateBrush FemaleImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bool")
	bool bIsHidden;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gambiarra")
	bool bCanDisplayUnitHUD;

	UFUNCTION(BlueprintCallable, Category = "References", BlueprintNativeEvent, Category = "References")
	void ReferenceCast();
	virtual void ReferenceCast_Implementation();

	EFemaleNames SetFemaleName();

	EMaleNames SetMaleName();

	ESurnames SetSurname();

	FString GetRandomName(int32 Sex);

	void SetUnitFeatures();

	void SetUnitBirthday();

	void BirthdayCheck();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Unit")
	void DisplayUnitHUD(AActor* Actor, bool bBypass);
	virtual void DisplayUnitHUD_Implementation(AActor* Actor, bool bBypass);
};

