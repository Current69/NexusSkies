#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	// Call this to attempt a placement at the camera trace hit point.
	UFUNCTION(BlueprintCallable, Category="Build Tool")
	bool TryPlaceSelected();

protected:
	// We don't need Tick for the simplest spawn test.
	virtual void BeginPlay() override;

private:
	// Do a camera line trace and return true if we hit something.
	bool DoPlacementTrace(FHitResult& OutHit) const;
};