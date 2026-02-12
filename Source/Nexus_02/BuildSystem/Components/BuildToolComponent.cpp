#include "BuildToolComponent.h"

// We need these includes for tracing and spawning
#include "Nexus_02/BuildSystem/Actors/BuildPieceActor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UBuildToolComponent::UBuildToolComponent()
{
	// We do not need per-frame updates for the "press key to place" milestone.
	PrimaryComponentTick.bCanEverTick = false;
}

void UBuildToolComponent::BeginPlay()
{
	Super::BeginPlay();

	// Nothing required yet.  We keep BeginPlay in case we add init later.
}

bool UBuildToolComponent::DoPlacementTrace(FHitResult& OutHit) const
{
	// Get the owner (usually the player character).
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return false;
	}

	// Get the player's controller from the owner.
	APlayerController* PC = Cast<APlayerController>(OwnerActor->GetInstigatorController());
	if (!PC)
	{
		// Fallback: try owner as pawn
		APawn* OwnerPawn = Cast<APawn>(OwnerActor);
		if (OwnerPawn)
		{
			PC = Cast<APlayerController>(OwnerPawn->GetController());
		}
	}

	if (!PC)
	{
		return false;
	}

	// Get camera viewpoint
	FVector CamLoc;
	FRotator CamRot;
	PC->GetPlayerViewPoint(CamLoc, CamRot);

	// Trace forward
	const FVector TraceStart = CamLoc;
	const FVector TraceEnd = TraceStart + (CamRot.Vector() * 5000.0f);

	FCollisionQueryParams Params;
	Params.bTraceComplex = false;
	Params.AddIgnoredActor(OwnerActor);

	// Do a visibility trace
	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	const bool bHit = World->LineTraceSingleByChannel(
		OutHit,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		Params
	);

	// Debug line so you can SEE the trace in-editor (remove later).
	DrawDebugLine(World, TraceStart, TraceEnd, bHit ? FColor::Green : FColor::Red, false, 2.0f, 0, 1.0f);

	return bHit;
}

bool UBuildToolComponent::TryPlaceSelected()
{
	// Debug line
	UE_LOG(LogTemp, Warning, TEXT("TryPlaceSelected Fired"));
	
	// Must have a selected class
	if (!SelectedBuildClass)
	{
		return false;
	}

	FHitResult Hit;
	if (!DoPlacementTrace(Hit))
	{
		return false;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	// Spawn at the hit point.
	// Rotation is flat for now (we'll improve later).
	const FVector SpawnLoc = Hit.ImpactPoint;
	const FRotator SpawnRot = FRotator::ZeroRotator;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ABuildPieceActor* Spawned = World->SpawnActor<ABuildPieceActor>(
		SelectedBuildClass,
		SpawnLoc,
		SpawnRot,
		SpawnParams
	);

	return (Spawned != nullptr);
}