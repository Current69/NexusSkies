#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/BuildPieceDefinition.h"
#include "BuildToolComponent.generated.h"

// Forward declare so we don't include more headers than needed
class ABuildPieceActor;

UCLASS(ClassGroup=(BuildSystem), Blueprintable, meta=(BlueprintSpawnableComponent))
class NEXUS_02_API UBuildToolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Constructor
	UBuildToolComponent();

	// The class we will spawn when the player "places" a piece.
	// This lets us choose BP_Build_Foundation_Wood from the editor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Build Tool")
	TSubclassOf<ABuildPieceActor> SelectedBuildClass;

	// Stores the latest candidate transform for preview placement (location + rotation + scale).
	UPROPERTY(BlueprintReadOnly, Category="Build|Preview")
	FTransform CandidateTransform; // The transform that preview ghost should use each update.

	// Stores whether the current candidate placement is valid according to C++ rules.
	UPROPERTY(BlueprintReadOnly, Category="Build|Preview")
	bool bPlacementValid = false; // True means preview should show "valid" material, false means "invalid".

	// The data asset that defines the currently selected build piece
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Build|Selection")
	UBuildPieceDefinition* SelectedDefinition;

	// Call this to attempt a placement at the camera trace hit point.
	UFUNCTION(BlueprintCallable, Category="Build Tool")
	bool TryPlaceSelected();

	// Camera trace.
	UFUNCTION(BlueprintCallable, Category="Build Tool|Preview")
	bool GetPlacementPreview(FHitResult& OutHit);

	// Returns the current preview transform calculated by the build tool.
	UFUNCTION(BlueprintPure, Category="Build|Preview")
	FTransform GetCandidateTransform() const { return CandidateTransform; } // Blueprint-safe accessor for preview transform.

	// Returns whether the current preview transform is considered valid.
	UFUNCTION(BlueprintPure, Category="Build|Preview")
	bool IsPlacementValid() const { return bPlacementValid; } // Blueprint-safe accessor for validity state.

protected:
	// We don't need Tick for the simplest spawn test.
	virtual void BeginPlay() override;

private:
	// Do a camera line trace and return true if we hit something.
	bool DoPlacementTrace(FHitResult& OutHit) const;
};