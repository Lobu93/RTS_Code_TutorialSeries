// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Library/Structures.h"
#include "Library/EnumList.h"
#include "SlateCore.h"
#include "RTS_Cpp_TutorialCharacter.generated.h"

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

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	EFemaleNames FemaleName;
	EMaleNames MaleName;
	ESurnames Surname;
	bool bIsSpawnedIn = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Master")
	FRTS_UnitProfileStruct UnitProfile;

	UPROPERTY()
	UMaterialInstanceDynamic* DynMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Unit|Configs")
	UMaterialInterface* MaterialToEdit;

	UPROPERTY(EditAnywhere, Category = "Unit|Configs")
	FLinearColor NewColor;

	/*UPROPERTY(EditDefaultsOnly, Category = "Unit|MeshDebug")
	UMaterialInterface* MaterialInterface;

	UPROPERTY(EditAnywhere, Category = "Unit|MeshDebug")
	FLinearColor NewColor;

	UPROPERTY()
	UMaterialInstanceDynamic* MeshMaterialInstanceLocal;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Details")
	int32 UnitSex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Details")
	FString UnitName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Stats")
	int32 UnitAge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Legend")
	FSlateBrush UnitImage;

	EFemaleNames SetFemaleName();

	EMaleNames SetMaleName();

	ESurnames SetSurname();

	FString GetRandomName(int32 Sex);

	void SetUnitFeatures();
};

