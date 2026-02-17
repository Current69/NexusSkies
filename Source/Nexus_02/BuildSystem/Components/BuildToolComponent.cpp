#include "BuildToolComponent.h"

// We need these includes for tracing and spawning
#include "Nexus_02/BuildSystem/Actors/BuildPieceActor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
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
	const FVector SpawnLoc = CandidateTransform.GetLocation();
	const FRotator SpawnRot = CandidateTransform.GetRotation().Rotator();
	
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

// Performs a non-destructive camera trace used for preview positioning.
bool UBuildToolComponent::GetPlacementPreview(FHitResult& OutHit) // Computes preview trace, transform, and validity.
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()); // Get the owning character (we assume the component is on a character).
	if (!OwnerCharacter) // Abort if we have no valid owning character.
	{
		bPlacementValid = false; // Mark preview invalid because we cannot compute placement.
		return false; // Return false to indicate we did not get a usable preview hit.
	}

	FVector Start = FVector::ZeroVector; // Will hold the camera/eyes start location for the trace.
	FRotator ViewRotation = FRotator::ZeroRotator; // Will hold the camera/eyes rotation for the trace direction.
	OwnerCharacter->GetActorEyesViewPoint(Start, ViewRotation); // Get viewpoint from the character's eyes/camera.

	const float TraceDistance = 5000.f; // How far forward we trace for placement preview.
	const FVector End = Start + (ViewRotation.Vector() * TraceDistance); // Compute trace end point from view direction.

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		OutHit,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	if (!bHit)
	{
		bPlacementValid = false;
		return false;
	}

	const FVector SpawnLoc = OutHit.ImpactPoint;
	const float OwnerYaw = OwnerCharacter->GetActorRotation().Yaw;
	const FRotator SpawnRot = FRotator(0.f, OwnerYaw, 0.f);

	CandidateTransform = FTransform(SpawnRot, SpawnLoc, FVector::OneVector);

	bPlacementValid = OutHit.bBlockingHit;

	return true;
}